#include "nemeeting_manager.h"

Napi::ThreadSafeFunction tsfn;

Napi::FunctionReference emit_func_ref;
Napi::ThreadSafeFunction emit_tsfn;

void createThreadSafeFunction(const Napi::CallbackInfo &info, int index) {
  tsfn = Napi::ThreadSafeFunction::New(
      info.Env(),
      info[index].As<Napi::Function>(),
      "tsfn",
      0,
      1
  );
}

void runJsCallBack (NEErrorCode errorCode, const std::string &errorMessage) {
    auto callback = [=](Napi::Env env, Napi::Function jsCallback) {
        Napi::Object obj = Napi::Object::New(env);
        obj.Set(Napi::String::New(env, "errorCode"), Napi::Number::New(env, errorCode));
        obj.Set(Napi::String::New(env, "errorMessage"), Napi::String::New(env, errorMessage));
        jsCallback.Call({ obj });
    };
    tsfn.BlockingCall( callback );
    tsfn.Release();
}

// Define NEMeetingManager class and export module
Napi::Object NEMeetingManager::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "NEMeetingManager", {
        InstanceMethod<&NEMeetingManager::initialize>("initialize"),
        InstanceMethod<&NEMeetingManager::unInitialize>("unInitialize"),
        InstanceMethod<&NEMeetingManager::isInitialized>("isInitialized"),
        InstanceMethod<&NEMeetingManager::login>("login"),
        InstanceMethod<&NEMeetingManager::logout>("logout"),
        InstanceMethod<&NEMeetingManager::isLoggedIn>("isLoggedIn"),
        InstanceMethod<&NEMeetingManager::startMeeting>("startMeeting"),
        InstanceMethod<&NEMeetingManager::callEmit>("callEmit")
    });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);

    exports.Set("NEMeetingManager", func);

    return exports;
}

NEMeetingManager::NEMeetingManager(const Napi::CallbackInfo &info) : Napi::ObjectWrap<NEMeetingManager>(info) {
}

NEMeetingManager::~NEMeetingManager() {
  if (!emit_func_ref.IsEmpty()) {
      emit_func_ref.Reset();
  }
}

void NEMeetingManager::initialize(const Napi::CallbackInfo &info) {
  NEMeetingSDKConfig config;
  config.getAppInfo()->ProductName("NetEase Meeting");
  config.getAppInfo()->OrganizationName("NetEase");
  config.getAppInfo()->ApplicationName("Meeting");
  config.setDomain("yunxin.163.com");

  createThreadSafeFunction(info, 0);
  NEMeetingSDK::getInstance()->initialize(config, [&](NEErrorCode errorCode, const std::string& errorMessage) {
    auto ipcMeetingService = NEMeetingSDK::getInstance()->getMeetingService();
    if (ipcMeetingService) {
      ipcMeetingService->addMeetingStatusListener(this);
    }

    runJsCallBack(errorCode, errorMessage);
  });
}

void NEMeetingManager::unInitialize(const Napi::CallbackInfo &info) {
  createThreadSafeFunction(info, 0);
  NEMeetingSDK::getInstance()->unInitialize([&](NEErrorCode errorCode, const std::string& errorMessage) {
    runJsCallBack(errorCode, errorMessage);
  });
}

Napi::Value NEMeetingManager::isInitialized(const Napi::CallbackInfo &info) {
  bool isInit = NEMeetingSDK::getInstance()->isInitialized();
  return Napi::Boolean::New(info.Env(), isInit);
}

void NEMeetingManager::login(const Napi::CallbackInfo &info) {
  auto ipcAuthService = NEMeetingSDK::getInstance()->getAuthService();
  if (ipcAuthService) {
    const std::string appKey = info[0].As<Napi::String>();
    const std::string accountId = info[1].As<Napi::String>();
    const std::string accountToken = info[2].As<Napi::String>();

    createThreadSafeFunction(info, 3);
    ipcAuthService->login(appKey, accountId, accountToken, [&](NEErrorCode errorCode, const std::string& errorMessage) {
      if (errorCode == ERROR_CODE_SUCCESS) {
        m_logged_in = true;
      } else {
        m_logged_in = false;
      }
      runJsCallBack(errorCode, errorMessage);
    });
  }
}

void NEMeetingManager::logout(const Napi::CallbackInfo &info) {
  auto authService = NEMeetingSDK::getInstance()->getAuthService();
  if (authService) {
    createThreadSafeFunction(info, 0);
    authService->logout([&](NEErrorCode errorCode, const std::string& errorMessage) {
      if (errorCode == ERROR_CODE_SUCCESS) {
        m_logged_in = false;
      }
      runJsCallBack(errorCode, errorMessage);
    });
  }
}

Napi::Value NEMeetingManager::isLoggedIn(const Napi::CallbackInfo &info) {
  return Napi::Boolean::New(info.Env(), m_logged_in);
}

void NEMeetingManager::startMeeting(const Napi::CallbackInfo &info) {
  auto meetingService = NEMeetingSDK::getInstance()->getMeetingService();
  if (meetingService) {
    const std::string nickname = info[0].As<Napi::String>();
    bool audio = info[1].As<Napi::Boolean>();
    bool video = info[2].As<Napi::Boolean>();

    NEStartMeetingParams params;
    params.displayName = nickname;

    NEStartMeetingOptions options;
    options.noAudio = !audio;
    options.noVideo = !video;

    createThreadSafeFunction(info, 3);
    meetingService->startMeeting(params, options, [&](NEErrorCode errorCode, const std::string& errorMessage) {
      runJsCallBack(errorCode, errorMessage);
    });
  }
}

void NEMeetingManager::onMeetingStatusChanged(int status, int code) {
  if (emit_func_ref == nullptr) return;

    auto callback = [=](Napi::Env env, Napi::Function jsCallback) {
        Napi::Object obj = Napi::Object::New(env);
        obj.Set(Napi::String::New(env, "status"), Napi::Number::New(env, status));
        obj.Set(Napi::String::New(env, "code"), Napi::Number::New(env, code));
        jsCallback.Call({ 
            Napi::String::New(env, "status"),
            obj
        });
    };

    emit_tsfn.BlockingCall( callback );
}

void NEMeetingManager::callEmit (const Napi::CallbackInfo &info) {
    emit_func_ref = Napi::Persistent(info[0].As<Napi::Function>());
    emit_tsfn = Napi::ThreadSafeFunction::New(
        emit_func_ref.Env(),
        emit_func_ref.Value(),
        "emit_tsfn",
        0,
        1
    );
}