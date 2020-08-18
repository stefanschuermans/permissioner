#include <permissioner/Tree.h>

#include <permissioner/Group.h>
#include <permissioner/Permissions.h>
#include <permissioner/User.h>
#include <permissioner/StringUtils.h>

#include <boost/filesystem.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

void Tree::parseParams(std::string const &paramStr) {
  // format of paramStr is: <user> " " <group> " " <permissions> " " <root>
  // user, group, permissions may be - for no change
  std::string userStr, groupStr, permissionsStr, rootStr;

  std::string::size_type pos = 0;
  StringUtils::getNextField(paramStr, pos, userStr, "user");
  StringUtils::getNextField(paramStr, pos, groupStr, "group");
  StringUtils::getNextField(paramStr, pos, permissionsStr, "permissions");
  if (pos >= paramStr.length()) {
    std::stringstream msg;
    msg << "<root> field missing in \"" << paramStr << "\"";
    throw std::runtime_error(msg.str());
  }

  try {
    user.parseUserName(userStr);
  } catch (std::exception const & e) {
    std::stringstream msg;
    msg << "invalid <user> field \"" << userStr << "\" in \""
        << paramStr << "\": " << e.what();
    throw std::runtime_error(msg.str());
  }

  try {
    group.parseGroupName(groupStr);
  } catch (std::exception const & e) {
    std::stringstream msg;
    msg << "invalid <group> field \"" << groupStr << "\" in \""
        << paramStr << "\": " << e.what();
    throw std::runtime_error(msg.str());
  }

  try {
    permissions.parseParams(permissionsStr);
  } catch (std::exception const & e) {
    std::stringstream msg;
    msg << "invalid <permissions> field \"" << permissionsStr << "\" in \""
        << paramStr << "\": " << e.what();
    throw std::runtime_error(msg.str());
  }

  root = rootStr;
}
