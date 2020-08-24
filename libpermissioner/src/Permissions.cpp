#include <permissioner/Permissions.h>

#include <boost/filesystem.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

void Permissions::parseParams(std::string const &paramStr) {
  // reset permission flags
  set = 0;
  setCond = 0;
  clear = 0;
  clearCond = 0;
  // parse comma-separated groups of [ugoa][+-][rwxX] settings
  Flags who = 0, what = 0, whatCond = 0;
  char action = ' ';
  // append comma at end to process last group
  for (char c : paramStr + ",") {
    switch (c) {
    case ',':
      switch (action) {
        case '=':
          set = who * what;
          setCond = who * whatCond;
          clear = who * (what ^ (flagRead | flagWrite | flagExecute));
          clearCond = 0;
          break;
        case '+':
          set = who * what;
          setCond = who * whatCond;
          break;
        case '-':
          clear = who * what;
          clearCond = who * whatCond;
          break;
      }
      who = 0;
      what = 0;
      whatCond = 0;
      action = ' ';
      break;
    case 'u':
      who |= flagUser;
      break;
    case 'g':
      who |= flagGroup;
      break;
    case 'o':
      who |= flagOther;
      break;
    case 'a':
      who |= flagUser | flagGroup | flagOther;
      break;
    case '=':
    case '+':
    case '-':
      action = c;
      break;
    case 'r':
      what |= flagRead;
      break;
    case 'w':
      what |= flagWrite;
      break;
    case 'x':
      what |= flagExecute;
      break;
    case 'X':
      whatCond |= flagExecute;
      break;
    default:
      {
        std::stringstream msg;
        msg << "invalid character \"" << c << "\" in permission string \""
            << paramStr << "\"";
        throw std::runtime_error(msg.str());
      }
    }
  }
}

void Permissions::apply(boost::filesystem::path const &path) const {
  // only process regular files and directories (especially no symlinks)
  if (! boost::filesystem::is_regular_file(path) &&
      ! boost::filesystem::is_directory(path)) {
    return;
  }

  // get permissions
  boost::filesystem::file_status st = boost::filesystem::status(path);
  boost::filesystem::perms perms = st.permissions();

  // compute updated permissions
  Flags doSet = set, doClear = clear;
  if (perms & boost::filesystem::perms::owner_exe) {
    doSet |= setCond;
    doClear |= clearCond;
  }
  perms &= boost::filesystem::perms::all_all ^ flags2perms(doClear);
  perms |= flags2perms(doSet);

  // set new permissions if they changed
  if (perms != st.permissions()) {
    boost::filesystem::permissions(path, perms);
  }
}

Permissions::Flags Permissions::getSet() const {
  return set;
}

Permissions::Flags Permissions::getSetCond() const {
  return setCond;
}

Permissions::Flags Permissions::getClear() const {
  return clear;
}

Permissions::Flags Permissions::getClearCond() const {
  return clearCond;
}

boost::filesystem::perms Permissions::flags2perms(Flags flags) {
  using fsp = boost::filesystem::perms;
  fsp perms = fsp::no_perms;
  if (flags & (flagUser * flagRead)) { perms |= fsp::owner_read; }
  if (flags & (flagUser * flagWrite)) { perms |= fsp::owner_write; }
  if (flags & (flagUser * flagExecute)) { perms |= fsp::owner_exe; }
  if (flags & (flagGroup * flagRead)) { perms |= fsp::group_read; }
  if (flags & (flagGroup * flagWrite)) { perms |= fsp::group_write; }
  if (flags & (flagGroup * flagExecute)) { perms |= fsp::group_exe; }
  if (flags & (flagOther * flagRead)) { perms |= fsp::others_read; }
  if (flags & (flagOther * flagWrite)) { perms |= fsp::others_write; }
  if (flags & (flagOther * flagExecute)) { perms |= fsp::others_exe; }
  return perms;
}
