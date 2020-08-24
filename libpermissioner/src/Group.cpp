#include <permissioner/Group.h>

#include <boost/optional.hpp>
#include <cerrno>
#include <cstring>
#include <grp.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>

void Group::parseGroupName(std::string const &groupNameStr) {
  if (groupNameStr == "-") {
    groupName = boost::none;
    gid = -1;
    return;
  }

  long size_max = sysconf(_SC_GETGR_R_SIZE_MAX);
  if (size_max <= 0) {
    std::stringstream msg;
    msg << "invalid maximum size of group entry structure " << size_max;
    throw std::runtime_error(msg.str());
  }

  struct group gr_buf, *gr;
  char buf[size_max];
  if (getgrnam_r(groupNameStr.c_str(), &gr_buf, buf, sizeof(buf), &gr) != 0) {
    std::string err(strerror(errno));
    std::stringstream msg;
    msg << "looking up group name \"" << groupNameStr << "\" failed: " << err;
    throw std::runtime_error(msg.str());
  }
  if (! gr) {
    std::stringstream msg;
    msg << "group name \"" << groupNameStr << "\" not found";
    throw std::runtime_error(msg.str());
  }

  groupName = gr->gr_name;
  gid = gr->gr_gid;
}

boost::optional<std::string> const & Group::getGroupName() const {
  return groupName;
}

int Group::getGid() const {
  return gid;
}
