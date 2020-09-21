#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Objects.h"
#include "Actions.h"

#include <map>
#include <queue>
#include <vector>

class Validator {
  struct PotInfo {
    std::string noodle;
    int         ready_at;
    int         stale_at;
    int         servings;
  };

  struct OrderInfo {
    int         id;
    std::string noodle;
    int         minute;
  };

  struct NoodleInfo {
    int cook_time;
    int batch_size;
    int ingredient_cost;
    int serving_price;

    int profit;
    int loss;

    std::queue<OrderInfo> orders;
  };

private:
  std::map<std::string, NoodleInfo> mNoodles;
  std::map<int, OrderInfo> mOrders;
  std::vector<PotInfo> mPots;

  int mCoGS;
  int mIncome;
  int mRent;
  int mMinute;
  int mVerbosity;

public:
  Validator(int npots, int rent, std::vector<Noodle> noodles, int verbosity);

  int cogs()   const;
  int income() const;
  int rent()   const;

  void log(const std::string& message, int level = 0) const;
  void orders(int minute, const std::vector<Order>& orders);
  void summarize() const;

  void validate(const Action*      action);
  void validate(const CookAction*  action);
  void validate(const CleanAction* action);
  void validate(const ServeAction* action);
  void validate(const NoAction*    action);
};

#endif
