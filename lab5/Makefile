# Athit Vue and Chris White
# CSCI 446
# LAB 4
# Makefile for myclient.c and myserver.c

######MAKEFILE START####################
CXX = gcc 
CXXFLAGS = -g -Wall -W -Werror -pedantic
all:	myclient	myserver
client:	myclient.c
	$(CXX) $(CXXFLAGS) -o myclient myclient.c
server:	myserver.c
	$(CXX) $(CXXFLAGS) -o myserver myserver.c
clean: 
	rm -f *.o myserver myclient
#######MAKEFILE END######################

