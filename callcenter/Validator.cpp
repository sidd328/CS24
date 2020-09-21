#include "Validator.h"

#include <iomanip>
#include <iostream>

std::string s(int i) {
  return std::to_string(i);
}


Validator::Validator(const std::vector<Employee>& employees, int verbosity) {
  mEmployees  = employees;
  mVerbosity  = verbosity;
  mTotalCalls = 0;
  mTotalScore = 0;
  mTotalTime  = 0;
}

Validator::~Validator() {
  for(const Employee& employee: mEmployees) {
    delete employee.call;
  }

  for(const auto& pair: mPool) {
    delete pair.second;
  }
}


std::vector<int> Validator::calls(int minute, const std::vector<Call>& calls) {
  if((calls.size() > 0 && mVerbosity > 0) || mVerbosity > 2) {
    const char* x = (calls.size() == 1)? "" : "s";
    std::cout << '[' << std::setfill('0') << std::setw(2) << (minute + 480) / 60;
    std::cout << ':' << std::setfill('0') << std::setw(2) << (minute + 480) % 60;
    std::cout << "]: " << s(calls.size()) + " new call" << x << ".\n" << std::setfill(' ');
  }

  std::vector<int> call_ids;
  for(const Call& call: calls) {
    mPool[call.id] = new Call(call);
    call_ids.emplace_back(call.id);
  }

  return call_ids;
}

void Validator::log(const std::string& message, int level) {
  if(level < mVerbosity) std::cout << message << '\n';
}

void Validator::summarize() const {
  for(const Employee& employee: mEmployees) {
    if(employee.call != nullptr) {
      throw validation_error("Some calls were still in progress.");
    }
  }

  if(!mPool.empty()) {
    throw validation_error("Some calls were still on hold.");
  }

  std::cout << "Calls completed:    " << std::setw(6) << mTotalCalls << '\n';
  std::cout << "Total wait time:    " << std::setw(6) << mTotalTime  << '\n';
  std::cout << "Weighted wait time: " << std::setw(6) << mTotalScore << '\n';
}

void Validator::roster() const {
  if(mVerbosity < 3) {
    return;
  }

  std::cout << " ID  Skill  Name\n";
  for(const Employee& employee: mEmployees) {
    std::cout << std::setw(3) << employee.id;
    std::cout << std::setw(7) << employee.skill;
    std::cout << "  " << employee.name << "\n";
  }

  std::cout << std::endl;
}

std::vector<Call> Validator::validate(int minute, const std::vector<int>& actions) {
  const int alen = actions.size();
  const int elen = mEmployees.size();
  std::vector<Call> learned;

  if(alen != elen) {
    throw validation_error("You have " + s(elen) + " employees but returned " + s(alen) + " actions.");
  }

  // Complete / hold any calls we can:
  for(int i = alen - 1; i >= 0; --i) {
    int       action   = actions[i];
    Employee& employee = mEmployees[i];
    Call*     call     = employee.call;

    if(action == -1) {
      // Complete the current call:
      if(call == nullptr) {
        throw validation_error(employee.name + " doesn't have an active call to complete.");
      }

      if(call->work_required > call->work_performed) {
        throw validation_error(employee.name + " ended Call " + s(call->id) + " prematurely.");
      }

      int time = minute - call->recieved + 1;
      mTotalScore += time * call->importance;
      mTotalTime  += time;
      mTotalCalls += 1;

      log(" - " + employee.name + " completed Call " + s(call->id) + " after " + s(time) + " minutes.");
      employee.call = nullptr;
      delete call;
    }
    else if(call && action != call->id) {
      // Put the current call back into the hold pool:
      log(" - " + employee.name + " puts Call " + s(call->id) + " on hold.", 3);
      employee.call   = nullptr;
      mPool[call->id] = call;
    }
  }

  // Pick up and work on calls:
  for(int i = alen - 1; i >= 0; --i) {
    int       action   = actions[i];
    Employee& employee = mEmployees[i];
    Call*     call     = employee.call;

    if(action < 1) {
      // Call was completed earlier, or
      // we're not doing any work.
      continue;
    }
    else if(call != nullptr) {
      // Collect call info or perform some work:
      if(call->difficulty > employee.skill && call->work_performed >= 0) {
        log(" ! Call " + s(call->id) + " is too difficult for " + employee.name + '.');
        continue;
      }

      call->work_performed += 1;
      if(call->work_performed == 0) {
        // It's a new call - get info:
        learned.emplace_back(*call);
      }
    }
    else {
      // Take the correct call out of the hold pool:
      auto itr = mPool.find(action);
      if(itr == mPool.end()) {
        throw validation_error("Call " + s(action) + " is not on hold.");
      }

      Call* call = itr->second;
      log(" - " + employee.name + " takes Call " + s(call->id) + " out of hold.", 3);
      employee.call = call;
      mPool.erase(itr);
    }
  }

  // Return any new information:
  return learned;
}
