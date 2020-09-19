/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/Tree.h>

#include <permissioner/Callback.h>
#include <permissioner/Group.h>
#include <permissioner/Permissions.h>
#include <permissioner/StringUtils.h>
#include <permissioner/User.h>

#include <boost/filesystem.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
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
  rootStr = paramStr.substr(pos);

  try {
    user.parseUserName(userStr);
  } catch (std::exception const &e) {
    std::stringstream msg;
    msg << "invalid <user> field \"" << userStr << "\" in \"" << paramStr
        << "\": " << e.what();
    throw std::runtime_error(msg.str());
  }

  try {
    group.parseGroupName(groupStr);
  } catch (std::exception const &e) {
    std::stringstream msg;
    msg << "invalid <group> field \"" << groupStr << "\" in \"" << paramStr
        << "\": " << e.what();
    throw std::runtime_error(msg.str());
  }

  try {
    permissions.parseParams(permissionsStr);
  } catch (std::exception const &e) {
    std::stringstream msg;
    msg << "invalid <permissions> field \"" << permissionsStr << "\" in \""
        << paramStr << "\": " << e.what();
    throw std::runtime_error(msg.str());
  }

  try {
    root = boost::filesystem::canonical(rootStr);
  } catch (std::exception const &e) {
    std::stringstream msg;
    msg << "invalid <root> field \"" << rootStr << "\" in \"" << paramStr
        << "\": " << e.what();
    throw std::runtime_error(msg.str());
  }
}

User const &Tree::getUser() const { return user; }

Group const &Tree::getGroup() const { return group; }

Permissions const &Tree::getPermissions() const { return permissions; }

boost::filesystem::path const &Tree::getRoot() const { return root; }

bool Tree::setPermissions(TreeMap const &exclude,
                          Callback &callback) const {
  return setPermissionsInternal(root, exclude, callback);
}

bool Tree::setPermissionsInternal(boost::filesystem::path const &path,
                                  TreeMap const &exclude,
                                  Callback &callback) const {
  try {
    if (boost::filesystem::is_regular_file(path)) {
      if (!setPermissionsOne(path, callback)) {
        return false;
      };
    } else if (boost::filesystem::is_directory(path)) {
      if (!setPermissionsOne(path, callback)) {
        return false;
      };
      for (boost::filesystem::directory_entry entry :
           boost::filesystem::directory_iterator(path)) {
        if (exclude.find(entry) != exclude.end()) {
          continue; // other tree -> skip here
        }
        // recurse
        if (!setPermissionsInternal(entry, exclude, callback)) {
          return false;
        }
      }
    }
  } catch (boost::filesystem::filesystem_error const &e) {
    // ignore filesystem errors for now, as this runs in a daemon in background
    (void)e;
  }
  return true;
}

bool Tree::setPermissionsOne(boost::filesystem::path const &path,
                             Callback &callback) const {
  // change permissions
  try {
    permissions.apply(path);
  } catch (boost::filesystem::filesystem_error const &e) {
    // ignore filesystem errors for now, as this runs in a daemon in background
    (void)e;
  }

  // change owner/group
  int ret = lchown(path.string().c_str(), user.getUid(), group.getGid());
  // ignore error for now, as this runs in a daemon in background
  (void)ret;

  // call callback and return whether to continue
  return callback.callback();
}
