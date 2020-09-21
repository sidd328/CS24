#ifndef MYNOODLESHOP_H
#define MYNOODLESHOP_H

#include "NoodleShop.h"
#include <queue>
#include <stdexcept>
#include <map>



class Buffer
{
public:
    int _capacity;
    int _start;
    int _count;

    std::vector<Order> queue;

private:
    Buffer(int capacity);

    int count();
    void push(Order order);
    Order pop();
    bool empty();
    Order top();

    friend class MyNoodleShop;
};


struct Pots
{
    int pot_id;

    bool stale;

    int count;

    Pots(int id) : pot_id(id), stale(false), count(0) {}
};


class MyNoodle
{
private:
    Noodle noodle;

    Pots *pot;

    int serving_remaining;

    int cooked_minute;

    int stale_minute;

    int count;

public:
    MyNoodle(Noodle _noodle);

    void set_pot(Pots *pot);

    void set_stale();

    void set_cooked(int minute);

    friend class MyNoodleShop;
};




class MyNoodleShop : public NoodleShop
{
    int _pots;
    int _customers;
    std::vector<MyNoodle> _noodles;
    int _rent;

    Buffer *accepted_orders;

    std::map<std::string, int> noodle_map;

    std::vector<Pots*> clean_pots;
    std::vector<Pots*> dirty_pots;

public:
    MyNoodleShop(int pots, int customers, std::vector<Noodle> noodles, int rent);
    // Destructor?

    // Required Member Functions
    std::vector<Order> orders(int minute, std::vector<Order> orderlist);
    Action *action(int minute);

    CookAction *cookNoodles(int minute, MyNoodle noodle);
    ServeAction *serveNoodles();
    CleanAction *cleanPots();
    NoAction *nothing();
};

#endif
