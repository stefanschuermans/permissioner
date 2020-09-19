/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/Config.h>
#include <permissioner/Group.h>
#include <permissioner/Permissions.h>
#include <permissioner/Tree.h>
#include <permissioner/User.h>

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

int testEmpty() {
  Config config;
  config.parseFile("empty.cfg");

  int ret = EXIT_SUCCESS;

  Nice const &nice = config.getNice();
  if (nice.getNice().is_initialized()) {
    std::cerr << "unexpected nice value: " << nice.getNice().get() << std::endl;
    ret = EXIT_FAILURE;
  }
  if (nice.getIoIdle()) {
    std::cerr << "unexpected I/O idle" << std::endl;
    ret = EXIT_FAILURE;
  }

  TreeMap const &treeMap = config.getTrees();
  if (treeMap.size() != 0) {
    std::cerr << "unexpected trees: " << treeMap.size() << std::endl;
    ret = EXIT_FAILURE;
  }

  return ret;
}

int testNice() {
  int ret = EXIT_SUCCESS;

  {
    Config config;
    config.parseFile("nice.cfg");

    Nice const &nice = config.getNice();
    if (!nice.getNice().is_initialized()) {
      std::cerr << "expected nice value, but got none" << std::endl;
      ret = EXIT_FAILURE;
    } else {
      if (nice.getNice().get() != 19) {
        std::cerr << "unexpected nice value: " << nice.getNice().get()
                  << ", expected 19" << std::endl;
        ret = EXIT_FAILURE;
      }
    }
    if (nice.getIoIdle()) {
      std::cerr << "unexpected I/O idle" << std::endl;
      ret = EXIT_FAILURE;
    }
  }

  {
    Config config;
    config.parseFile("idle.cfg");

    Nice const &nice = config.getNice();
    if (nice.getNice().is_initialized()) {
      std::cerr << "unexpected nice value: " << nice.getNice().get()
                << std::endl;
      ret = EXIT_FAILURE;
    }
    if (!nice.getIoIdle()) {
      std::cerr << "expected I/O idle, but did not get it" << std::endl;
      ret = EXIT_FAILURE;
    }
  }

  {
    Config config;
    config.parseFile("nice_idle.cfg");

    Nice const &nice = config.getNice();
    if (!nice.getNice().is_initialized()) {
      std::cerr << "expected nice value, but got none" << std::endl;
      ret = EXIT_FAILURE;
    } else {
      if (nice.getNice().get() != 19) {
        std::cerr << "unexpected nice value: " << nice.getNice().get()
                  << ", expected 19" << std::endl;
        ret = EXIT_FAILURE;
      }
    }
  }

  return ret;
}

bool checkTrees(TreeMap const &treeMap, std::string const &rel_path,
                boost::optional<std::string> user,
                boost::optional<std::string> group, mode_t setMode,
                mode_t setCondMode, mode_t clearMode, mode_t clearCondMode) {
  std::string path = boost::filesystem::canonical(rel_path).string();
  TreeMap::const_iterator itTree = treeMap.find(path);
  if (itTree == treeMap.end()) {
    std::cerr << "tree map entry \"" << path << "\" not found" << std::endl;
    return false;
  }
  Tree const &tree = itTree->second;

  bool ret = true;

  if (tree.getRoot() != path) {
    std::cerr << "tree map entry \"" << path << "\": unexpected root \""
              << tree.getRoot() << "\"" << std::endl;
    ret = false;
  }
  if (tree.getUser().getUserName() != user) {
    std::cerr << "tree map entry \"" << path << "\": unexpected user \""
              << tree.getUser().getUserName() << "\"" << std::endl;
    ret = false;
  }
  if (tree.getGroup().getGroupName() != group) {
    std::cerr << "tree map entry \"" << path << "\": unexpected group \""
              << tree.getGroup().getGroupName() << "\"" << std::endl;
    ret = false;
  }
  Permissions const &perms = tree.getPermissions();
  if (perms.getSet() != setMode) {
    std::cerr << "tree map entry \"" << path << "\": unexpected set mode "
              << std::oct << perms.getSet() << " != " << setMode << std::dec
              << std::endl;
    ret = false;
  }
  if (perms.getSetCond() != setCondMode) {
    std::cerr << "tree map entry \"" << path << "\": unexpected set cond mode "
              << std::oct << perms.getSetCond() << " != " << setCondMode
              << std::dec << std::endl;
    ret = false;
  }
  if (perms.getClear() != clearMode) {
    std::cerr << "tree map entry \"" << path << "\": unexpected clear mode "
              << std::oct << perms.getClear() << " != " << clearMode << std::dec
              << std::endl;
    ret = false;
  }
  if (perms.getClearCond() != clearCondMode) {
    std::cerr << "tree map entry \"" << path
              << "\": unexpected clear cond mode " << std::oct
              << perms.getClearCond() << " != " << clearCondMode << std::dec
              << std::endl;
    ret = false;
  }

  return ret;
}

int testTrees() {
  Config config;
  config.parseFile("trees.cfg");

  int ret = EXIT_SUCCESS;

  TreeMap const &treeMap = config.getTrees();
  if (treeMap.size() != 2) {
    std::cerr << "unexpected number of trees: " << treeMap.size() << std::endl;
    ret = EXIT_FAILURE;
  }
  if (!checkTrees(treeMap, "some/dir", std::string("nobody"),
                  std::string("nogroup"), 0011, 0044, 0022, 000)) {
    ret = EXIT_FAILURE;
  }
  if (!checkTrees(treeMap, "some/other/dir", boost::none, boost::none, 0777,
                  0000, 0000, 0000)) {
    ret = EXIT_FAILURE;
  }

  return ret;
}

void merge_ret(int &ret, int ret2) {
  if (ret == EXIT_SUCCESS && ret2 != EXIT_SUCCESS) {
    ret = ret2;
  }
}

int main() {
  int ret = EXIT_SUCCESS;
  merge_ret(ret, testEmpty());
  merge_ret(ret, testNice());
  merge_ret(ret, testTrees());
  return ret;
}
