#include "NoodleShop.h"
#include "Validator.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <stdexcept>
#include <iterator>

// Global for creating unique IDs.
unsigned int order_id = 0;

// Random number generator initialization.
std::random_device rd;
std::mt19937 gen(rd());

// Helper function to parse noodle information.
std::vector<Noodle> parse_noodles(std::istream &stream, unsigned int n)
{
    std::string line;
    std::vector<Noodle> noodles;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (!std::getline(stream, line))
        {
            throw std::runtime_error("Could not read noodle line.");
        }

        Noodle n;
        std::istringstream linestream(line);
        if (!(linestream >> n.batch_size >> n.cook_time >> n.ingredient_cost >> n.serving_price >> std::ws))
        {
            throw std::runtime_error("Invalid noodle line: " + line);
        }

        std::getline(linestream, n.name);
        if (n.name == "")
        {
            throw std::runtime_error("Invalid noodle line: " + line);
        }

        noodles.push_back(n);
    }

    return noodles;
}

// Helper function to parse arrival rates and generate orders.
std::vector<Order> parse_orders(std::istream &stream, const std::vector<Noodle> &noodles)
{
    std::string line;
    if (!std::getline(stream, line)) {
        throw std::runtime_error("Could not read order line.");
    }

    std::vector<Order> orders;
    std::istringstream linestream(line);
    for (const Noodle &noodle : noodles)
    {
        float arrival_rate;
        if (!(linestream >> arrival_rate))
        {
            throw std::runtime_error("Invalid order line: " + line);
        }

        std::poisson_distribution<unsigned int> dist(arrival_rate);
        for (int i = dist(gen); i > 0; --i)
        {
            Order order;
            order.id = ++order_id;
            order.noodle = noodle.name;
            orders.push_back(order);
        }
    }

    return orders;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "USAGE: ./noodles [data-file]\n";
        std::exit(1);
    }

    // Change this to get more detailed logs!
    int verbosity = 0;

    unsigned int npots;
    unsigned int rent;
    unsigned int expect;
    unsigned int nnoodles;
    std::vector<Noodle> noodles;
    NoodleShop *shop = nullptr;

    try
    {
        std::string line;
        std::ifstream file(argv[1]);

        if (!std::getline(file, line))
        {
            throw std::runtime_error("Could not read file!");
        }

        std::istringstream metadata(line);
        if (!(metadata >> npots >> rent >> expect >> nnoodles))
        {
            throw std::runtime_error("Could not read metadata!");
        }

        noodles = parse_noodles(file, nnoodles);
        Validator validator(npots, rent, noodles, verbosity);
        shop = NoodleShop::create(npots, rent, expect, noodles);

        unsigned int minute = 0;
        while (std::getline(file, line))
        {
            std::istringstream linestream(line);
            auto in = parse_orders(linestream, noodles);
            auto ok = shop->orders(minute, in);

            std::vector<Order> accepted;
            std::set_intersection(
                in.begin(), in.end(), ok.begin(), ok.end(),
                std::inserter(accepted, accepted.begin()));

            validator.orders(minute, accepted);
            Action *action = shop->action(minute);
            validator.validate(action);
            delete action;
            minute += 1;
        }

        validator.summarize();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << "\n";
        std::exit(1);
    }

    delete shop;
    return 0;
}
