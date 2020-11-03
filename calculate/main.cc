#include <napi.h>

Napi::Value Plus(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Plus function require two arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "The argument must be a number").ThrowAsJavaScriptException();
    return env.Null();
  }

  double a = info[0].As<Napi::Number>().DoubleValue();
  double b = info[1].As<Napi::Number>().DoubleValue();

  return Napi::Number::New(env, a + b);
}

Napi::Value Minus(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();


  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Minus function require two arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "The argument must be a number").ThrowAsJavaScriptException();
    return env.Null();
  }


  double a = info[0].As<Napi::Number>().DoubleValue();
  double b = info[1].As<Napi::Number>().DoubleValue();

  return Napi::Number::New(env, a - b);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "plus"), Napi::Function::New(env, Plus));
  exports.Set(Napi::String::New(env, "minus"), Napi::Function::New(env, Minus));
  return exports;
}

NODE_API_MODULE(calculate, Init)
