// This code is PUBLIC DOMAIN, and is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND.

#include "userid.hh"

#if !defined(_WIN32)
#include <pwd.h>
#else
// Mocks for Windows

using uid_t = uint32_t;
using gid_t = uint32_t;
struct passwd {
  char *pw_name;   /* username */
  char *pw_passwd; /* user password */
  uid_t pw_uid;    /* user ID */
  gid_t pw_gid;    /* group ID */
  char *pw_gecos;  /* user information */
  char *pw_dir;    /* home directory */
  char *pw_shell;  /* shell program */
};

/**
 * The getpwuid() function returns a pointer to a structure containing the broken-out fields of the record in the
 * password database that matches the user ID uid.
 */
auto getpwuid(uid_t uid) -> struct passwd *;
#endif

using Napi::CallbackInfo;
using Napi::Error;
using Napi::String;
using Napi::TypeError;

auto userid::UserName(const CallbackInfo &info) -> String {
  const auto env = info.Env();

  if (info.Length() < 1) {
    throw TypeError::New(env, "Wrong number of arguments");
  }

  if (!info[0].IsNumber()) {
    throw TypeError::New(env, "Argument must be a number");
  }

  const auto n = info[0].As<Number>().Int32Value();
  const auto *const user = getpwuid(n);

  if (user == nullptr) {
    throw Error::New(env, "uid not found");
  }

  return String::New(env, user->pw_name);
}
