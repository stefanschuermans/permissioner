#include <permissioner/Config.h>

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

static int go_on = 1;

void sighandler(int) {
  go_on = 0;
}

int main(int argc, char const **argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <config file>" << std::endl;
    return EXIT_FAILURE;
  }
  std::string configFileName(argv[1]);

  // load configuration
  Config config;
  try {
    config.parseFile(configFileName);
  } catch (std::exception const &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  // catch signals to exit properly on Ctrl-C and so on
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  signal(SIGQUIT, sighandler);
  signal(SIGTERM, sighandler);

  std::cout << "permissionerd (" << configFileName << ") starting"
            << std::endl;

  // continuously set ownership and permissions
  int ret = EXIT_SUCCESS;
  while (go_on) {

    // set owneship and permissions, measure time it takes
    std::cout << "permissionerd (" << configFileName
              << "): setting ownership and permissions" << std::endl;
    auto begin = std::chrono::steady_clock::now();
    try {
      config.setPermissions();
    } catch (std::exception const &e) {
      std::cerr << "error: " << e.what() << std::endl;
      ret = EXIT_FAILURE;
      break;
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<float, std::ratio<1>> duration = end - begin;
    std::cout << "permissionerd (" << configFileName << "): took "
              << duration.count() << " s" << std::endl;

    // sleep 10 times as long as the work took plus one second
    auto sleep_time = 10 * duration
                    + std::chrono::duration<int, std::ratio<1>>(1);
    std::this_thread::sleep_for(sleep_time);

  } // while (go_on)

  std::cout << "permissionerd (" << configFileName << ") shutting down"
            << std::endl;

  return ret;
}
