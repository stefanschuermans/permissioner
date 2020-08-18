#ifndef USER_H
#define USER_H

#include <boost/optional.hpp>
#include <string>

/// user name and ID configuration
class User {
public:
  /**
   * @brief parse user name
   * @param[in] userNameStr user name
   * @throws std::exception if something goes wrong
   */
  void parseUserName(std::string const &userNameStr);

  /// return user name
  boost::optional<std::string> const & getUserName() const;

  /// return user ID (or -1 if none)
  int getUid() const;

protected:
  boost::optional<std::string> userName;
  int uid = -1;
};

#endif // #ifndef USER_H
