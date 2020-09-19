/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/Nice.h>

#include <permissioner/StringUtils.h>

#include <boost/optional.hpp>
#include <string>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/syscall.h>

#define IOPRIO_CLASS_SHIFT (13)
#define IOPRIO_PRIO_VALUE(class_, data) (((class_) << IOPRIO_CLASS_SHIFT) | data)
enum {
  IOPRIO_WHO_PROCESS = 1,
  IOPRIO_WHO_PGRP,
  IOPRIO_WHO_USER,
};
enum {
  IOPRIO_CLASS_NONE,
  IOPRIO_CLASS_RT,
  IOPRIO_CLASS_BE,
  IOPRIO_CLASS_IDLE,
};

Nice::Nice() : ioIdle(false) {}

void Nice::parseParams(std::string const &paramStr) {
  // format of paramStr is: <nice prm 1> [" " <nice prm 2>]
  // <nice prm> may be <nice value: int> or "idle"
  std::string niceStr;

  std::string::size_type pos = 0;
  while (pos < paramStr.length()) {
    StringUtils::getNextField(paramStr, pos, niceStr, "nice prm");
    if (niceStr == "idle") {
      ioIdle = true;
      continue;
    }
    nice = StringUtils::str2intRange(niceStr, -20, 19, "nice value");
  }
}

boost::optional<int> Nice::getNice() const { return nice; }

bool Nice::getIoIdle() const { return ioIdle; }

void Nice::apply() const {
  // set CPU priority
  if (nice.is_initialized()) {
    setpriority(PRIO_PROCESS, 0, nice.get());
  }

  // set idle I/O priority
  if (ioIdle) {
    syscall(SYS_ioprio_set, IOPRIO_WHO_PROCESS, 0,
            IOPRIO_PRIO_VALUE(IOPRIO_CLASS_IDLE, 0));
  }
}
