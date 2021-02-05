#include "native_emitter.h"

Napi::Object NativeEmitter::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "NativeEmitter", {
    InstanceMethod("callMessage", &NativeEmitter::CallMessage)
  });

  Napi::FunctionReference *constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);
  constructor->SuppressDestruct();
  env.SetInstanceData(constructor);

  exports.Set("NativeEmitter", func);

  return exports;
}

NativeEmitter::NativeEmitter(const Napi::CallbackInfo &info)
  : Napi::ObjectWrap<NativeEmitter>(info) {
}

Napi::Function NativeEmitter::GetEmit(const Napi::CallbackInfo &info) {
  return info.This().As<Napi::Object>().Get("emit").As<Napi::Function>();
}

void NativeEmitter::CallMessage(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Function emit = this->GetEmit(info);
  emit.Call(info.This(), {
    Napi::String::New(env, "message"),
    Napi::String::New(env, "This is CallEmitter")
  });
}