#include <napi.h>
#include "native_emitter.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return NativeEmitter::Init(env, exports);
}

NODE_API_MODULE(addon, InitAll)
