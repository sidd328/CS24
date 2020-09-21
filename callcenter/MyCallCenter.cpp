#include "MyCallCenter.h"
#include <cmath>

int finished = 0;
int percentBuf = 10; // Percent of employees to set as buffer
int percentTop = 10;

CallCenter *CallCenter::create(std::vector<Employee> employees)
{
  return new MyCallCenter(employees);
}

// Sort functions for vectors
bool compEmpsSkill(MyEmployee *a, MyEmployee *b)
{
  return a->skill < b->skill;
}

bool compEmpsID(MyEmployee *a, MyEmployee *b)
{
  return a->id < b->id;
}

bool compCallImp(MyCalls *a, MyCalls *b)
{
  if (a->importance == b->importance)
  {
    return a->difficulty > b->difficulty;
  }
  return a->importance > b->importance;
}

bool compCallDiff(MyCalls *a, MyCalls *b)
{
  if (a->difficulty == b->difficulty)
  {
    return a->importance < b->importance;
  }
  return a->difficulty < b->difficulty;
}

/* Usage
  std::sort(x.begin(), x.end(), compCallImp);

  x will now be sorted by difficulty
*/

MyCallCenter::MyCallCenter(std::vector<Employee> employee)
{
  // Determines number of employees to make buffers, rounding down (at least 1 Buf)
  numBuf = floor(employee.size() * percentBuf / 100.0);
  if (numBuf == 0)
  {
    numBuf++;
  }

  numTop = floor(employee.size() * percentTop / 100.0);
  if (numTop == 0)
  {
    numTop++;
  }

  for (Employee emp : employee)
  {
    mEmps.push_back(new MyEmployee(emp));
  }

  //When the callcenter is constructed set all previous records to 0
  previous_records.resize(mEmps.size());
  for (int i = 0; (unsigned)i <= previous_records.size() - 1; i++)
  {
    previous_records[i] = 0;
  }

  on_hold.clear();
  on_hold.resize(0);

  std::sort(mEmps.begin(), mEmps.end(), compEmpsSkill); // Sorted mEmps from lowest skill to highest skill
}

MyCallCenter::~MyCallCenter()
{
  for (int i = 0; (unsigned)i <= mEmps.size() - 1; i++)
  {
    delete mEmps[i];
    mEmps[i] = nullptr;
  }

  if (on_hold.size() > 0)
  {
    for (int k = 0; (unsigned)k <= on_hold.size() - 1; k++)
    {
      if (on_hold[k] != nullptr)
      {
        delete on_hold[k];
        on_hold[k] = nullptr;
      }
    }
  }
}

