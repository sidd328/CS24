#include "MyNoodleShop.h"


//Buffer Class functions
Buffer::Buffer(int capacity)
{
    _count = 0;
    _capacity = capacity;
    _start = 0;
    queue.clear();
}

int Buffer::count()
{
    return _count;
}

void Buffer::push(Order order)
{
    if (_count == _capacity)
    {
        throw std::overflow_error("Full");
    }

    int i = (_start + _count) % _capacity;
    queue[i] = order;
    _count += 1;
}

Order Buffer::pop()
{
    if (_count == 0)
    {
        throw std::underflow_error("Empty");
    }
    Order popped = queue[_start];
    _count -= 1;
    _start = (_start + 1) % _capacity;
    return popped;
}

Order Buffer::top()
{
    if (_count == 0)
    {
        throw std::underflow_error("Empty");
    }
    Order popped(queue[_start]);
    return popped;
}

bool Buffer::empty()
{
    if (_count == 0)
        return true;
    return false;
}


//MyNoodle Class functions
MyNoodle::MyNoodle(Noodle _noodle)
{
    noodle = _noodle;

    pot = nullptr;
    cooked_minute = -1;
    stale_minute = -1;
    serving_remaining = 0;
    count = 0;
}

void MyNoodle::set_pot(Pots *_pot)
{
    pot = _pot;
}

void MyNoodle::set_cooked(int minute)
{
    cooked_minute = minute + noodle.cook_time;
}

void MyNoodle::set_stale()
{
    stale_minute = cooked_minute + 30;
}



//MyNoodleShop class functions
MyNoodleShop::MyNoodleShop(int pots, int customers, std::vector<Noodle> noodles, int rent)
{
    _pots = pots;
    _customers = customers;
    _rent = rent;
    for (Noodle item : noodles)
    {
        int index = 0;
        MyNoodle new_noodle(item);
        _noodles.push_back(new_noodle);
        noodle_map.insert(std::pair<std::string, int>(item.name, index));
        index +=1;
    }

    accepted_orders = new Buffer(customers);

    clean_pots.resize(pots);
    dirty_pots.resize(pots);

    for (int i = 0; i <= pots - 1; i++)
    {
        Pots *new_pot = new Pots(i);
        clean_pots.push_back(new_pot);
    }
}

NoodleShop *NoodleShop::create(int npots, int rent, int customers, std::vector<Noodle> noodles)
{
    return new MyNoodleShop(npots, customers, noodles, rent);
}





CookAction *MyNoodleShop::cookNoodles(int minute, MyNoodle noodle)
{
    Pots *_pot = clean_pots[0];
    clean_pots.erase(clean_pots.begin() + 0);
    int id = _pot->pot_id;
    dirty_pots.push_back(_pot);
    std::string type = noodle.noodle.name;

    auto it = noodle_map.find(type);
    int index = it->second;

    _noodles[index].cooked_minute = minute + _noodles[index].noodle.cook_time;
    _noodles[index].stale_minute = _noodles[index].cooked_minute + 30;
    _noodles[index].pot = _pot;

    return new CookAction(id, type);
}

ServeAction *MyNoodleShop::serveNoodles()
{
    std::vector<Serve> serves;
    for (Order item : accepted_orders->queue)
    {
        std::string type = item.noodle;
        auto it = noodle_map.find(type);
        int index = it->second;
        if (_noodles[index].serving_remaining > 0)
        {
            Serve serving;
            serving.pot_id = _noodles[index].pot->pot_id;
            serving.order_id = item.id;
            serves.push_back(serving);
            _noodles[index].serving_remaining -= 1;
            _noodles[index].pot->count -= 1;
        }
    }
    return new ServeAction(serves);
}

CleanAction *MyNoodleShop::cleanPots()
{
    for (Pots *item : dirty_pots)
    {
        if (item->count == 0)
        {
            Pots *pot = item;
            for (int i = 0; (unsigned)i<= dirty_pots.size() -1; i++)
            {
                if (pot = dirty_pots[i])
                {
                    dirty_pots.erase(dirty_pots.begin() + i);
                }
            }
            clean_pots.push_back(pot);
            return new CleanAction(item->pot_id);
        }
        else if (item->stale == true)
        {
            Pots *pot = item;
            for (int i = 0; i<= dirty_pots.size() -1; i++)
            {
                if (pot = dirty_pots[i])
                {
                    dirty_pots.erase(dirty_pots.begin() + i);
                }
            }
            clean_pots.push_back(pot);
            return new CleanAction(item->pot_id);
        }
    }
}

NoAction *MyNoodleShop::nothing()
{
    return new NoAction();
}


std::vector<Order> MyNoodleShop::orders(int minute, std::vector<Order> orderlist)
{
    for (Order item : orderlist)
    {
        auto it = noodle_map.find(item.noodle);
        int index = it->second;
        _noodles[index].count++;
        accepted_orders->push(item);
    }
    for (MyNoodle item : _noodles)
    {
        if (item.cooked_minute == minute)
        {
            item.serving_remaining += item.noodle.batch_size;
            item.pot->count += item.noodle.batch_size;
        }
        if (item.stale_minute == minute)
        {
            item.serving_remaining = 0;
            item.pot->stale = true;
        }
    }
    return orderlist;
}

Action *MyNoodleShop::action(int minute)
{
    if (!accepted_orders->empty())
    {
        for (MyNoodle item : _noodles)
        {
            if (item.count > 0)
            {
                if (item.serving_remaining == 0)
                {
                    if (item.cooked_minute < minute)
                    {
                        if (clean_pots.size() > 0)
                        {
                            return cookNoodles(minute, item);
                        }
                    }
                }
            }
        }
        for (MyNoodle item : _noodles)
        {
            if (item.count > 0)
            {
                if (item.serving_remaining > 0)
                {
                    return serveNoodles();
                }
            }
        }

    }

    if (!dirty_pots.empty())
    {
        return cleanPots();
    }

    return nothing();
}

// MyNoodleShop Member Functions
