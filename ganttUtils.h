//  Nicholas Poole
//  CS 302 - Section 1004 
//  Assignment #11

// The program projectInfo is a project management tool that uses a
// Gantt Chart. To implement the Gantt Chart, it uses a number of
// different data structures, such as Kahn's toposort algorithm, a
// linked queue, and an adjacency list.

// *********************************************************************

#include <iostream>
using namespace std;

struct graphNode{
	int vertex;
	int weight;
	graphNode * next;
};

struct frontNode{
	graphNode * head;
};

class ganttUtils
{
public:
	//constructor
	ganttUtils();
	//destructor
	~ganttUtils();
	//allocate adjacency matrix
	void addEdge(int, int, int);
	//read in graph data from file
	bool readGraph(const string);
	int getTaskCount();
	void findSlackTimes();
	void printSlackTimes() const;
	void criticalPath();
	void printCriticalPath() const;
	void topoSort();
	void printTopoSort() const;
	void findAPs();
	void printAPs() const;
	void findIndependentMilestones();
	void findDependencyStats();
	void printDependencyStats() const;
	void findNodePoint();
	void findKeyMilestone();
	void findGraphInformation();
	void printGraphInformation();
	void printGraph();
	bool isValidProject();
	bool getArguments(int, char*[], string&, bool&);
private:
	//adjacency list
	frontNode * graphList = NULL;
	//number of vertices in graph
	int mileStoneCount;
	//number of edges in graph
	int taskCount = 0;
	//graph title
	string title;
	int sourceNode;
	int endNode;
	//the first milestone with the maximum out-degree
	int nodePoint;
	//the latest milestone with the maximum out-degree
	int keyMilestone;
	//key milestone indegree
	int kmsInDegree;
	//topoSort results
	int * topoNodes = NULL;
	//Number of nodes in topoSort
	int topoCount = 0;
	//independent milestones
	int indieMSCount = 0;
	//Outdegree array;
	int * outDegs = NULL;
	//dependency statistics
	int hiIndeg = 1;
	int lowIndeg = 399999999;
	int hiInCount = 0;
	int lowInCount = 0;
	int hiOutdeg = 1;
	int lowOutdeg = 399999999;
	int hiOutCount = 0;
	int lowOutCount = 0;
	//articulation points
	bool * aps = NULL;
	double tmRatio = 0;
	double tasksDensity = 0;
	int finalTask;
	int totalDuration;
	//critical path
	int * crPath;
	//crPath count
	int crPathCount;
	//slack times array
	int * slackTimes = NULL;
	//distance array
	int * distance = NULL;
	//independent milestones
	int * independentMilestones;
	bool isCycle(int,bool[],bool[]);
	void inDegreeCalc(int*&);
	void findAPsHelper(int, bool*&, int*&, int*&, int*&, bool*&);
};