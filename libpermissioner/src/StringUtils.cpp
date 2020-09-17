/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#include <permissioner/StringUtils.h>

#include <stdexcept>
#include <sstream>
#include <string>

void StringUtils::getNextField(std::string const &str,
                               std::string::size_type &pos, std::string &field,
                               std::string const &name) {
  static const std::string whitespace(" \t");

  std::string::size_type begin = str.find_first_not_of(whitespace, pos);
  if (begin == std::string::npos) {
    std::stringstream msg;
    msg << "<" << name <<"> field missing in \"" << str << "\"";
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
