#ifndef TREE_H
#define TREE_H

#include <permissioner/Permissions.h>

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
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

protected:
  boost::optional<std::string> user;
  boost::optional<std::string> group;
  Permissions permissions;
  boost::filesystem::path root;
};

#endif // #ifndef TREE_H
