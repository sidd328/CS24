# Noodle Shop

_Due Monday, August 10th, before midnight Pacific Time._

**Note:**  You can work on this assignment with a partner if you want to. If you
do,  the two of you should make  one submission  on Gradescope with both of your
names listed.  You'll need to do this every time you submit.


## Backstory

If there's one thing this town lacks it's quality noodles.  So you quit your job
as a programmer and set out to fix that. You buy a whole bunch of pots, you rent
a stall at a nearby farmers' market, and you hang out your sign:

<p align="center">
<strong>Fresh Hot Noodles</strong><br/>
(in fifteen minutes or they're free)
</p>

Now all you have to do  is cook some noodles and wait for customers!  And figure
out how to pay that rent...


## Your Assignment

- Write a class that simulates running a noodle shop.
- Make at least enough money to pay for your rent and ingredients.
- You must serve all the customers whose orders you accept.
- You can use anything from the standard library.
- Make sure you don't print anything that isn't explicitly allowed.
- Make sure you don't have any memory leaks.


## Your Day

Your day starts at eight in the morning, and is divided into minutes. You accept
orders until  eight in the evening,  and you keep your kitchen  open until eight
thirty to handle any orders that came in right before eight.

Every minute, the framework code will call two functions on your shop:

- First it will call  `orders(minute, orderlist)`  to let you know about any new
  orders coming in. Return a list of all the orders you choose to accept. If you
  want to reject an order you must do it here: there's no way to cancel an order
  that you previously accepted.

- Then it will call `action(minute)` to ask what you want in that minute. Return
  a new  `Action`  describing what you choose to do.  Your available actions are
  described in the Making Noodles section, below.

The `orders()` function  will be still called between eight and eight thirty PM,
but there will never be any new orders during this time.


## Making Noodles

To make noodles, all you need is a pot and ingredients! You have the ingredients
to make any number of meals, but you have a limited number of pots.  In order to
make money, you'll need to manage your pots carefully;  the actions you can take
all relate to your pots.  All actions take one minute.

- Take the **cook** action to start a batch of noodles in a clean pot.
  - This action will generate `batch_size` servings of noodles.
  - The noodles will be ready to serve in `cook_time` minutes.
  - The noodles will go stale half an hour after they're ready.
  - The pot you use is now dirty.

- Take the  **serve**  action to take noodles out of pots and  give them to your
  customers (this is how  you get paid!).  You can serve any number of customers
  with a single serve action.

- Take the  **clean**  action to clean a pot.  If there are any noodles still in
  the pot, they will be discarded.  All your pots are initially clean.

- If you don't want to do any of these things, you can take no action.


## Making Money

You rent your stall by the day. The amount you pay for the current day is passed
to the create function. You'll want to make as much money as possible regardless
of how much you paid, but you'll have the number for reference.

You have all your ingredients already, but for accounting purposes  you only pay
for your ingredients when you use them.  You'll pay a noodle's `ingredient_cost`
when you start cooking a new batch of that noodle.

Customers pay you the noodle's  `serving_price`  when they get their noodles. If
they don't have to wait very long they'll give you a tip:

- Customers  who get their noodles the same minute they order them will give you
  a one dollar (100 cent) tip.
- The tip decreases by ten cents per minute until it reaches zero. Customers who
  wait ten to fifteen minutes will pay for their noodles, but won't tip.
- Customers who wait more than fifteen minutes get their noodles free.


## Invalid Actions

There are some things you will never do, because you have standards:

- You will never use more pots than you have.
- You will never cook noodles in a dirty pot.
- You will never serve anyone stale noodles.
- You will never serve anyone undercooked noodles.
- You will never serve anyone the wrong type of noodle.
- You will never fulfill orders for the same type of noodle out of order.
- You will never fail to fulfill an order you accepted.
- You will never fulfill an order more than once.


## Extra Credit

To get full credit,  don't perform  any invalid actions,  fulfill all the orders
you accept,  and make enough money to pay for  your rent and any ingredients you
use.  To get extra credit,  make as much money as you can!  The best  submission
will get 10 extra points, the next best will get 9, and so on.
