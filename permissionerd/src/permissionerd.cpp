/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/Callback.h>
#include <permissioner/Config.h>

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

class DaemonCallback : public Callback {
public:
  DaemonCallback() : go_on(true) {}
  bool callback();
  std::chrono::duration<float, std::ratio<1>> sleepTime;
  template <class Rep, class Period>
  bool iterativeSleep(std::chrono::duration<Rep, Period> duration) const;
  bool go_on;
};

bool DaemonCallback::callback() {
  iterativeSleep(sleepTime);
  return go_on;
}

// iterative sleep, watching go_on, returns whether sleep completed
template <class Rep, class Period>
bool DaemonCallback::iterativeSleep(
    std::chrono::duration<Rep, Period> duration) const {
  const std::chrono::duration<int, std::milli> zero(0), step(100);
  while (duration > zero && go_on) {
    if (duration >= step) {
      std::this_thread::sleep_for(step);
      duration -= step;
    } else {
      std::this_thread::sleep_for(duration);
      duration = zero;
    }
  }
  return duration <= zero;
}

DaemonCallback daemonCallback;

void sighandler(int) { daemonCallback.go_on = false; }

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

  // get timing from config
  std::chrono::duration<float, std::ratio<1>> sleepTime(
      config.getSleepTime().get());
  float waitFactor = config.getWaitFactor().get();
  std::chrono::duration<float, std::ratio<1>> waitTime(
      config.getWaitTime().get());

  // catch signals to exit properly on Ctrl-C and so on
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  signal(SIGQUIT, sighandler);
  signal(SIGTERM, sighandler);

  std::cout << "permissionerd (" << configFileName << ") starting" << std::endl;

  // set nicecess of process
  config.getNice().apply();

  // set sleep time after each file in daemon callback
  daemonCallback.sleepTime = sleepTime;

  // continuously set ownership and permissions
  int ret = EXIT_SUCCESS;
  while (daemonCallback.go_on) {

    // set ownership and permissions, measure time it takes
    std::cout << "permissionerd (" << configFileName
              << "): setting ownership and permissions" << std::endl;
    auto begin = std::chrono::steady_clock::now();
    try {
      if (!config.setPermissions(daemonCallback)) {
        break;
      }
    } catch (std::exception const &e) {
      std::cerr << "error: " << e.what() << std::endl;
      ret = EXIT_FAILURE;
      break;
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<float, std::ratio<1>> duration = end - begin;
    std::cout << "permissionerd (" << configFileName << "): took "
              << duration.count() << " s" << std::endl;

    // wait after tree traversal
    if (!daemonCallback.iterativeSleep(waitFactor * duration + waitTime)) {
      break;
    }

  } // while (daemonCallback.go_on)

  std::cout << "permissionerd (" << configFileName << ") shutting down"
            << std::endl;

  return ret;
}
