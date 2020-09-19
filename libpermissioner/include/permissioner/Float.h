/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#ifndef FLOAT_H
#define FLOAT_H

#include <string>

/// a floating point number in the settings
class Float {
public:
  /**
   * @brief initialize floating-point setting
   * @param[in] name name of value for error messages
   * @param[in] defVal default value
   * @param[in] minVal minimum value
   * @param[in] maxVal maximum value
   */
  Float(std::string const &name, float minVal, float maxVal, float defVal);

  /**
   * @brief parse floating-point value parameter
   * @param[in] parmStr parameter string
   * @throws std::exception if something goes wrong
   */
  void parseParams(std::string const &paramStr);

  /// return floating-point value
  float get() const;

protected:
  std::string name; ///< name for error messages
  float minVal;     ///< minimum value
  float maxVal;     ///< maximum value
  float val;        ///< configured value
};

#endif // #ifndef FLOAT_H
