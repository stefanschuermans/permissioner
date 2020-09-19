/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/Config.h>

#include <permissioner/Callback.h>
#include <permissioner/StringUtils.h>
#include <permissioner/Tree.h>

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
    // actual configuration lines
    if (typeStr == "nice") {
      nice.parseParams(line.substr(pos));
      continue;
    }
    if (typeStr == "tree") {
      Tree tree;
      tree.parseParams(line.substr(pos));
      trees[tree.getRoot()] = tree;
      continue;
    }
    // unknown configuration line
    std::stringstream msg;
    msg << "unknown configuration type \"" << typeStr << "\"";
    throw std::runtime_error(msg.str());
  }
}

Nice const & Config::getNice() const {
  return nice;
}

TreeMap const & Config::getTrees() const {
  return trees;
}

bool Config::setPermissions(Callback &callback) const {
  for (auto const & path_tree : trees) {
    Tree const & tree = path_tree.second;
    if (! tree.setPermissions(trees, callback)) { // exclude all other trees
      return false;
    }
  }
  return true;
}
