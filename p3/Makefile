# Athit Vue
# Chris White
# CSCI 446
# Spring 2016
# Makefile for Program 2: t-counter
CXX = gcc
CXXFLAGS = -g -Wall -W -Werror -pedantic
all:	client server
client: client.c
	$(CXX) $(CXXFLAGS) -o client client.c
server: server.c
	$(CXX) $(CXXFLAGS) -o server server.c
clean:
	rm -f *.o client
	rm -f *.o server
