CXXFLAGS = -g -std=c++17 -Wall -Wextra -Werror
CXX      = g++

# These targets don't create any files:
.PHONY: callcenter clean

# Magic Variables!
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#  $@ is the name of the target
#  $+ is a list of all the dependencies
#  $< is the first dependency

callcenter: main.o MyCallCenter.o Validator.o
	${CXX} $(CXXFLAGS) -o $@ $+


main.o: main.cpp CallCenter.h Validator.h Objects.h
	${CXX} $(CXXFLAGS) -c -o $@ $<

MyCallCenter.o: MyCallCenter.cpp MyCallCenter.h CallCenter.h Objects.h
	${CXX} $(CXXFLAGS) -c -o $@ $<

Validator.o: Validator.cpp Validator.h Objects.h
	${CXX} $(CXXFLAGS) -c -o $@ $<


clean:
	rm -f callcenter main.o MyCallCenter.o Validator.o
