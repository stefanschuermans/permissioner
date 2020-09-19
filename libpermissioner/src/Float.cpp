/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/Float.h>

#include <permissioner/StringUtils.h>

#include <sstream>
#include <stdexcept>
#include <string>

Float::Float(std::string const &name, float minVal, float maxVal, float defVal)
    : name(name), minVal(minVal), maxVal(maxVal), val(defVal) {}

void Float::parseParams(std::string const &paramStr) {
  // format of paramStr is: <floating-point-value>
  std::string valStr;
  std::string::size_type pos = 0;
  StringUtils::getNextField(paramStr, pos, valStr, name);
  if (pos < paramStr.length()) {
    std::stringstream msg;
    msg << "too many fields for \"" << name << "\" in \"" << paramStr << "\"";
    throw std::runtime_error(msg.str());
  }
  val = StringUtils::str2floatRange(valStr, minVal, maxVal, name);
}

float Float::get() const { return val; }
