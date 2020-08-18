#include <permissioner/Permissions.h>

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
          clearCond = who * (what ^ flagExecute);
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
    }
  }
}
