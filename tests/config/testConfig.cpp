#include "Config.h"

#include <cstdlib>

int main(int argc, char const **argv) {
  (void)argc;
  Config config;
  config.parseFile(argv[1]);
  return EXIT_SUCCESS;
}
