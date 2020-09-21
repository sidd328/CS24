# Call Center

_Due Monday, August 17th, before midnight Pacific Time._

**Note:**  You can work on this assignment with a partner if you want to. If you
do,  the two of you should make  one submission  on Gradescope with both of your
names listed.  You'll need to do this every time you submit.


## Backstory

Your local call center is having serious customer satisfaction problems. Callers
are put on hold indefinitely, they have their time wasted talking to unqualified
agents... And it's even happening to important customers! Which means they might
stop paying!  And that would be terrible.

So management decided to finally revamp the Just Do Whatever customer management
system they'd been using, and they've hired you to write the replacement.  Write
a program that organizes the employees more efficiently and make sure that those
valued customers get served in a timely manner.


## Your Assignment

- Write a class that assigns calls to call center employees.
- Complete all the calls that come in during the day.
- Don't go over your weighted wait time budget.
- You can use anything from the standard library.
- Make sure you don't print anything that isn't explicitly allowed.
- Make sure you don't have any memory leaks.


## The Workday

Your day starts at eight in the morning,  and is divided into minutes. The phone
lines open at eight exactly,  and stay open until five in the evening.  You have
until six  to finish up any long-running calls  before all the employees go home
for the night.


## Employees

You have  a variety of people  woking at your  call center,  some more competent
than others.  When your call center opens each morning, you'll get a list of all
the  employees working that day.  This will include some additional  information
that will help you assign calls to employees:

- Their **name**.  You don't actually care about this, but it's used for logging.
- Their **ID**.  This is a non-negative integer less than the number of employees.
- Their **skill**.  This is the highest call difficulty the employee can help with.


## Calls

Throughout the day,  you'll get calls  from your customers  asking for technical
support.  When calls arrive  at your call center,  they're placed in the  **hold
pool** - a collection of  on-hold calls  that can be answered by any employee. A
call has the following attributes, all of which are positive integers:

- A unique **ID**.
- An **importance** - how important the customer is.
- A **difficulty** - how difficult the problem is to solve.
- Minutes of **work required** - how long it will take a skilled employee
  to solve the problem.

When calls come in,  you only know  their IDs.  An employee  needs to answer the
call in order to figure out  how important  and difficult it is.  This takes one
minute.  Any employee can determine the importance and difficulty of a call, but
only employees with skill greater than or equal to a call's difficulty  can work
on solving the problem.  Having an  unskilled employee work on a  difficult call
wastes everybody's time.

It takes one minute  to transfer a call between employees,  or to take it out of
hold or  put it on hold;  this doesn't count  towards the work required to solve
the problem.  Useful work only happens when the skilled employee who was working
on the call the previous minute continues to do so for another minute.

Once employees have performed at least the number of minutes of work required by
the call,  any employee can  **complete**  the call.  This takes one minute, and
then the caller hangs up, their problem solved.


## Assigning Work

At the beginning of every minute you get a list of the IDs of any new calls. You
can then  decide how to allocate  your employees' time  for that minute.  You do
this by returning a vector of orders such that:

```
vector[employee_id] = call_id;  // Work on the call with this ID, or...
                    = -1;       // Complete your current call, or...
                    = 0;        // Don't work on anything.
```

Any employee who is told to work on a different call will put their current call
on hold - it goes back into the hold pool. Then all employees will work on their
assigned calls. If they aren't working on the correct call already, they'll take
the correct call out of the hold pool;  otherwise, they'll perform one minute of
work.

At the end  of every minute,  you will get notified of all the  call details you
collected during that minute.  Use this to update your call representations with
the correct values.


## Weighted Wait Time

Management needs a way to make sure  that your program is doing its job, so they
came up with a metric to measure it: weighted weight time. Every call that comes
in is given an importance score.  When the call is completed, this importance is
multiplied by the number of minutes the call took to complete.  Your final score
is the sum of the weighted weight times for all calls.

Lower scores are better.  You'll need to get a lower score than the old software
to convince management to pay for your new version.


## Example Call

```
[11:10]: A call comes in.
[11:11]: ...on hold...
[11:12]: Alice takes the call out of hold.
[11:13]: Alice collects call info; it will need six minutes of work.
[11:14]: Alice isn't skilled enough; she transfers the call to Bob.
[11:15]: Bob works on the call (minute 1/6).
[11:16]: Bob works on the call (minute 2/6).
[11:17]: Bob works on the call (minute 3/6).
[11:18]: Bob works on the call (minute 4/6).
[11:19]: Bob gets distracted and puts the call on hold.
[11:20]: ...on hold...
[11:21]: ...on hold...
[11:22]: Carol takes the call out of hold.
[11:23]: Carol works on the call (minute 5/6).
[11:24]: Carol works on the call (minute 6/6).
[11:25]: Carol completes the call.
```

This call took sixteen minutes from being received to being completed,  so if it
had an importance of three, it would have a weighted wait time of 48 minutes.


## Extra Credit

To get full credit,  don't perform  any invalid actions,  complete all the calls
you recieve  before six PM,  and don't  go over your  wait budget.  To get extra
credit,  minimize your  weighted wait times!  The  best submission will  get ten
extra credit points, the next best submission will get nine, and so on.
