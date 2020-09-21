# CS24
Code for CS24 Projects. Completed by me and inspired by assignments by xavierholt (Kevin Burk) from UCSB course CS24 

## Noodle Shop:
Simulates a Noodle Shop. Takes in order for certain types of noodles, also takes in data about number of pots, types of noodles, cost of ingredients and selling cost for each noodle. 
Used a Buffer Queue as well as OOP 
### Use:
To use the code you will have to first compile the .cpp file (using either the makefile provided) or by using your own compiler. 
Then, the program can be called and provided the command line arguments as such:
``` markdown
main simple.tsv
``` 

*****

## Call Center:
Simulates a day at a call center. Takes in various difficulty calls during the day given a list of employees of different skill. 
Implements a Buffer Queue as well as resource allocation in order to answer and solve calls in the least amount of time. Furthermore, used inheritance in order to make the code more organized
### Use:
To use the code you will have to first compile the .cpp file (using either the makefile provided) or by using your own compiler. 
Then, the program can be called and provided the command line arguments with the employees as the first argument followed by the way the calls are received:
``` markdown
main pyramid.tsv normal.tsv
``` 

*****

## Vaccine:
A Pandemic simulator where, certain factories have to supply vaccines to all other cities over various routes with certain travel time and loads. 
Implmented Djikstra's Algorithm on a Graph Data Structure with an Edge Set while taking into accoutn travel time, as well as 
cost per route load in order to optimize the algorithm and achieve a minimum cost for transportation given a certain time limit.
### Use:
To use the code you will have to first compile the .cpp file (using either the makefile provided) or by using your own compiler. 
Then, the program can be called and provided the command line arguments:
``` markdown
main ringsworld.tsv
```



