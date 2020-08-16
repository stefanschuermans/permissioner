#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <string>

/// permissions configuration
class Permissions {
public:
  /**
   * @brief parse permissions parameters
   * @param[in] parmStr parameter string
   * @throws std::exception if something goes wrong
   */
  void parseParams(std::string const &paramStr);

protected:
  
};

#endif // #ifndef PERMISSIONS_H
