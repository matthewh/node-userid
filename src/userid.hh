// This code is PUBLIC DOMAIN, and is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND.

#pragma once

#include <napi.h>

namespace userid {
using Napi::Array;
using Napi::CallbackInfo;
using Napi::Number;
using Napi::Object;
using Napi::String;

auto GroupName(const CallbackInfo &info) -> String;
auto Gids(const CallbackInfo &info) -> Array;
auto Gid(const CallbackInfo &info) -> Number;
auto UserName(const CallbackInfo &info) -> String;
auto Ids(const CallbackInfo &info) -> Object;
} // namespace userid
