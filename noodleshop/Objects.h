#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>

// This file contains the objects you'll use to communicate
// with the autograder framework.  Don't edit this file!


// This class contains information about a single type of noodle.
// A vector of this type will be passed to your constructor.
struct Noodle {
  // The name of the noodle.
  std::string name;

  // The number of servings in one batch.
  int batch_size;

  // The time needed to cook one batch.
  int cook_time;

  // The price of the ingredients needed
  // to make one batch of this noodle.
  int ingredient_cost;

  // The amount customers pay for one serving.
  int serving_price;
};


// A vector of this type will be passed to your shop each
// minute to tell you about any new orders coming in.
struct Order {
  // The unique ID of this order.
  // IDs are positive and strictly increasing.
  int id;

  // The name of the noodle being ordered.
  std::string noodle;

  // Comparison operator to support std::set operations.
  bool operator < (const Order& other) const {
    return id < other.id;
  }
};


// Return a vector of this type in a ServeAction
// to give noodles to any number of customers.
struct Serve {
  // The ID of the order you're fulfilling.
  int order_id;

  // The ID of the pot you're serving from.
  // Pot IDs are integers 0 <= id < npots.
  int pot_id;
};

#endif
