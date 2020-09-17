/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#ifndef GROUP_H
#define GROUP_H

#include <boost/optional.hpp>
#include <string>

/// group name and ID configuration
class Group {
public:
  /**
   * @brief parse group name
   * @param[in] groupNameStr group name
   * @throws std::exception if something goes wrong
   */
  void parseGroupName(std::string const &groupNameStr);

  /// return group name
  boost::optional<std::string> const & getGroupName() const;

  /// return group ID (or -1 if none)
  int getGid() const;

protected:
  boost::optional<std::string> groupName;
  int gid = -1;
};

#endif // #ifndef GROUP_H
