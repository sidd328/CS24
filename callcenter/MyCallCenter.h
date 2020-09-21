#ifndef MYCALLCENTER_H
#define MYCALLCENTER_H

#include "CallCenter.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
struct MyCalls;
struct MyEmployee;

struct MyCalls
{
  int id;             // The unique ID of this call (always 1 or greater).
  int recieved;       // The minute this call was first recieved.
  int importance;     // The importance of this call (always 1 or greater).
  int difficulty;     // The difficulty of this call (always 1 or greater).
  int work_required;  // The total number of minutes needed to complete this call.
  int work_performed; // The number of minutes of work that have been performed.

  MyEmployee *attendee;

  bool status;

  MyCalls()
  {
    id = 10000000;
    recieved = -1;
    importance = -1;
    difficulty = 0;
    work_required = 1000000;
    work_performed = -1;
    attendee = nullptr;
    status = false;
  }

  ~MyCalls()
  {
    attendee = nullptr;
  }
};

struct MyEmployee
{
  int id;           // The unique ID of this employee (always 0 or greater).
  std::string name; // The name of this employee (used for logging).
  int skill;        // The highest difficulty call this employee can work on.

  MyCalls *mycall;

  MyEmployee(Employee emp)
  {
    id = emp.id;
    name = emp.name;
    skill = emp.skill;
    mycall = nullptr;
  }

  ~MyEmployee()
  {
    delete mycall;
    mycall = nullptr;
  }
};

class MyCallCenter : public CallCenter
{
  int numBuf;

  int numTop;

  std::vector<int> previous_records; //Hold a record of who the employees were talking to the last minute (the previous result vector)

  std::vector<MyEmployee *> mEmps; //Vector of all employees sorted by skill level

  std::vector<MyCalls *> on_hold; //calls on hold (info is gathered)

  std::vector<int> unanswered; //calls never answered (no info is known)

public:
  MyCallCenter(std::vector<Employee> employee);
  ~MyCallCenter();

  // Required Member Functions
  std::vector<int> calls(int minute, const std::vector<int> &call_ids);
  void learn(int minute, const std::vector<Call> &calls);

  void try_to_answer(int start);

  void clear_on_hold();
};

#endif