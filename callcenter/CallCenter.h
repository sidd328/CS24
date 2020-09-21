#ifndef CALLCENTER_H
#define CALLCENTER_H

#include "Objects.h"

#include <map>
#include <vector>

// This file describes the CallCenter interface you'll need to implement.
// Don't edit this file!  Your code goes in the MyCallCenter.* files.


class CallCenter {
public:
  // This function gets called at the beginning of the day to set
  // up your call center.  Employee N will be in vector index N.
  static CallCenter* create(std::vector<Employee> employees);

public:
  // The framework will call this function at the beginning of every minute to
  // tell you the IDs of any incoming calls.  You won't get any more information
  // about the calls until you answer them.  Return a vector of the IDs of the calls
  // you want your employees to handle this minute (see the readme for details).
  virtual std::vector<int> calls(int minute, const std::vector<int>& call_ids) = 0;

  // The framework will call this function at the end of every minute to tell you
  // the detailed information about any calls you answered during the minute.
  virtual void learn(int minute, const std::vector<Call>& calls) = 0;

  // Don't forget!
  virtual ~CallCenter() {}
};

#endif
