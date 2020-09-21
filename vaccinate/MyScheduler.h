#ifndef MYSCHEDULER_H
#define MYSCHEDULER_H

#define INF 4294967290

#include "Scheduler.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <set>
#include <math.h>
#include <queue>

struct MyRoute;
struct MyCity;
struct Continuation;
struct compareCont;

//Used in Djikstras
struct compShipment
{
  bool operator()(Shipment &a, Shipment &b)
  {
    {
      if (a.route_id == b.route_id)
        return a.source < b.source;
      return a.route_id < b.route_id;
    }
  }
};

struct Continuation
{
  std::vector<MyRoute *> routes;
  unsigned int distance;

  Continuation(std::vector<MyRoute *> input, unsigned int dist)
  {
    routes = input;
    distance = dist;
  }

  bool operator<(const Continuation &other) const
  {
    return this->distance > other.distance;
  }
};

struct MyRoute
{
  unsigned int id;   // The unique ID of this route.
  MyCity *city1;     // The name of the city at one end of this route.
  MyCity *city2;     // The name of the city at the other end of the route.
  unsigned int days; // The number of days it takes to travel this route.
  unsigned int load; // The number of doses that fit in one vehicle.
  double cost;       // The price you pay to ship one load.

  bool visited();

  MyRoute(Route route, MyCity *mcity1, MyCity *mcity2)
  {
    id = route.id;
    city1 = mcity1;
    city2 = mcity2;
    days = route.days;
    load = route.load;
    cost = route.cost;
  }

  bool has(MyCity *city)
  {
    return ((city1 == city) || (city2 == city));
  }

  ~MyRoute()
  {
    city1 = nullptr;
    city2 = nullptr;
  }
};

struct MyCity
{
  std::string name;
  unsigned int doses;                      // The number of doses currently in the city
  std::vector<MyRoute *> connected_routes; //list of all routes connected to the city
  std::vector<MyRoute *> validated_route;  //valid route to be taken (either cost or days efficient)
  MyRoute *prev;
  unsigned int valid_route_cost; //cost of the valid route
  unsigned int valid_route_time; // time taken to traverse the validated_route
  int daysToSpare;               //Days till deadline
  bool is_factory;               // whether or not this city is a factory
  bool visited;                  //Helper member variable in Djikstras algorithm
  MyCity(std::string _name, unsigned int _doses) : name(_name), doses(_doses), prev(nullptr), valid_route_cost(INF), valid_route_time(INF), daysToSpare(0), is_factory(false), visited(false) {}
  ~MyCity()
  {
    connected_routes.clear();
    validated_route.clear();
  }
};

class MyScheduler : public Scheduler
{
  std::map<std::string, MyCity *> mCities;
  std::vector<MyRoute *> mRoutes;
  unsigned int mDeadline;
  std::vector<MyCity *> mFactories;

  // Member Variables
public:
  // Constructor
  MyScheduler(
      unsigned int deadline,
      std::map<std::string, unsigned int> cities,
      std::vector<std::string> factories,
      std::vector<Route> routes);

  ~MyScheduler();

  // Required Member Function
  std::vector<Shipment> schedule();

  bool compareRoutes(MyRoute *r1, MyRoute *r2);

  bool cheapestPath();
  void shortestPath();
  //void selectedPath();

  bool validate(const std::vector<MyRoute *> &validate);
  unsigned int total_time(const std::vector<MyRoute *> &validate);
  unsigned int total_cost(const std::vector<MyRoute *> &validate);
  unsigned int public_cost(const std::vector<MyRoute *> &validate);
  void unvisit();

  void print_paths();

  Shipment create_ship(unsigned int _doses, unsigned int _id, unsigned int _day, std::string start);

  friend struct Continuation;
};

#endif
