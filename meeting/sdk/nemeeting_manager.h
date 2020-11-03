#ifndef NEMEETINGMANAGER_H
#define NEMEETINGMANAGER_H

#include <napi.h>
#include "nemeeting_sdk_interface_include.h"

USING_NS_NNEM_SDK_INTERFACE;

class NEMeetingManager
        : public Napi::ObjectWrap<NEMeetingManager>
        , public NEMeetingStatusListener
{
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit NEMeetingManager(const Napi::CallbackInfo& info);
    ~NEMeetingManager();

    void initialize(const Napi::CallbackInfo& info);
    void unInitialize(const Napi::CallbackInfo& info);
    Napi::Value isInitialized(const Napi::CallbackInfo& info);
    void login(const Napi::CallbackInfo& info);
    void logout(const Napi::CallbackInfo& info);
    Napi::Value isLoggedIn(const Napi::CallbackInfo& info);
    void startMeeting(const Napi::CallbackInfo& info);
    void callEmit(const Napi::CallbackInfo& info);
    virtual void onMeetingStatusChanged(int status, int code) override;

  private:
    bool m_logged_in = false;
};

#endif // NEMEETINGMANAGER_H