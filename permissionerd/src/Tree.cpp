#include "Tree.h"

#include "Permissions.h"
#include "StringUtils.h"

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
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

  if (userStr == "-") {
    user = boost::none;
  } else {
    user = userStr;
  }
  if (groupStr == "-") {
    group = boost::none;
  } else {
    group = groupStr;
  }
  try {
    permissions.parseParams(permissionsStr);
  } catch (std::exception const & e) {
    std::stringstream msg;
    msg << "invalid <permissions> field in \"" << paramStr << "\": "
        << e.what();
    throw std::runtime_error(msg.str());
  }
  root = rootStr;
}
