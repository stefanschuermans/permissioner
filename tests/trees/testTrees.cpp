#include <permissioner/Config.h>

#include <boost/filesystem.hpp>
#include <cstdlib>
#include <grp.h>
#include <iomanip>
#include <iostream>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

struct LogLchown {
  std::string pathname;
  uid_t owner;
  gid_t group;
};
std::vector<LogLchown> logs_lchown;

// mock version of lchown, to see if right files get right owners
extern "C" int lchown(const char *pathname, uid_t owner, gid_t group) {
  logs_lchown.emplace_back(LogLchown { pathname, owner, group });
  return 0;
}

bool check_lchown(unsigned int idx, std::string const &path,
                  uid_t owner, gid_t group) {
  bool ret = true;
  if (idx > logs_lchown.size()) {
    std::cerr << "no such lchown call #" << idx << std::endl;
    return false;
  }
  LogLchown const & log_lchown = logs_lchown.at(idx);
  if (log_lchown.pathname != path) {
    std::cerr << "lchown call #" << idx << ": unexpected path \""
              << log_lchown.pathname << "\" != ...\"" << path << "\""
              << std::endl;
    ret = false;
  }
  if (log_lchown.owner != owner) {
    std::cerr << "lchown call #" << idx << ": unexpected owner "
              << log_lchown.owner << " != " << owner << std::endl;
    ret = false;
  }
  if (log_lchown.group != group) {
    std::cerr << "lchown call #" << idx << ": unexpected group "
              << log_lchown.group << " != " << group << std::endl;
    ret = false;
  }
  return ret;
}

struct LogChmod {
  std::string pathname;
  mode_t mode;
};
std::vector<LogChmod> logs_chmod;

// mock version of chmod, to see if right files get right permissions
extern "C" int chmod(const char *pathname, mode_t mode) {
  logs_chmod.emplace_back(LogChmod { pathname, mode });
  return 0;
}

bool check_chmod(unsigned int idx, std::string const &path, mode_t mode) {
  bool ret = true;
  if (idx > logs_chmod.size()) {
    std::cerr << "no such chmod call #" << idx << std::endl;
    return false;
  }
  LogChmod const & log_chmod = logs_chmod.at(idx);
  if (log_chmod.pathname != path) {
    std::cerr << "chmod call #" << idx << ": unexpected path \""
              << log_chmod.pathname << "\" != ...\"" << path << "\""
              << std::endl;
    ret = false;
  }
  if (log_chmod.mode != mode) {
    std::cerr << "chmod call #" << idx << ": unexpected mode "
              << std::oct << log_chmod.mode << " != " << mode
              << std::dec << std::endl;
    ret = false;
  }
  return ret;
}

bool check(unsigned int idx, std::string const &rel_path,
           uid_t owner, gid_t group, mode_t mode) {
  bool ret = true;
  std::string path = boost::filesystem::canonical(rel_path).string();
  if (! check_lchown(idx, path, owner, group)) {
    ret = false;
  }
  if (! check_chmod(idx, path, mode)) {
    ret = false;
  }
  return ret;
}

int main(int argc, char const **argv) {
  (void)argc;
  Config config;
  config.parseFile(argv[1]);
  config.setPermissions();

  int ret = EXIT_SUCCESS;

  uid_t nobody = getpwnam("nobody")->pw_uid;
  gid_t nogroup = getgrnam("nogroup")->gr_gid;

  if (! check(0, "work", nobody, nogroup, 0775)) {
    ret = EXIT_FAILURE;
  }
  if (! check(1, "work/file", nobody, nogroup, 0664)) {
    ret = EXIT_FAILURE;
  }
  if (! check(2, "work/nested", -1, -1, 0757)) {
    ret = EXIT_FAILURE;
  }
  if (! check(3, "work/nested/other", -1, -1, 0646)) {
    ret = EXIT_FAILURE;
  }
  unsigned int size = 4;
  if (logs_lchown.size() != size) {
    std::cerr << "unexpected size of logs_lchown: " << logs_lchown.size()
              << " != " << size << std::endl;
    ret = EXIT_FAILURE;
  }
  if (logs_chmod.size() != size) {
    std::cerr << "unexpected size of logs_chmod: " << logs_chmod.size()
              << " != " << size << std::endl;
    ret = EXIT_FAILURE;
  }

  return ret;
}
