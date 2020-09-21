#ifndef NOODLESHOP_H
#define NOODLESHOP_H

#include "Objects.h"
#include "Actions.h"

// This file describes the NoodleShop interface you'll need to implement.
// Don't edit this file!  Your code goes in the MyNoodleShop.* files.

class NoodleShop {
public:
  static NoodleShop* create(
    int                 npots,      // The number of pots you have available.
    int                 rent,       // The amount you paid to rent a stall for the day.
    int                 customers,  // The approximate number of customers you expect today.
    std::vector<Noodle> noodles     // Information about the noodles you're selling.
  );

public:
  // This function gets called at the beginning of every minute.
  // Any new orders coming in will be in the orderlist vector.
  // Return a vector of all the orders you agree to fulfill.
  virtual std::vector<Order> orders(int minute, std::vector<Order> orderlist) = 0;

  // This function will be called immediately after the orders function.
  // Return the action that you decide to perform in this minute.
  // Return a new Action object; the framework will delete it.
  virtual Action* action(int minute) = 0;

  // Destructor
  virtual ~NoodleShop() {}
};

#endif
