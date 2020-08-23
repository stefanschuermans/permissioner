#ifndef CONFIG_H
#define CONFIG_H

#include <permissioner/Tree.h>

#include <boost/filesystem.hpp>
#include <map>
#include <string>

/// configuration file
class Config {
public:
  /**
   * @brief parse configuration file
   * @param[in] configFileName name of configuation file
   * @throws std::exception if something goes wrong
   */
  void parseFile(std::string const &configFileName);

  /// return trees
  TreeMap const & getTrees() const;

  /**
   * @brief set owners and permissions of files in trees
   */
  void setPermissions() const;

protected:
  TreeMap trees;
};

#endif // #ifndef CONFIG_H