void MyCallCenter::clear_on_hold()
{
  //Function used to remove all the nullptr values from on_hold
  std::vector<MyCalls *>::iterator it = on_hold.begin();
  while (it != on_hold.end())
  {
    if ((*it) == nullptr)
    {
      it = on_hold.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

void MyCallCenter::try_to_answer(int start)
{
  clear_on_hold();
  (void)start;
  if (!on_hold.empty())
  {
    //First we give any employee who isnt doing anything a call off on_hold (if they can handle it)
    std::sort(on_hold.begin(), on_hold.end(), compCallDiff);
    std::reverse(on_hold.begin(), on_hold.end());
    std::sort(mEmps.begin(), mEmps.end(), compEmpsSkill);

    for (int i = 0; (unsigned)i < mEmps.size(); i++)
    {
      for (int j = 0; (unsigned)j < on_hold.size(); j++)
      {
        if (on_hold[j])
        {
          if (!mEmps[i]->mycall)
          {
            if ((mEmps[i]->skill >= on_hold[j]->difficulty))
            {
              mEmps[i]->mycall = on_hold[j];
              mEmps[i]->mycall->attendee = mEmps[i];
              on_hold[j] = nullptr;
            }
          }
        }
      }
    }

    //Cler on_hold of all nullptr values (set as placeholders)
    clear_on_hold();

    //Now we make sure that the employees after the buffer employees get the most difficult call they can work on (and the most important)
    if (!on_hold.empty())
    {
      for (int i = 0; (unsigned)i < mEmps.size(); i++)
      {
        for (int j = 0; (unsigned)j < on_hold.size(); j++)
        {
          if (i < numBuf) //Employees in the buffer zone only take a call if their current call is less difficult (that means that we already know about the information)
          {
            if ((mEmps[i]->mycall) && (mEmps[i]->mycall->status) && (mEmps[i]->skill >= on_hold[j]->difficulty) && ((mEmps[i]->mycall->difficulty < on_hold[j]->difficulty) || ((mEmps[i]->mycall->difficulty == on_hold[j]->difficulty) && (mEmps[i]->mycall->importance < on_hold[j]->importance))))
            {
              MyCalls *temp = mEmps[i]->mycall;
              mEmps[i]->mycall = on_hold[j];
              mEmps[i]->mycall->attendee = mEmps[i];
              temp->attendee = nullptr;
              on_hold[j] = temp;
            }
          }
          else //If employee isn tin the buffer, give the employee the most difficult call they can handle
          {
            if ((mEmps[i]->mycall) && (mEmps[i]->skill >= on_hold[j]->difficulty) && ((mEmps[i]->mycall->difficulty < on_hold[j]->difficulty) || ((mEmps[i]->mycall->difficulty == on_hold[j]->difficulty) && (mEmps[i]->mycall->importance < on_hold[j]->importance))))
            {
              MyCalls *temp = mEmps[i]->mycall;
              mEmps[i]->mycall = on_hold[j];
              mEmps[i]->mycall->attendee = mEmps[i];
              temp->attendee = nullptr;
              on_hold[j] = temp;
            }
          }
        }
      }
    }
  }
}

std::vector<int> MyCallCenter::calls(int minute, const std::vector<int> &call_ids)
{
  std::stringstream stream; //used for debugging
  (void)minute;

  std::vector<int> result; //Vector to be returned at the end of the function
  result.clear();
  result.resize(mEmps.size());

  //merge incoming calls into unanswered calls:
  unanswered.insert(unanswered.end(), call_ids.begin(), call_ids.end());

  //Remove calls that are too much for an employee to handle and put it on hold (provided we know about the call)
  for (MyEmployee *emps : mEmps)
  {
    if ((emps->mycall != nullptr) && (emps->mycall->status) && (emps->mycall->difficulty > emps->skill))
    {
      on_hold.push_back(emps->mycall);
      emps->mycall->attendee = nullptr;
      emps->mycall = nullptr;
    }
  }

  //Give buffer employees an unanswered call if they currently dont have a call
  unsigned int start = 0;
  if (!unanswered.empty())
  {
    for (int i = 0; i < numBuf; i++)
    {
      if (!mEmps[i]->mycall)
      {
        mEmps[i]->mycall = new MyCalls();
        mEmps[i]->mycall->id = unanswered.front();
        unanswered.erase(unanswered.begin());
      }

      if (unanswered.empty())
      {
        break;
      }
    }
  }

  //give employees (who arent answering unanswered calls) the most important calls
  try_to_answer(start);

  //If there are still employee doing nothing and there are unanswered calls, give them something
  if (!unanswered.empty())
  {
    for (int i = 0; (unsigned)i < mEmps.size(); i++)
    {
      if (!mEmps[i]->mycall)
      {
        mEmps[i]->mycall = new MyCalls();
        mEmps[i]->mycall->id = unanswered.front();
        unanswered.erase(unanswered.begin());
      }

      if (unanswered.empty())
      {
        break;
      }
    }
  }

  //Now we start to generate the final return vector
  std::sort(mEmps.begin(), mEmps.end(), compEmpsID); //Sort employees by ID as that is what teh return vector requires

  for (int i = 0; (unsigned)i <= mEmps.size() - 1; i++)
  {
    if (mEmps[i]->mycall != nullptr)
    {
      result[i] = mEmps[i]->mycall->id;
    }
    else
    {
      result[i] = 0;
    }
  }

  //We also properly increment the work_performed values for each call if the same employee has had the call for a full minute
  //And if we have finished all the work, we can then change the result vector to say -1 (call ended) instead
  for (int i = 0; (unsigned)i <= mEmps.size() - 1; i++)
  {
    if (mEmps[i]->mycall)
    {
      if (mEmps[i]->mycall->id == previous_records[i])
      {
        mEmps[i]->mycall->work_performed++;
        if (mEmps[i]->mycall->work_performed > mEmps[i]->mycall->work_required)
        {
          result[i] = -1;
          mEmps[i]->mycall->attendee = nullptr;
          delete mEmps[i]->mycall;
          mEmps[i]->mycall = nullptr;
        }
      }
    }
  }
  //
  //
  //
  //
  //
  //
  //
  //
  //Debugging statements---------------------------------------------------------
  /* for (MyCalls *item : on_hold)
  {
    stream << item->id << " ";
  }
  std::cout << "On hold: " << stream.str() << std::endl;

  stream.str("");
  clear_on_hold();
  for (MyEmployee *emps : mEmps)
  {
    if (emps->mycall)
    {
      stream << emps->mycall->id << " ";
    }
    else
    {
      stream << 0 << " ";
    }
  }
  std::cout << "Calls being answered:   " << stream.str() << std::endl; */
  //Debugging statements---------------------------------------------------------
  //
  //
  //
  //
  //
  //
  //
  //

  previous_records = result;
  std::sort(mEmps.begin(), mEmps.end(), compEmpsSkill); // sort employees for the next minute
  return result;
}

void MyCallCenter::learn(int minute, const std::vector<Call> &calls)
{
  (void)minute;
  // Loop through every employee and every call to match all of them
  if (!calls.empty())
  {
    for (MyEmployee *emp : mEmps)
    {
      for (Call item : calls)
      {
        // if call ids match
        if (emp->mycall != nullptr)
        {
          if (item.id == emp->mycall->id)
          {
            emp->mycall->difficulty = item.difficulty;
            emp->mycall->importance = item.importance;
            emp->mycall->recieved = item.recieved;
            emp->mycall->work_performed = item.work_performed;
            emp->mycall->work_required = item.work_required;

            emp->mycall->attendee = emp;
            emp->mycall->status = true;
          }
        }
      }
    }
  }
}