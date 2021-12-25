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
 * The getpwnam() function returns a pointer to a structure containing
 * the broken-out fields of the record in the password database (e.g.,
 * the local password file /etc/passwd, NIS, and LDAP) that matches the
 * username name.
 */
auto getpwnam(const char *name) -> struct passwd *;
#endif

using Napi::CallbackInfo;
using Napi::Error;
using Napi::Object;
using Napi::TypeError;

auto userid::Ids(const CallbackInfo &info) -> Object {
  auto env = info.Env();

  if (info.Length() < 1) {
    throw TypeError::New(env, "Wrong number of arguments");
  }

  if (!info[0].IsString()) {
    throw TypeError::New(env, "Argument must be a string");
  }

  auto name = std::string(info[0].As<String>());
  auto user = getpwnam(name.c_str());

  if (user == nullptr) {
    throw Error::New(env, "username not found");
  }

  auto ret = Object::New(env);

  ret["uid"] = Number::New(env, user->pw_uid);
  ret["gid"] = Number::New(env, user->pw_gid);

  return ret;
}
