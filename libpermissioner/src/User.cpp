#include <permissioner/User.h>

#include <boost/optional.hpp>
#include <cerrno>
#include <cstring>
#include <pwd.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>

void User::parseUserName(std::string const &userNameStr) {
  if (userNameStr == "-") {
    userName = boost::none;
    uid = -1;
    return;
  }

  long size_max = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (size_max <= 0) {
    std::stringstream msg;
    msg << "invalid maximum size of passwd entry structure " << size_max;
    throw std::runtime_error(msg.str());
  }

  struct passwd pw_buf, *pw;
  char buf[size_max];
  if (getpwnam_r(userNameStr.c_str(), &pw_buf, buf, sizeof(buf), &pw) != 0) {
    std::string err(strerror(errno));
    std::stringstream msg;
    msg << "looking up user name \"" << userNameStr << "\" failed: " << err;
    throw std::runtime_error(msg.str());
  }
  if (! pw) {
    std::stringstream msg;
    msg << "user name \"" << userNameStr << "\" not found";
    throw std::runtime_error(msg.str());
  }

  userName = pw->pw_name;
  uid = pw->pw_uid;
}

boost::optional<std::string> const & User::getUserName() const {
  return userName;
}

int User::getUid() const {
  return uid;
}
