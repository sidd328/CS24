#include "MyScheduler.h"
MyScheduler::~MyScheduler()
{
  std::map<std::string, MyCity *>::iterator it = mCities.begin();
  while (it != mCities.end())
  {
    if (it->second)
      delete it->second;
    ++it;
  }

  auto rout = mRoutes.begin();
  while (rout != mRoutes.end())
  {
    if ((*rout))
      delete (*rout);
    ++rout;
  }
}

void MyScheduler::print_paths()
{
  std::map<std::string, MyCity *>::iterator it = mCities.begin();
  while (it != mCities.end())
  {
    std::cout << "Routes for City " << it->second->name << ": " << std::endl;
    for (unsigned int i = 0; i < it->second->validated_route.size(); i++)
    {
      std::cout << "ID " << it->second->validated_route[i]->id << " " << it->second->validated_route[i]->city1->name << "-" << it->second->validated_route[i]->city2->name << " Cost: " << total_cost(it->second->validated_route) << ", ";
    }
    std::cout << std::endl;
    ++it;
  }
}

void MyScheduler::unvisit()
{
  std::map<std::string, MyCity *>::iterator it = mCities.begin();
  while (it != mCities.end())
  {
    if (!it->second->is_factory)
    {
      it->second->visited = false;
    }
    ++it;
  }
}

Shipment MyScheduler::create_ship(unsigned int _doses, unsigned int _id, unsigned int _day, std::string start)
{
  Shipment ship;
  ship.day = _day;
  ship.doses = _doses;
  ship.route_id = _id;
  ship.source = start;

  return ship;
}

bool sortID(Shipment &a, Shipment &b)
{
  return a.route_id < b.route_id;
}

bool MyRoute::visited()
{
  return (this->city1->visited || this->city2->visited);
}

bool MyScheduler::validate(const std::vector<MyRoute *> &validate)
{
  return (total_time(validate) <= mDeadline);
}

unsigned int MyScheduler::total_time(const std::vector<MyRoute *> &validate)
{
  unsigned int sum = 0;
  for (MyRoute *route : validate)
  {
    sum += route->days;
  }
  return sum;
}

unsigned int MyScheduler::total_cost(const std::vector<MyRoute *> &validate)
{
  MyCity *src;
  //Set the source factory
  if (validate[0]->city1->is_factory)
    src = validate[0]->city1;
  else
    src = validate[0]->city2;

  //Go to the ending city (the final destination)
  MyCity *dst = src;
  int sum = 0;
  for (MyRoute *route : validate)
  {
    if (route->city1 != dst)
    {
      sum += route->city1->doses;
      dst = route->city1;
    }
    else
    {
      sum += route->city2->doses;
      dst = route->city2;
    }
  }

  unsigned int cost = 0;
  for (MyRoute *route : validate)
  {
    cost += ceil(((double)sum / (double)route->load)) * route->cost;
    if (route->city1 != src)
    {
      sum -= route->city1->doses;
      src = route->city1;
    }
    else
    {
      sum -= route->city2->doses;
      src = route->city2;
    }
  }

  return cost;
}

unsigned int MyScheduler::public_cost(const std::vector<MyRoute *> &validate)
{
  //Go to the ending city (the final destination)
  MyCity *dst;
  if (validate.size() > 1)
  {
    if ((validate[validate.size() - 1]->city1 == validate[validate.size() - 2]->city1) || (validate[validate.size() - 1]->city1 == validate[validate.size() - 2]->city2))
      dst = validate[validate.size() - 1]->city2;
    else
      dst = validate[validate.size() - 1]->city1;
  }
  else
  {
    if (validate[0]->city2->is_factory)
      dst = validate[0]->city1;
    else
      dst = validate[0]->city2;
  }

  unsigned int cost = 0;
  for (MyRoute *route : validate)
  {
    cost += ceil(((double)dst->doses / (double)route->load)) * route->cost;
  }

  return cost;
}

Scheduler *Scheduler::create(unsigned int deadline, std::map<std::string, unsigned int> cities, std::vector<std::string> factories, std::vector<Route> routes)
{
  return new MyScheduler(deadline, cities, factories, routes);
}

bool MyScheduler::compareRoutes(MyRoute *r1, MyRoute *r2)
{
  if (r1->cost == r2->cost)
    return (r1->days < r2->days);
  return (r1->cost < r2->cost);
}

