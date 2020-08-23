#ifndef TREE_H
#define TREE_H

#include <permissioner/Group.h>
#include <permissioner/Permissions.h>
#include <permissioner/User.h>

#include <boost/filesystem.hpp>
#include <string>

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

protected:
  User user;
  Group group;
  Permissions permissions;
  boost::filesystem::path root;
};

#endif // #ifndef TREE_H
