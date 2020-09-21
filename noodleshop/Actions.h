#ifndef ACTIONS_H
#define ACTIONS_H

#include "Objects.h"
class Validator;

#include <string>
#include <vector>

// This file describes the actions you can choose
// to perform each minute.  Don't edit this file!


struct Action {
  // This function is used by the grader to make sure that
  // the action is legal.  It uses the visitor pattern to
  // dispatch the correct subclass's implementation. The
  // implementations are at the top of Validator.cpp.
  virtual void validate(Validator*) const = 0;

  virtual ~Action() {}
};

struct CookAction: public Action {
  std::string  noodle;
  unsigned int pot_id;

  void validate(Validator*) const;

  // Return this action to start cooking a batch of noodles.
  // Pot IDs are integers 0 <= id < npots.
  CookAction(unsigned int pot_id, const std::string& noodle) {
    this->noodle = noodle;
    this->pot_id = pot_id;
  }
};

struct CleanAction: public Action {
  unsigned int pot_id;

  void validate(Validator*) const;

  // Return this action to clean a pot.
  // Pot IDs are integers 0 <= id < npots.
  CleanAction(unsigned int pot_id) {
    this->pot_id = pot_id;
  }
};

struct NoAction: public Action {
  void validate(Validator*) const;
};

struct ServeAction: public Action {
  std::vector<Serve> serves;

  void validate(Validator*) const;

  // Return this action to serve any number of customers.
  ServeAction(const std::vector<Serve>& serves) {
    this->serves = serves;
  }
};

#endif
