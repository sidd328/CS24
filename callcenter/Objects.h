#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

// This file contains the objects the autograder framework uses
// to represent calls and employees.  If these don't contain all
// the information you need to track, create a new class or a
// subclass to hold your versions.  Don't edit this file!


struct Call {
  int id;              // The unique ID of this call (always 1 or greater).
  int recieved;        // The minute this call was first recieved.
  int importance;      // The importance of this call (always 1 or greater).
  int difficulty;      // The difficulty of this call (always 1 or greater).
  int work_required;   // The total number of minutes needed to complete this call.
  int work_performed;  // The number of minutes of work that have been performed.
};

struct Employee {
  int         id;     // The unique ID of this employee (always 0 or greater).
  std::string name;   // The name of this employee (used for logging).
  int         skill;  // The highest difficulty call this employee can work on.
  Call*       call;   // The call this employee is currently working on (if any).
};

#endif
