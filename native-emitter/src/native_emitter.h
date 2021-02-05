#ifndef NATIVEEMITTER_H
#define NATIVEEMITTER_H

#include <napi.h>

class NativeEmitter : public Napi::ObjectWrap<NativeEmitter> {
  public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    explicit NativeEmitter(const Napi::CallbackInfo& info);

  private:
    Napi::Function GetEmit(const Napi::CallbackInfo& info);
    void CallMessage(const Napi::CallbackInfo& info);
};

#endif