// Djikstras algorithms, for cheapest as well as fastest routes
bool MyScheduler::cheapestPath()
{
  //Set up queue (heap) for djikstras with the source city's routes being the first items in queue
  std::priority_queue<Continuation> queue;
  for (MyCity *fact : mFactories)
  {
    for (MyRoute *route : fact->connected_routes)
    {
      if (!route->city2->is_factory || !route->city1->is_factory)
      {
        Continuation original({route}, 0);
        original.distance = total_cost(original.routes);
        queue.push(original);
      }
    }
    fact->visited = true;
  }
  //std::sort(queue.begin(), queue.end(), compare); //Sort by placing smallest "distance" last

  //Keep going through routes in the queue, until the queue is empty.
  while (!queue.empty())
  {
    Continuation curr = queue.top();
    queue.pop();

    //The city that the route stored in curr leads to
    MyCity *on;
    if (!curr.routes[curr.routes.size() - 1]->city1->visited)
      on = curr.routes[curr.routes.size() - 1]->city1;
    else
      on = curr.routes[curr.routes.size() - 1]->city2;

    //In the scenario where the queue has 2 paths to the same city,
    //and one was chosen before (making the city visited), remove this unecessary path
    if (on->visited)
      continue;

    //Check to make sure that the route leading to teh city is valid and is shorter than the current route pointing to the city
    //std::cout << "For city: " << on->name << " Current cost: " << on->valid_route_cost << " trying to replace: " << curr.distance << std::endl;
    if ((validate(curr.routes)) && curr.distance < on->valid_route_cost)
    {
      on->validated_route = curr.routes;
      on->valid_route_cost = curr.distance;
      on->valid_route_time = total_time(curr.routes);
    }

    //Now add all the connected routes from the "on" city to other cities making sure not to create a loop.
    //Create a new Continuation object by adding the next route to the previous routes, thereby forwarding the path
    for (MyRoute *route : on->connected_routes)
    {
      if (!route->visited() && !route->city1->is_factory && !route->city2->is_factory)
      {
        std::vector<MyRoute *> push_forward = curr.routes;
        int nextDist;
        push_forward.push_back(route);
        nextDist = total_cost(push_forward);

        queue.push(Continuation(push_forward, nextDist));
      }
    }

    //set current city to visited after adding all its connected routes (that dont create loops)
    on->visited = true;

    //Sort routes again, placing the shortest distance route at the end
    //std::sort(queue.begin(), queue.end(), compare);
  }

  //revert all cities to unvisited for the next iteration of function
  unvisit();

  std::map<std::string, MyCity *>::iterator it = mCities.begin();
  while (it != mCities.end())
  {
    if (it->second->valid_route_cost == INF)
      return false;
    ++it;
  }
  return true;
}

void MyScheduler::shortestPath()
{
  //Set up queue (heap) for djikstras
  std::priority_queue<Continuation> queue;
  for (MyCity *fact : mFactories)
  {
    for (MyRoute *route : fact->connected_routes)
    {
      if (!route->city2->is_factory || !route->city1->is_factory)
        queue.push(Continuation({route}, route->days));
    }
    fact->visited = true;
  }
  //std::sort(queue.begin(), queue.end(), compare); //Sort by placing smallest distance last

  //Keep going through routes in the queue, until the queue is empty.
  while (!queue.empty())
  {
    Continuation curr = queue.top();
    queue.pop();

    //The city that the route stored in curr leads to
    MyCity *on;
    if (!curr.routes[curr.routes.size() - 1]->city1->visited)
      on = curr.routes[curr.routes.size() - 1]->city1;
    else
      on = curr.routes[curr.routes.size() - 1]->city2;

    //In the scenario where the queue has 2 paths to the same city,
    //and one was chosen before (making the city visited), remove this unecessary path
    if (on->visited)
      continue;

    //Check to make sure that teh route leading to teh city is valid and is shorter than the current route pointing to the city
    if ((validate(curr.routes)) && (on->valid_route_time == INF) && (curr.distance < on->valid_route_time) && (total_cost(curr.routes) < on->valid_route_cost))
    {
      on->validated_route = curr.routes;
      on->valid_route_time = curr.distance;
      on->valid_route_cost = total_cost(curr.routes);
    }

    //Now add all the connected routes from the "on" city to other cities making sure not to create a loop.
    //Create a new Continuation object by adding the next route to the previous routes, thereby forwarding the path
    for (MyRoute *route : on->connected_routes)
    {
      if (!route->visited() && !route->city2->is_factory && !route->city1->is_factory)
      {
        std::vector<MyRoute *> push_forward = curr.routes;
        int nextDist = curr.distance;
        push_forward.push_back(route);
        nextDist += route->days;

        queue.push(Continuation(push_forward, nextDist));
      }
    }

    //set current city to visited after adding all its connected routes (that dont create loops)
    on->visited = true;

    //Sort routes again, placing the shortest distance route at the end
    //std::sort(queue.begin(), queue.end(), compare);
  }

  unvisit();
}

