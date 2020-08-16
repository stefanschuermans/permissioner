#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>

class StringUtils {
public:
  /**
   * @brief get next whitespace separated field in string
   * @param[in] str string on which to operate
   * @param[in, out] pos current position, updated to begin of next field
   * @param[out] field value of field extracted from string
   * @param[in] name filed name for exception
   * @throws std::exception if something goes wrong
   */
  static void getNextField(std::string const &str, std::string::size_type &pos,
                           std::string &field, std::string const &name);
};

#endif // #ifndef STRING_UTILS_H
