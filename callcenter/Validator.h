#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Objects.h"

#include <map>
#include <stdexcept>
#include <vector>

// Thrown when the validator detects an error.
struct validation_error: public std::runtime_error {
  validation_error(const std::string& message): std::runtime_error(message) {}
};


class Validator {
  std::vector<Employee> mEmployees;
  std::map<int, Call*>  mPool;

  int mTotalCalls;
  int mTotalScore;
  int mTotalTime;
  int mVerbosity;
private:
  void log(const std::string& message, int level = 0);

public:
  Validator(const std::vector<Employee>& employees, int verbosity = 0);
  ~Validator();

  void roster()    const;
  void summarize() const;

  std::vector<int>  calls(   int minute, const std::vector<Call>& calls);
  std::vector<Call> validate(int minute, const std::vector<int>&  actions);
};

#endif

