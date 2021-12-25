// This code is PUBLIC DOMAIN, and is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND.

#include "userid.hh"

#if !defined(_WIN32)
#include <grp.h>
#else
// Mocks for Windows

using gid_t = uint32_t;
struct group {
  /**
   * group name
   **/
  char *gr_name;
  /**
   * group password
   **/
  char *gr_passwd;
  /**
   * group ID
   **/
  gid_t gr_gid;
  /**
   * NULL-terminated array of pointers to names of group members
   **/
  char **gr_mem;
};

/**
 * The getgrnam() function returns a pointer to a structure containing
 * the broken-out fields of the record in the group database (e.g., the
 * local group file /etc/group, NIS, and LDAP) that matches the group
 * name name.
 */
auto getgrnam(const char *name) -> struct group *;
#endif

using Napi::CallbackInfo;
using Napi::Error;
using Napi::Number;
using Napi::String;
using Napi::TypeError;

auto userid::Gid(const CallbackInfo &info) -> Number {
  auto env = info.Env();

  if (info.Length() < 1) {
    throw TypeError::New(env, "Wrong number of arguments");
  }

  if (!info[0].IsString()) {
    throw TypeError::New(env, "Argument must be a string");
  }

  auto name = std::string(info[0].As<String>());

  auto group = getgrnam(name.c_str());

  if (!group) {
    throw Error::New(env, "groupname not found");
  }

  return Number::New(env, group->gr_gid);
}
