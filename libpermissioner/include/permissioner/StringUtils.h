/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

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

  /**
   * @brief convert a string to a long integer
   * @param[in] str string on which to operate
   * @param[in] name filed name for exception
   * @return integer value
   * @throws std::exception if something goes wrong
   */
  static long str2long(std::string const &str, std::string const &name);

  /**
   * @brief convert a string to an integer and check range
   * @param[in] str string on which to operate
   * @param[in] minVal minium value
   * @param[in] maxVal maxium value
   * @param[in] name filed name for exception
   * @return integer value
   * @throws std::exception if something goes wrong
   */
  static int str2intRange(std::string const &str, int minVal, int maxVal,
                          std::string const &name);
};

#endif // #ifndef STRING_UTILS_H
