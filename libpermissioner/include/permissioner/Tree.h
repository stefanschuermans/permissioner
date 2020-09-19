/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#ifndef TREE_H
#define TREE_H

#include <permissioner/Callback.h>
#include <permissioner/Group.h>
#include <permissioner/Permissions.h>
#include <permissioner/User.h>

#include <boost/filesystem.hpp>
#include <map>
#include <string>

/// map of trees
typedef std::map<boost::filesystem::path, class Tree> TreeMap;

/// directory tree configuration
class Tree {
public:
  /**
   * @brief parse tree parameters
   * @param[in] parmStr parameter string
   * @throws std::exception if something goes wrong
   */
  void parseParams(std::string const &paramStr);

  /// get user
  User const &getUser() const;

  /// get group
  Group const &getGroup() const;

  /// get permissions
  Permissions const &getPermissions() const;

  /// get root path
  boost::filesystem::path const &getRoot() const;

  /**
   * @brief set owners and permissions of files in tree
   * @param[in] exclude map of other trees that shall be excluded
   * @param[in] callback callback object to call after each processed file
   * @return whether traversal was completed (false means aborted)
   */
  bool setPermissions(TreeMap const &exclude, Callback &callback) const;

protected:
  /**
   * @brief set owners and permissions of files in path and subtrees
   * @param[in] path path to top of directory tree for which to set owner/perms
   * @param[in] exclude map of other trees that shall be excluded
   * @param[in] callback callback object to call after each processed file
   * @return whether to continue (false means abort traversal)
   */
  bool setPermissionsInternal(boost::filesystem::path const &path,
                              TreeMap const &exclude,
                              Callback &callback) const;

  /**
   * @brief set owners and permissions of one file or directory
   * @param[in] path path to file or directory
   * @param[in] callback callback object to call after each processed file
   * @return whether to continue (false means abort traversal)
   */
  bool setPermissionsOne(boost::filesystem::path const &path,
                         Callback &callback) const;

protected:
  User user;
  Group group;
  Permissions permissions;
  boost::filesystem::path root;
};

#endif // #ifndef TREE_H
