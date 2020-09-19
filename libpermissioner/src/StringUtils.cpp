/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/StringUtils.h>

#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>

void StringUtils::getNextField(std::string const &str,
                               std::string::size_type &pos, std::string &field,
                               std::string const &name) {
  static const std::string whitespace(" \t");

  std::string::size_type begin = str.find_first_not_of(whitespace, pos);
  if (begin == std::string::npos) {
    std::stringstream msg;
    msg << "<" << name << "> field missing in \"" << str << "\"";
    throw std::runtime_error(msg.str());
  }

  std::string::size_type end = str.find_first_of(whitespace, begin);
  if (end == std::string::npos) {
    end = str.length();
  }
  field = str.substr(begin, end - begin);

  pos = str.find_first_not_of(whitespace, end);
  if (pos == std::string::npos) {
    pos = str.length();
  }
}

long StringUtils::str2long(std::string const &str, std::string const &name) {
  char const *c_str = str.c_str();
  char *end;
  long val = strtol(c_str, &end, 0);
  if (end == c_str || *end != 0) {
    std::stringstream msg;
    msg << "invalid integer value \"" << str << "\" for <" << name << "> field";
    throw std::runtime_error(msg.str());
  }
  return val;
}

int StringUtils::str2intRange(std::string const &str, int minVal, int maxVal,
                              std::string const &name) {
  long val = str2long(str, name);
  if (val < minVal || val > maxVal) {
    std::stringstream msg;
    msg << "value " << val << " of <" << name << "> field out of range "
        << minVal << " - " << maxVal;
    throw std::runtime_error(msg.str());
  }
  return val;
}

float StringUtils::str2float(std::string const &str, std::string const &name) {
  char const *c_str = str.c_str();
  char *end;
  float val = strtof(c_str, &end);
  if (end == c_str || *end != 0) {
    std::stringstream msg;
    msg << "invalid floating-point value \"" << str << "\" for <" << name
        << "> field";
    throw std::runtime_error(msg.str());
  }
  return val;
}

float StringUtils::str2floatRange(std::string const &str, float minVal,
                                  float maxVal, std::string const &name) {
  float val = str2float(str, name);
  if (val < minVal || val > maxVal) {
    std::stringstream msg;
    msg << "value " << val << " of <" << name << "> field out of range "
        << minVal << " - " << maxVal;
    throw std::runtime_error(msg.str());
  }
  return val;
}
