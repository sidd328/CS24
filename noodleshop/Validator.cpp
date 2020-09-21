#include "Validator.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>

// Visitor callbacks for the Action classes:
void CleanAction::validate(Validator* validator) const {validator->validate(this);}
void  CookAction::validate(Validator* validator) const {validator->validate(this);}
void    NoAction::validate(Validator* validator) const {validator->validate(this);}
void ServeAction::validate(Validator* validator) const {validator->validate(this);}


Validator::Validator(int npots, int rent, std::vector<Noodle> noodles, int verbosity) {
  std::cout << std::fixed << std::setprecision(2);

  for(const auto& noodle: noodles) {
    NoodleInfo info;
    info.cook_time       = noodle.cook_time;
    info.batch_size      = noodle.batch_size;
    info.ingredient_cost = noodle.ingredient_cost;
    info.serving_price   = noodle.serving_price;
    info.profit          = 0;
    info.loss            = 0;

    mNoodles[noodle.name] = info;
  }

  mPots      = std::vector<PotInfo>(npots);
  mMinute    = -1;
  mVerbosity = verbosity;
  mRent      = rent;
  mIncome    = 0;
  mCoGS      = 0;
}


int Validator::cogs() const {
  return mCoGS;
}

int Validator::income() const {
  return mIncome;
}

void Validator::log(const std::string& message, int level) const {
  if(mVerbosity > level) {
    std::cout << '[' << std::setfill('0') << std::setw(2) << (mMinute + 480) / 60;
    std::cout << ':' << std::setfill('0') << std::setw(2) << (mMinute + 480) % 60;
    std::cout << "]: " << message << "\n" << std::setfill(' ');
  }
}

void Validator::orders(int minute, const std::vector<Order>& orders) {
  if(minute != ++mMinute) {
    throw std::runtime_error("Time is an illusion!");
  }

  if(minute >= 720 && !orders.empty()) {
    throw std::runtime_error("Orders received after eight PM!");
  }

  if(orders.size() == 0) {
    return;
  }

  const char* s = (orders.size() == 1)? "" : "s";
  log(std::to_string(orders.size()) + " new order" + s + ".", 1);

  for(const auto& order: orders) {
    auto itr = mNoodles.find(order.noodle);
    if(itr == mNoodles.end()) {
      throw std::runtime_error("No such noodle: " + order.noodle);
    }

    if(mVerbosity > 2) {
      std::cout << " - " << order.noodle << "\n";
    }

    OrderInfo info;
    info.id     = order.id;
    info.noodle = order.noodle;
    info.minute = minute;

    NoodleInfo& noodle = itr->second;
    noodle.orders.push(info);
    mOrders[order.id] = info;
  }
}

int Validator::rent() const {
  return mRent;
}

void Validator::summarize() const {
  for(const auto& pair: mNoodles) {
    if(!pair.second.orders.empty()) {
      throw std::runtime_error("Unfulfilled orders for " + pair.first + ".");
    }
  }

  std::cout << "      Profit        Loss         Net  Noodle\n";
  std::cout << std::fixed << std::setprecision(2);
  for(const auto& pair: mNoodles) {
    const NoodleInfo& n = pair.second;
    std::cout << std::setw(12) << float(n.profit) / 100
              << std::setw(12) << float(n.loss)   / 100
              << std::setw(12) << float(n.profit - n.loss) / 100
              << "  " << pair.first << '\n';
  }

  std::cout << std::setw(12) << float(mIncome)         / 100
            << std::setw(12) << float(mCoGS)           / 100
            << std::setw(12) << float(mIncome - mCoGS) / 100
            << "  SUBTOTAL\n\n";

  std::cout << std::setw(24) << float(mRent) /  100
            << std::setw(12) << float(mRent) / -100
            << "  Rent\n";

  std::cout << std::setw(12) << float(mIncome)                 / 100
            << std::setw(12) << float(mCoGS + mRent)           / 100
            << std::setw(12) << float(mIncome - mCoGS - mRent) / 100
            << "  TOTAL\n";
}

void Validator::validate(const Action* action) {
  action->validate(this);
}

void Validator::validate(const CleanAction* action) {
  int pid = action->pot_id;
  log("You clean pot " + std::to_string(pid) + ".");

  if(pid < 0 || pid >= int(mPots.size())) {
    throw std::runtime_error("Invalid pot ID: " + std::to_string(pid));
  }

  mPots[pid].noodle = "";
}

void Validator::validate(const CookAction* action) {
  int pid = action->pot_id;
  log("You start a batch of " + action->noodle + " in pot " + std::to_string(pid) + ".");

  if(pid < 0 || pid >= int(mPots.size())) {
    throw std::runtime_error("Invalid pot ID: " + std::to_string(pid));
  }

  PotInfo& pot = mPots[pid];
  if(pot.noodle != "") {
    throw std::runtime_error("Pot " + std::to_string(pid) + " is not clean.");
  }

  auto itr = mNoodles.find(action->noodle);
  if(itr == mNoodles.end()) {
    throw std::runtime_error("No such noodle: " + action->noodle);
  }

  NoodleInfo& noodle = itr->second;
  noodle.loss += noodle.ingredient_cost;
  mCoGS += noodle.ingredient_cost;

  pot.noodle   = action->noodle;
  pot.ready_at = mMinute + noodle.cook_time;
  pot.stale_at = mMinute + noodle.cook_time + 30;
  pot.servings = noodle.batch_size;
}

void Validator::validate(const NoAction*) {
  // This action is always valid.
}

void Validator::validate(const ServeAction* action) {
  const char* s = (action->serves.size() == 1)? "" : "s";
  log("You serve " + std::to_string(action->serves.size()) + " customer" + s + ".");

  for(const Serve& serve: action->serves) {
    int pid = serve.pot_id;
    int oid = serve.order_id;

    if(pid < 0 || pid >= int(mPots.size())) {
      throw std::runtime_error("Invalid pot ID: " + std::to_string(pid));
    }

    auto itr = mOrders.find(oid);
    if(itr == mOrders.end()) {
      throw std::runtime_error("Invalid order ID: " + std::to_string(oid));
    }

    PotInfo& pot = mPots[pid];
    if(pot.noodle == "") {
      throw std::runtime_error("Pot " + std::to_string(pid) + " is empty.");
    }

    OrderInfo& order = itr->second;
    if(pot.noodle != order.noodle) {
      throw std::runtime_error("Order " + std::to_string(oid) + " wanted " + order.noodle + " but got " + pot.noodle + ".");
    }

    NoodleInfo& noodle = mNoodles[order.noodle];
    if(order.id != noodle.orders.front().id) {
      throw std::runtime_error("Order " + std::to_string(oid) + " was served before order " + std::to_string(noodle.orders.front().id) + ".");
    }

    int wait = mMinute - order.minute;
    int base = noodle.serving_price;
    int tips = 0;

    if(wait > 15) {
      base = 0;
      if(mVerbosity > 1) {
        std::cout << " - You give away some " << order.noodle << ".\n";
      }
    }
    else if(wait < 10) {
      tips = (10 - wait) * 10;
      if(mVerbosity > 1) {
        std::cout << " - You sell some " << order.noodle << " for $" << float(base) / 100 << " plus a $" << float(tips) / 100 << " tip.\n";
      }
    }
    else {
      if(mVerbosity > 1) {
        std::cout << " - You sell some " << order.noodle << " for $" << float(base) / 100 << ".\n";
      }
    }

    mIncome       += base + tips;
    noodle.profit += base + tips;
    noodle.orders.pop();
    mOrders.erase(oid);
    pot.servings -= 1;
  }
}
