#include <permissioner/Config.h>
#include <permissioner/Group.h>
#include <permissioner/Permissions.h>
#include <permissioner/User.h>
#include <permissioner/Tree.h>

#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

bool check(TreeMap const &treeMap, std::string const &rel_path,
           boost::optional<std::string> user,
           boost::optional<std::string> group,
           mode_t setMode, mode_t setCondMode,
           mode_t clearMode, mode_t clearCondMode) {
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
    std::cerr << "tree map entry \"" << path
              << "\": unexpected set mode "
              << std::oct << perms.getSet() << " != " << setMode
              << std::dec << std::endl;
    ret = false;
  }
  if (perms.getSetCond() != setCondMode) {
    std::cerr << "tree map entry \"" << path
              << "\": unexpected set cond mode "
              << std::oct << perms.getSetCond() << " != " << setCondMode
              << std::dec << std::endl;
    ret = false;
  }
  if (perms.getClear() != clearMode) {
    std::cerr << "tree map entry \"" << path
              << "\": unexpected clear mode "
              << std::oct << perms.getClear() << " != " << clearMode
              << std::dec << std::endl;
    ret = false;
  }
  if (perms.getClearCond() != clearCondMode) {
    std::cerr << "tree map entry \"" << path
              << "\": unexpected clear cond mode "
              << std::oct << perms.getClearCond() << " != " << clearCondMode
              << std::dec << std::endl;
    ret = false;
  }

  return ret;
}

int main(int argc, char const **argv) {
  (void)argc;
  Config config;
  config.parseFile(argv[1]);

  int ret = EXIT_SUCCESS;

  TreeMap const &treeMap = config.getTrees();
  if (treeMap.size() != 2) {
    std::cerr << "unexpected number of trees: " << treeMap.size() << std::endl;
    ret = EXIT_FAILURE;
  }
  if (! check(treeMap, "some/dir", std::string("nobody"),
                                   std::string("nogroup"),
                                   0011, 0044, 0022, 000)) {
    ret = EXIT_FAILURE;
  }
  if (! check(treeMap, "some/other/dir", boost::none,
                                         boost::none,
                                         0777, 0000, 0000, 0000)) {
    ret = EXIT_FAILURE;
  }

  return ret;
}
