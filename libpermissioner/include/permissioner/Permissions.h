/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <boost/filesystem.hpp>
#include <cstdint>
#include <string>

/// permissions configuration
class Permissions {
public:
  typedef uint64_t Flags;

  static const Flags flagRead = 01;
  static const Flags flagWrite = 02;
  static const Flags flagExecute = 04;
  static const Flags flagOther = 01;
  static const Flags flagGroup = 010;
  static const Flags flagUser = 0100;

  /**
   * @brief parse permissions parameters
   * @param[in] parmStr parameter string
   * @throws std::exception if something goes wrong
   */
  void parseParams(std::string const &paramStr);

  /**
   * @brief apply paermissions to file or directory
   * @param[in] path path to file or directory
   */
  void apply(boost::filesystem::path const &path) const;

  /// return permissions to set
  Flags getSet() const;

  /// return permissions to set conditionally on usrexec/dir
  Flags getSetCond() const;

  /// return permissions to clear
  Flags getClear() const;

  /// return permissions to clear conditionally on usrexec/dir
  Flags getClearCond() const;

protected:
  /// convert flags into boost filesystem permissions
  static boost::filesystem::perms flags2perms(Flags flags);

protected:
  Flags set = 0;       ///< permissions to set
  Flags setCond = 0;   ///< permissions to set conditionally on usrexec/dir
  Flags clear = 0;     ///< permissions to clear
  Flags clearCond = 0; ///< permissions to clear conditionally on usrexec/dir
};

#endif // #ifndef PERMISSIONS_H
