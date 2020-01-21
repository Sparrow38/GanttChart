#  Nicholas Poole
#  CS 302 - Section 1004 
#  Assignment #11

#  The program projectInfo is a project management tool that uses a
# Gantt Chart. To implement the Gantt Chart, it uses a number of
# different data structures, such as Kahn's toposort algorithm, a
# linked queue, and an adjacency list.

# *********************************************************************

OBJS	= projectInfo.o ganttUtils.o
CC	= g++ -g -Wall -pedantic -std=c++11
DEPS	= ganttUtils.h
DEPS1	= linkedQueue.h

all: projectInfo

projectInfo: $(OBJS) $(DEPS)
	$(CC) -o projectInfo $(OBJS)

projectInfo.o: projectInfo.cpp $(DEPS)
	$(CC) -c projectInfo.cpp

ganttUtils.o: ganttUtils.cpp $(DEPS1)
	$(CC) -c ganttUtils.cpp

# -----
# clean by removing object files

clean:
	rm $(OBJS)