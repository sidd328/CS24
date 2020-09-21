#include "CallCenter.h"
#include "Validator.h"

#include <fstream>
#include <iostream>
#include <sstream>

// Helper function to read in an employee file.
std::vector<Employee> parse_employees(const std::string &filename)
{
  std::vector<Employee> employees;
  Employee employee;
  employee.id = 0;
  employee.call = nullptr;

  std::string line;
  std::ifstream file(filename);
  while (std::getline(file, line))
  {
    std::istringstream stream(line);
    if (!(stream >> employee.skill >> std::ws))
    {
      throw std::runtime_error("Bad employee line: " + line);
    }

    if (!std::getline(stream, employee.name))
    {
      throw std::runtime_error("Bad employee line: " + line);
    }

    employees.emplace_back(employee);
    employee.id += 1;
  }

  return employees;
}

// Helper function to read in calls from a data line.
std::vector<Call> parse_calls(int minute, const std::string &line)
{
  static int call_id = 1;

  Call call;
  call.id = call_id;
  call.recieved = minute;
  call.work_performed = -1;

  std::vector<Call> calls;
  std::istringstream stream(line);
  while (stream >> call.importance >> call.difficulty >> call.work_required)
  {
    calls.emplace_back(call);
    call.id += 1;
    call_id += 1;
  }

  return calls;
}

int main(int argc, char **argv) {

  if (argc != 3)
  {
    std::cerr << "USAGE: ./callcenter [employee-file] [call-file]\n";
    std::exit(1);
  }

  // Increase this to get more output!
  int verbosity = 3;

  CallCenter *center = nullptr;

  try
  {
    std::vector<Employee> employees = parse_employees(argv[1]);
    Validator validator(employees, verbosity);
    center = CallCenter::create(employees);
    validator.roster();

    int minute = 0;
    std::string line;
    std::ifstream callfile(argv[2]);
    while (std::getline(callfile, line))
    {
      std::vector<Call> calls = parse_calls(minute, line);

      // Get any incoming calls and decide what to do:
      std::vector<int> call_ids = validator.calls(minute, calls);
      std::vector<int> actions = center->calls(minute, call_ids);

      // Learn the details about any newly-answered calls:
      std::vector<Call> learned = validator.validate(minute, actions);
      center->learn(minute, learned);
      minute += 1;
    }

    validator.summarize();
  }
  catch (const validation_error &e)
  {
    std::cerr << "Inavlid action: " << e.what() << "\n";
    delete center;
    std::exit(1);
  }
  catch (const std::exception &e)
  {
    std::cerr << "Unexpected exception: " << e.what() << "\n";
    delete center;
    std::exit(1);
  }

  delete center;
  return 0;
}
