#include <napi.h>
#include "nemeeting_manager.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return NEMeetingManager::Init(env, exports);
}

NODE_API_MODULE(nemeeting, InitAll)
