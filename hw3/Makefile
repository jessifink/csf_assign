CPP_SRCS = main.cpp
CPPFLAGS = -g -Wall -Wextra -pedantic -std=c++11
%.o : %.c
	g++ $(CPPFLAGS) -cpp $<

all: main

main : main.o 
	g++ -o $@ main.o 

.PHONY: solution.zip
solution.zip :
	rm -f solution.zip
	zip -9r $@ Makefile *.cpp *.h

	clean :
	rm -f *.o main