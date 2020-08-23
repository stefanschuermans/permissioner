#ifndef CONFIG_H
#define CONFIG_H

#include <permissioner/Tree.h>

#include <boost/filesystem.hpp>
#include <map>
#include <string>

/// configuration file
class Config {
public:
  /// map of trees
  typedef std::map<boost::filesystem::path, Tree> TreeMap;

  /**
   * @brief parse configuration file
   * @param[in] configFileName name of configuation file
   * @throws std::exception if something goes wrong
   */
  void parseFile(std::string const &configFileName);

  /// return trees
  TreeMap const & getTrees() const;

protected:
  TreeMap trees;
};

#endif // #ifndef CONFIG_H
