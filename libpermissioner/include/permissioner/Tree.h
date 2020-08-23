#ifndef TREE_H
#define TREE_H

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
  User const & getUser() const;

  /// get group
  Group const & getGroup() const;

  /// get permissions
  Permissions const & getPermissions() const;

  /// get root path
  boost::filesystem::path const & getRoot() const;

  /**
   * @brief set owners and permissions of files in tree
   * @param[in] exclude map of other trees that shall be excluded
   */
  void setPermissions(TreeMap const &exclude) const;

protected:
  /**
   * @brief set owners and permissions of files in path and subtrees
   * @param[in] path path to top of directory tree for which to set owner/perms
   * @param[in] exclude map of other trees that shall be excluded
   */
  void setPermissionsInternal(boost::filesystem::path const &path,
                              TreeMap const &exclude) const;

  /**
   * @brief set owners and permissions of one file or directory
   * @param[in] path path to file or directory
   */
  void setPermissionsOne(boost::filesystem::path const &path) const;

protected:
  User user;
  Group group;
  Permissions permissions;
  boost::filesystem::path root;
};

#endif // #ifndef TREE_H
