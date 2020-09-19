/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <permissioner/Callback.h>
#include <permissioner/Float.h>
#include <permissioner/Nice.h>
#include <permissioner/Tree.h>

#include <boost/filesystem.hpp>
#include <map>
#include <string>

/// configuration file
class Config {
public:
  Config();

  /**
   * @brief parse configuration file
   * @param[in] configFileName name of configuation file
   * @throws std::exception if something goes wrong
   */
  void parseFile(std::string const &configFileName);

  /// return nice settings
  Nice const & getNice() const;

  /// return sleep time setting
  Float const & getSleepTime() const;

  /// return wait factor setting
  Float const & getWaitFactor() const;

  /// return wait time setting
  Float const & getWaitTime() const;

  /// return trees
  TreeMap const & getTrees() const;

  /**
   * @brief set owners and permissions of files in trees
   * @param[in] callback callback object to call after each processed file
   * @return whether traversal was completed (false means aborted)
   */
  bool setPermissions(Callback &callback) const;

protected:
  Nice nice;
  Float sleepTime;
  Float waitFactor;
  Float waitTime;
  TreeMap trees;
};

#endif // #ifndef CONFIG_H
