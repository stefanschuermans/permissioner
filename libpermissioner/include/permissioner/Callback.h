/**
 * Permissioner: set file ownerships and permissions
 * Copyright 2020: Stefan Schuermans, Aachen, Germany <stefan@schuermans.info>
 * Copyleft: GNU GENERAL PUBLIC LICENSE version 3 (see LICENSE)
 */

#ifndef CALLBACK_H
#define CALLBACK_H

/// callback interface, called after each file
class Callback {
public:
  virtual ~Callback();

  /**
   * @brief callback, called after each file
   * @return whether to continue (false means abort)
   *
   * This default implementation does nothing and returns true
   */
  virtual bool callback();
};

#endif // #ifndef CALLBACK_H
