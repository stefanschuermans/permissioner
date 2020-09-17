/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/Config.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char const **argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <config file>" << std::endl;
    return EXIT_FAILURE;
  }
  std::string configFileName(argv[1]);

  try {
    Config config;
    config.parseFile(configFileName);
    config.setPermissions();
  } catch (std::exception const &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
