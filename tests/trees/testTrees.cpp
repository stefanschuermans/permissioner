#include <permissioner/Config.h>

#include <cstdlib>
#include <iostream> // DEBUG
#include <unistd.h>
#include <sys/stat.h>

// mock version of lchown, to see if right files get right owners
extern "C" int lchown(const char *pathname, uid_t owner, gid_t group) {
  std::cout << "DEBUG lchown " << pathname << " owner " << owner
            << " group " << group << std::endl;
  return 0;
}

// mock version fo chmod, to see if right files get right permissions
extern "C" int chmod(const char *pathname, mode_t mode) {
  std::cout << "DEBUG chmod " << pathname << " mode " << mode << std::endl;
  return 0;
}

int main(int argc, char const **argv) {
  (void)argc;
  Config config;
  config.parseFile(argv[1]);
  config.setPermissions();
  return EXIT_SUCCESS;
}
