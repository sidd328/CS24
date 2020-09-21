# Data Files

These are some scenarios to help you test your solution:

- **Stable**:  All your noodles have the same demand,  which stays constant over
  the course of the day.

- **Chef's Special**:  One of your noodles is very popular, while there's not as
  much demand for the others.

- **Lunch Rush**:  There's not much demand in the morning or evening, but you'll
  get a whole lot of customers around noon.

- **Custom Noodles**: You have plenty of pots, but one batch of these noodles is
  only one serving.  Optimize your serving.

- **Breakfast,  Lunch,  and Dinner**:  Different types of noodles are popular at
  different times of the day.


## File Format

All the data files start with the following integer fields on the first line:

- `npots` is the number of pots you have available.
- `rent` is the amount you paid to rent your stall for the day, in cents.
- `expect` is the number of customers you expect for the day.
- `nnoodles` is the number of noodle types you serve.

The next `nnoodle` lines describe your noodles.  Each line contains:

- `batch_size` is the number of servings you get from one batch.
- `cook_time` is the number of minutes this noodle needs to cook.
- `ingredient_cost` is the amount you pay to start a batch of this noodle, in cents.
- `serving_price` is the amount you earn per serving of this noodle, in cents.

Finally there are 750 lines, one for each minute of your workday.  Each line has
one number per noodle type.  This number is the  average arrival rate for orders
of that noodle type at that minute. The autograder will select the actual number
of arrivals from a Poisson distribution based on this rate.
