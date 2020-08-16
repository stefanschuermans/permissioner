#ifndef CONFIG_H
#define CONFIG_H

#include "Tree.h"

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

protected:
  std::map<boost::filesystem::path, Tree> trees;
};

#endif // #ifndef CONFIG_H
