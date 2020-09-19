/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#ifndef NICE_H
#define NICE_H

#include <boost/optional.hpp>
#include <string>

/// nice (CPU and I/O) priority settings
class Nice {
public:
  Nice();

  /**
   * @brief parse nice parameters
   * @param[in] parmStr parameter string
   * @throws std::exception if something goes wrong
   */
  void parseParams(std::string const &paramStr);

  /// return niceness level
  boost::optional<int> getNice() const;

  /// return whether using I/O idle class
  bool getIoIdle() const;

  /**
   * @brief set CPU and I/O priority of process
   */
  void apply() const;

protected:
  boost::optional<int> nice;
  bool ioIdle;
};

#endif // #ifndef NICE_H