/* void MyScheduler::selectedPath()
{
  //first check whether all cities have a cheapest route
  std::map<std::string, MyCity *>::iterator iter = mCities.begin();
  bool success = false;
  while (iter != mCities.end())
  {
    if (iter->second->is_factory)
    {
      //std::cout << "Starting Factory: " << iter->second->name << std::endl;
      success = (cheapestPath(iter->second) || success);
    }
    ++iter;
  }

  //If the cheapest route doesnt fit, then find the shosrtest route
  if (!success)
  {
    iter = mCities.begin();
    while (iter != mCities.end())
    {
      if (iter->second->is_factory)
      {
        shortestPath(iter->second);
      }
      ++iter;
    }
  }
} */

// MyScheduler Member Functions
MyScheduler::MyScheduler(unsigned int deadline, std::map<std::string, unsigned int> cities, std::vector<std::string> factories, std::vector<Route> routes)
{
  mDeadline = deadline;

  for (auto city : cities)
  {
    mCities.insert(std::pair<std::string, MyCity *>(city.first, new MyCity(city.first, city.second)));
  }

  for (auto route : routes)
  {
    auto it = mCities.find(route.city1);
    MyCity *city1 = it->second;
    auto it2 = mCities.find(route.city2);
    MyCity *city2 = it2->second;
    mRoutes.push_back(new MyRoute(route, city1, city2));
    city1->connected_routes.push_back(mRoutes[mRoutes.size() - 1]);
    city2->connected_routes.push_back(mRoutes[mRoutes.size() - 1]);
  }

  for (auto factory : factories)
  {
    auto fac = (mCities.find(factory));
    mFactories.push_back(fac->second);
    fac->second->is_factory = true;
    fac->second->valid_route_cost = 0;
    fac->second->valid_route_time = 0;
  }
}

std::vector<Shipment> MyScheduler::schedule()
{
  std::vector<Shipment> result;
  std::priority_queue<Shipment, std::vector<Shipment>, compShipment> queue;

  //first check whether all cities have a cheapest route
  bool success = false;
  success = (cheapestPath() || success);

  //If the cheapest route doesnt fit, then find the shosrtest route
  if (!success)
  {
    shortestPath();
  }

  //print_paths();

  // All the stuff to find the optimal routes, now stored in each city

  // Creates the shipments for each of the route portions for each city
  auto itr = mCities.begin();
  while (itr != mCities.end())
  { // iterating through each city
    unsigned int vaccinesNeeded = itr->second->doses;
    MyCity *sourceCity = nullptr;
    //MyCity *endCity = nullptr;
    if (itr->second->validated_route.empty())
    {
      itr++;
      continue;
    }
    else if (itr->second->validated_route[0]->city1->is_factory)
    { // Find the initial city, should be a factory
      sourceCity = itr->second->validated_route[0]->city1;
    }
    else
    {
      sourceCity = itr->second->validated_route[0]->city2;
    }
    unsigned int currDay = 0;

    // iterating through the routes to get from factory to that city
    for (auto leg : itr->second->validated_route)
    {
      queue.push(create_ship(vaccinesNeeded, leg->id, currDay, sourceCity->name)); // Create and add Shipment
      currDay += leg->days;                                                        //Update day of arrival

      //Change source city
      if (leg->city1 == sourceCity)
        sourceCity = leg->city2;
      else
        sourceCity = leg->city1;
    }
    // Udating days to spare
    itr->second->daysToSpare = mDeadline - currDay;
    ++itr;
  }
  // Each leg should now be added to the Shipment vector

  //std::sort(result.begin(), result.end(), sortID);
  // Combine possible routes

  while (!queue.empty())
  {
    Shipment curr = queue.top();
    queue.pop();

    Shipment next;
    do
    {
      if (queue.empty())
        break;
      next = queue.top();
      if (next.route_id == curr.route_id && next.day == curr.day && next.source == curr.source)
      {
        queue.pop();
        curr.doses += next.doses;
        if (queue.empty())
          break;
      }
    } while (next.route_id == curr.route_id && next.day == curr.day && next.source == curr.source);
    result.push_back(curr);
  }
  /* auto shipItr = result.begin();
  while (shipItr != result.end())
  {
    unsigned int shipId = shipItr->route_id;
    auto dupeItr = shipItr;
    dupeItr++;
    while (dupeItr->route_id == shipId && dupeItr != result.end())
    {
      if (shipItr->day == dupeItr->day && shipItr->route_id == dupeItr->route_id && shipItr->source == dupeItr->source)
      {
        shipItr->doses += dupeItr->doses;
        dupeItr = result.erase(dupeItr);
      }
      else
        dupeItr++;
    }
    shipItr++;
  } */
  /* for (auto res : result)
  {
    std::cout << "day " << res.day << " id " << res.route_id << " source " << res.source << " doses " << res.doses << std::endl;
  } */
  return result;
}