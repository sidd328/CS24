CXXFLAGS = -g -std=c++17 -Wall -Wextra -Werror
CXX      = g++

# These targets don't create any files:
.PHONY: noodleshop clean

# Magic Variables!
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#  $@ is the name of the target
#  $+ is a list of all the dependencies
#  $< is the first dependency

noodleshop: main.o MyNoodleShop.o Validator.o
	${CXX} $(CXXFLAGS) -o $@ $+


main.o: main.cpp Validator.h MyNoodleShop.h Actions.h Objects.h
	${CXX} $(CXXFLAGS) -c -o $@ $<

MyNoodleShop.o: MyNoodleShop.cpp MyNoodleShop.h Actions.h Objects.h
	${CXX} $(CXXFLAGS) -c -o $@ $<

Validator.o: Validator.cpp Validator.h Actions.h Objects.h
	${CXX} $(CXXFLAGS) -c -o $@ $<


clean:
	rm -f noodleshop main.o MyNoodleShop.o Validator.o
