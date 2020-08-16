#include "Config.h"
#include "StringUtils.h"

#include "Tree.h"

#include <boost/filesystem.hpp>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

void Config::parseFile(std::string const &configFileName) {
  std::ifstream configFile(configFileName, std::ios::in);
  if (! configFile.is_open()) {
    std::stringstream msg;
    msg << "cannot open file \"" << configFileName << "\" for reading";
    throw std::runtime_error(msg.str());
  }

  for (std::string line; std::getline(configFile, line); ) {
    std::string::size_type pos = 0;
    std::string typeStr;
    try {
      StringUtils::getNextField(line, pos, typeStr, "config line type");
    } catch (std::exception const &e) {
      continue; // empty line -> ignore
    }
    if (typeStr == "#") {
      continue; // comment line -> ignore
    }
    // actual consfiguration lines
    if (typeStr == "tree") {
      Tree tree;
      tree.parseParams(line.substr(pos));
      continue;
    }
    // unknown configuration line
    std::stringstream msg;
    msg << "unknown configuration type \"" << typeStr << "\"";
    throw std::runtime_error(msg.str());
  }
}
