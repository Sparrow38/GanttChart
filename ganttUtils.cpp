//  Nicholas Poole
//  CS 302 - Section 1004 
//  Assignment #11

// The program projectInfo is a project management tool that uses a
// Gantt Chart. To implement the Gantt Chart, it uses a number of
// different data structures, such as Kahn's toposort algorithm, a
// linked queue, and an adjacency list.

// *********************************************************************

#include "ganttUtils.h"
#include "linkedQueue.h"
#include <iostream>
#include <fstream>
#include <string>
#include "string.h"
#include <iomanip>
using namespace std;

/*---------------------------------------
Constructor
PARAMETER(S):	N/A
POSTCONDITION(S): Sets variables and array pointers to empty states.
---------------------------------------*/
ganttUtils::ganttUtils()
{
	taskCount = 0;
	nodePoint = 0;
	hiIndeg = 1;
	lowIndeg = 399999999;
	hiInCount = 0;
	lowInCount = 0;
	hiOutdeg = 1;
	lowOutdeg = 399999999;
	hiOutCount = 0;
	lowOutCount = 0;
	taskCount = 0;
	topoNodes = NULL;
	topoCount = 0;
	indieMSCount = 0;
	outDegs = NULL;
	aps = NULL;
	tmRatio = 0;
	tasksDensity = 0;
	finalTask = 0;
	totalDuration = 0;
	crPath = 0;
	crPathCount = 0;
	slackTimes = NULL;
	distance = NULL;
	independentMilestones = 0;
}

/*---------------------------------------
Destructor
PARAMETER(S):	N/A
POSTCONDITION(S): Deletes allocated memory.
---------------------------------------*/
ganttUtils::~ganttUtils()
{
	if (graphList != NULL){
		graphNode * tempNd = NULL;
		graphNode * link = NULL;
		for (int i=0; i<mileStoneCount; i++){
			tempNd = graphList[i].head;

			while (tempNd != NULL){
				link = tempNd;
				tempNd = tempNd -> next;
				delete link;
			}
		}
		delete [] graphList;
		graphList = NULL;
	}

	if (topoNodes != NULL)
		delete [] topoNodes;

	if (outDegs != NULL)
		delete [] outDegs;

	if (aps != NULL)
		delete [] aps;

	if (crPath != NULL)
		delete [] crPath;

	if (slackTimes != NULL)
		delete [] slackTimes;

	if (distance != NULL)
		delete [] distance;

	if (independentMilestones != NULL)
		delete [] independentMilestones;

}

/*---------------------------------------
FUNCTION NAME:	addEdge
PARAMETER(S):	current vertex, destination vertex, weight between them
RETURN TYPE:	void
POSTCONDITION(S): Adds new task to adjacency list
---------------------------------------*/
void ganttUtils::addEdge(int fromVert, int toVert, int weight)
{
	//valid count checks
	if (toVert >= mileStoneCount || fromVert >= mileStoneCount || toVert < 0 
		|| fromVert < 0){
		cout << "addEdge: error, invalid vertex." << endl;
		return;
	}
	if (toVert == fromVert){
		cout << "addEdge: error, vertex to and from can not be the same." << endl;
		return;
	}

	graphNode * tempNode = graphList[fromVert].head; // Hold the previous head.
	graphList[fromVert].head = new graphNode;
	graphList[fromVert].head->vertex = toVert;
	graphList[fromVert].head->weight = weight;
	graphList[fromVert].head->next = tempNode; // Head insert
	taskCount++;

	/*
	graphNode * tempNode = graphList[fromVert].head; // Hold the previous head.
	graphList[fromVert].head = new graphNode;
	graphList[fromVert].head->vertex = toVert;
	graphList[fromVert].head->weight = weight;
	graphList[fromVert].head->next = tempNode; // Head insert
	taskCount++;
	*/

	//TEST
	//cout << "flag3" << endl;
	//cout << "toVert: " << toVert << endl;
	//cout << "fromVert: " << fromVert << endl;
	//cout << "addEdge: " << endl;
	//cout << "toVert: " << toVert << " fromVert: " << fromVert
    //    << " weight: " << weight << endl;

	//delete tempNode;
}

/*---------------------------------------
FUNCTION NAME:	readGraph
PARAMETER(S):	input file name
RETURN TYPE:	bool
POSTCONDITION(S): Checks for valid graph file,
	reads in graph data if valid.
---------------------------------------*/
bool ganttUtils::readGraph(const string graphFile)
{
	//open file, declare variables
	ifstream infile;
	infile.open(graphFile);
	if (!infile.is_open()){
		return false;
	}
	string strMilestones; 	//milestones
	string strSource; //source vertex
	string strTo;	//destination vertex
	int toVert;
	string strFrom; //previous vertex
	int fromVert;
	string strWeight; //weight
	int theWeight;

	//read title, vertex count, source vertex
	getline(infile, title);
	if(title.substr(0,6) == "title:"){
       title.erase(0,6);
   	}
	infile >> strMilestones;
	infile >> strMilestones;
	mileStoneCount = stoi(strMilestones);
	infile >> strSource;
	infile >> strSource;
	sourceNode = stoi(strSource);

	graphList = new frontNode[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++){
    	graphList[i].head = NULL;
    }

	//Write file data to graph
	taskCount = 0;
    while (infile >> strFrom >> strTo >> strWeight){
    	fromVert = stoi(strFrom);
        toVert = stoi(strTo);
        theWeight = stoi(strWeight);
        addEdge(fromVert, toVert, theWeight);
    }

	return true;
}

/*---------------------------------------
FUNCTION NAME:	getTaskCount
PARAMETER(S):	N/A
RETURN TYPE:	int
POSTCONDITION(S): Returns number of tasks in graph
---------------------------------------*/
int ganttUtils::getTaskCount()
{
	return taskCount;
}

/*---------------------------------------
FUNCTION NAME:	findSlackTimes
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates slack times
	and populates slackTimes array.
---------------------------------------*/
void ganttUtils::findSlackTimes()
{
	int v = 0;
	int time = 0;

	slackTimes = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		slackTimes[i] = -1;

	for (int i=0; i<crPathCount; i++){
		slackTimes[crPath[i]] = 0;
	}

	//TEST
	//for (int i=0; i<mileStoneCount; i++)
	//	cout << "slackTimes[" << i << "]: " << slackTimes[i] << endl;

	for (int u=0; u<mileStoneCount; u++){
		if (slackTimes[u] == -1){
			graphNode * tempNd = NULL;
			tempNd = graphList[u].head;
			while (tempNd != NULL){
				v = tempNd -> vertex;
				time = tempNd -> weight;
				slackTimes[u] = distance[v] - (distance[u] + time);
				tempNd = tempNd -> next;
			}
		}	
	}
}

/*---------------------------------------
FUNCTION NAME:	printSlackTimes
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Prints slackTimes to console.
---------------------------------------*/
void ganttUtils::printSlackTimes() const
{
	cout << "------------------------------------------------------------" << endl;
	cout << "Slack Times (task-slacktime):" << endl;

	for (int i=0; i<mileStoneCount; i++){
		if (slackTimes[i] != 0 && slackTimes[i] != -1)
			cout << " " << i << "-" << slackTimes[i];
	}

	cout << endl << endl;
}

/*---------------------------------------
FUNCTION NAME:	criticalPath
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates critical path
	in Gantt Chart.
---------------------------------------*/
void ganttUtils::criticalPath()
{
	int v = 0;
	int u = 0;
	int time = 0;
	int x = 0; //final task

	int * inDegree = NULL;
	inDegree = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		inDegree[i] = 0;
	
	distance = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		distance[i] = 0;

	inDegreeCalc(inDegree);

	linkedQueue<int> vQueue;
	for (int i=0; i<mileStoneCount; i++){
		if (inDegree[i] == 0)
			vQueue.addItem(i);
	}

	//While queue is not empty:
	while (!vQueue.isEmptyQueue()){
		//remove a vertex from the queue
		v = vQueue.front();
		vQueue.deleteItem();

		//decrease in-degree by 1 for all of its neighbors
		graphNode * tempNd = NULL;
		tempNd = graphList[v].head;

		while (tempNd != NULL){
			u = tempNd -> vertex;
			time = tempNd -> weight;

			distance[u] = max(distance[u],distance[v]+time);
			inDegree[u] -= 1;
			if (inDegree[u] == 0)
				vQueue.addItem(u);
			tempNd = tempNd -> next;
		}
	}

	//find final task, total duration
	totalDuration = 0;
	for (int i=0; i<mileStoneCount; i++){
		if (i == 0)
			totalDuration = distance[i];
		if (distance[i] > distance[x]){
			totalDuration = distance[i];
			x = i;
		}
	}

	finalTask = x;

	crPath = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		crPath[i] = -1;
	crPathCount = 0;
	crPath[crPathCount] = x;
	crPathCount++;
	while (x != sourceNode){
		for (int v=0; v<mileStoneCount; v++){
			graphNode * tempNd = NULL;
			tempNd = graphList[v].head;

			while (tempNd != NULL){
				u = tempNd -> vertex;
				time = tempNd -> weight;
				
				if (x == u){
					if (distance[x] == distance[v] + time){
						crPath[crPathCount] = v;
						crPathCount++;
						x = v;
					}
				}

				tempNd = tempNd -> next;
			}
		}
	}

	delete [] inDegree;
}

/*---------------------------------------
FUNCTION NAME:	printCriticalPath
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Prints critical path to console.
---------------------------------------*/
void ganttUtils::printCriticalPath() const
{
	cout << "------------------------------------------------------------" << endl;
	cout << "Critical Path:" << endl;
	cout << "   Source Node: " << sourceNode << endl;
	cout << "   Final Task: " << finalTask << endl;
	cout << "   Total Duration: " << totalDuration << endl;
	cout << endl;
	cout << "Critical Path: " << endl;

	for (int i=crPathCount-1; i>=0; i--){
		cout << " " << crPath[i];
	}

	cout << endl << endl;
}

/*---------------------------------------
FUNCTION NAME:	topoSort
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates topological sort
	of graph using Kahn's algorithm.
---------------------------------------*/
void ganttUtils::topoSort()
{
	if (graphList == NULL){
		cout << "topoSort: Error, no graph data." << endl;
		return;
	}

	//Perform initializations
	int * inDegree;
	//create inDegree array, topoNodes array
	inDegree = new int[mileStoneCount];
	//The topoNodes array holds the topoSort
	topoNodes = new int[mileStoneCount];
	//initialize array elements to 0
	for (int i=0; i<mileStoneCount; i++){
		topoNodes[i] = 0;
		inDegree[i] = 0;
	}
	//initialize topoCount to 0
	topoCount = 0;

	//Compute in-degree for each vertex in graph
	inDegreeCalc(inDegree);

	//TEST
	//for (int i=0; i<mileStoneCount; i++)
	//	cout << "1) inDegree[" << i << "]: " << inDegree[i] << endl;

	//En-queue each vertex with an in-degree of 0
	linkedQueue<int> vQueue;
	for (int i=0; i<mileStoneCount; i++){

		//TEST
		//cout << "2) inDegree[" << i << "]: " << inDegree[i] << endl;

		if (inDegree[i] == 0)
			vQueue.addItem(i);
	}

	//While queue is not empty:
	int popVertex = 0;
	while (!vQueue.isEmptyQueue()){
		//remove a vertex from the queue
		popVertex = vQueue.front();
		vQueue.deleteItem();

		//TEST
		//cout << "popVertex: " << popVertex << endl;

		topoNodes[topoCount] = popVertex;
		topoCount++;

		//decrease in-degree by 1 for all of its neighbors
		graphNode * tempNd = NULL;
		tempNd = graphList[popVertex].head;

		//TEST
		//cout << "popVertex: " << popVertex << endl;

		while (tempNd != NULL){
			if (tempNd -> weight != 0)
				inDegree[tempNd -> vertex]--;
			//if in-degree of a neighbor is reduced to 0, add to queue.
			if (inDegree[tempNd -> vertex] == 0)
				vQueue.addItem(tempNd -> vertex);
			tempNd = tempNd -> next;
		}
	}

	delete [] inDegree;
}

/*---------------------------------------
FUNCTION NAME:	printTopoSort
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Prints topoSort to console
---------------------------------------*/
void ganttUtils::printTopoSort() const
{
	cout << "------------------------------------------------------------" << endl;
	cout << "Topological Sort: " << endl;
	for (int i=0; i<topoCount; i++)
		cout << " " << topoNodes[i];
	cout << endl << endl << endl;;
}

/*---------------------------------------
FUNCTION NAME:	findAPs
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates articulation points
---------------------------------------*/
void ganttUtils::findAPs()
{
	//bool * aps;
	bool * visited;
	int * parent;
	int * low;
	int * discovered;

	aps = new bool[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		aps[i] = false;
	visited = new bool[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		visited[i] = false;
	parent = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		parent[i] = -1;
	low = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		low[i] = 0;
	discovered = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		discovered[i] = 0;

	for (int v=0; v<mileStoneCount; v++){
		if (!visited[v])
			findAPsHelper(v, visited, discovered, low, parent, aps);
	}

	delete [] visited;
	delete [] parent;
	delete [] low;
	delete [] discovered;
}

/*---------------------------------------
FUNCTION NAME:	printAPs
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Prints articulation points to console.
---------------------------------------*/
void ganttUtils::printAPs() const
{
	cout << "------------------------------------------------------------" << endl;
	cout << "Articulation Points:" << endl;
	for (int i=0; i<mileStoneCount; i++){
		if (aps[i] == true)
			cout << " " << i;
	}
	cout << endl << endl;
}

/*---------------------------------------
FUNCTION NAME:	findIndependentMilestones
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates independent milestones.
---------------------------------------*/
void ganttUtils::findIndependentMilestones()
{
	//TEST
	//for (int i=0; i<mileStoneCount; i++)
	//	cout << "outDegs[" << i << "]: " << outDegs[i] << endl;

	independentMilestones = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		independentMilestones[i] = 0;

	for (int i=0; i<mileStoneCount; i++){
		if (outDegs[i] == 0)
			independentMilestones[i] = i;
	}

	/*
	cout << "   Independent Milestones" << endl;
	for (int i=0; i<mileStoneCount; i++){
		//cout << "outDegs[" << i << "]: " << outDegs[i] << endl;
		if (outDegs[i] == 0)
			cout << " " << i;
	}
	cout << endl;
	*/
}

/*---------------------------------------
FUNCTION NAME:	findDependencyStats
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates dependency statistics.
---------------------------------------*/
void ganttUtils::findDependencyStats()
{
	int * inDegs = NULL; //indegrees array
	inDegs = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		inDegs[i] = 0;

	inDegreeCalc(inDegs);

	for (int i=0; i<mileStoneCount; i++){
		if (inDegs[i] > hiIndeg && i != sourceNode)
			hiIndeg = inDegs[i];
		if (inDegs[i] < lowIndeg && i != sourceNode)
			lowIndeg = inDegs[i];
		if (outDegs[i] > hiOutdeg && i != finalTask)
			hiOutdeg = outDegs[i];
		if (outDegs[i] < lowOutdeg && i != finalTask)
			lowOutdeg = outDegs[i];
	}

	for (int i=0; i<mileStoneCount; i++){
		if (inDegs[i] == hiIndeg && i != sourceNode)
			hiInCount++;
		if (inDegs[i] == lowIndeg && i != sourceNode)
			lowInCount++;
		if (outDegs[i] == hiOutdeg && i != finalTask)
			hiOutCount++;
		if (outDegs[i] == lowOutdeg && i != finalTask)
			lowOutCount++;
	}

	delete [] inDegs;
}

/*---------------------------------------
FUNCTION NAME:	printDependencyStats
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Prints dependency statistics to console.
---------------------------------------*/
void ganttUtils::printDependencyStats() const
{
	cout << "------------------------------------------------------------" << endl;
	cout << "Dependency Statistics (in-degree):" << endl;
	cout << "   Highest In-Degree: " << hiIndeg << endl;
	cout << "   Lowest In-Degree: " << lowIndeg << endl;
	cout << "   Count of Highest Degree: " << hiInCount << endl;
	cout << "   Count of Lowest Degree: " << lowInCount << endl;
	cout << endl;
	cout << "Dependency Statistics (out-degree):" << endl;
	cout << "   Highest Out-Degree: " << hiOutdeg << endl;
	cout << "   Lowest Out-Degree: " << lowOutdeg << endl;
	cout << "   Count of Highest Degree: " << hiOutCount << endl;
	cout << "   Count of Lowest Degree: " << lowOutCount << endl;
	cout << endl;
}

/*---------------------------------------
FUNCTION NAME:	findNodePoint
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates nodePoint
---------------------------------------*/
void ganttUtils::findNodePoint()
{
	//TEST
    //cout << "msCount: " << mileStoneCount << endl;

    outDegs = new int[mileStoneCount];
    for (int i=0; i<mileStoneCount; i++)
        outDegs[i] = 0;

    graphNode * tempNd = NULL;
    for (int i=0; i<mileStoneCount; i++){
        tempNd = graphList[i].head;
        while (tempNd != NULL){
            outDegs[i]++;

            //TEST
            //cout << "outDegs[" << i << "]: " << outDegs[i] << endl;

            tempNd = tempNd -> next;
        }
    }

    //TEST
    //cout << "msCount: " << mileStoneCount << endl;

    for (int i=1; i<mileStoneCount; i++){
        if (outDegs[i] > outDegs[nodePoint])
            nodePoint = i;
    }

    //TEST
    //cout << "Test" << endl;

    //cout << "   Node Point: " << nodePoint
    //<< ", out-degree: " << outDegs[nodePoint] << " tasks" << endl;
}

/*---------------------------------------
FUNCTION NAME:	findKeyMilestone
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates key milestone
---------------------------------------*/
void ganttUtils::findKeyMilestone()
{
	int keyMileStone = 0;
	int * inDegs = NULL; //indegrees array
	inDegs = new int[mileStoneCount];
	for (int i=0; i<mileStoneCount; i++)
		inDegs[i] = 0;

	inDegreeCalc(inDegs);

	for (int i=0; i<mileStoneCount; i++){
		if (inDegs[i] >= inDegs[keyMileStone])
			keyMileStone = i;
	}

	keyMilestone = keyMileStone;
	kmsInDegree = inDegs[keyMileStone];

	//cout << "   Key Milestone: " << keyMileStone 
	//<< ", in-degree: " << inDegs[keyMileStone] << " tasks" << endl;

	delete [] inDegs;
}

/*---------------------------------------
FUNCTION NAME:	findGraphInformation
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Calculates graph information.
---------------------------------------*/
void ganttUtils::findGraphInformation()
{
	double tasks = getTaskCount();
	tmRatio = tasks/mileStoneCount;
	tasksDensity = (2*tasks)/(mileStoneCount*(mileStoneCount-1));
}

/*---------------------------------------
FUNCTION NAME:	printGraphInformation
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Prints graph info to console.
---------------------------------------*/
void ganttUtils::printGraphInformation()
{
	cout << fixed << setprecision(6);
	cout << "------------------------------------------------------------" << endl;
	cout << "Graph Information" << endl;
	cout << "   Project title: " << title << endl;
	cout << "   Milestone Count: " << mileStoneCount << endl;
	cout << "   Task Count: " << getTaskCount() << endl;
	cout << "   Source Milestone: " << sourceNode << endl;
	cout << "   Tasks/Milestones Ratio: " << tmRatio << endl;
	cout << "   Project Tasks Density: " << tasksDensity << endl;
	cout << endl;
	cout << "   Key Milestone: " << keyMilestone 
	<< ", in-degree: " << kmsInDegree << " tasks" << endl;
	cout << "   Node Point: " << nodePoint
	<< ", out-degree: " << outDegs[nodePoint] << " tasks" << endl;
	cout << "   Independent Milestones" << endl;
	for (int i=0; i<mileStoneCount; i++){
		if (independentMilestones[i] != 0)
			cout << " " << independentMilestones[i];
	}
	cout << endl << endl;

}

/*---------------------------------------
FUNCTION NAME:	printGraph
PARAMETER(S):	N/A
RETURN TYPE:	void
POSTCONDITION(S): Prints Gantt chart to console.
---------------------------------------*/
void ganttUtils::printGraph()
{
	int v = 0;
	int u = 0;

	cout << "------------------------------------------------------------" << endl;
	cout << "Graph Adjacency List:" << endl;
	cout << "   Title: " << title << endl;
	cout << endl;

	cout << "Vertex    ";
	for (int i=0; i<3; i++)
		cout << "vrt /weight | ";
	cout << "..." << endl;
	cout << "------    " 
	<< "----------------------------------------------" << endl;

	graphNode * tempNd = NULL;
	for (int i=0; i<mileStoneCount; i++){
		cout << setw(6) << i << " ->";
		tempNd = graphList[i].head;

		if (tempNd != NULL){
			while (tempNd != NULL){
				v = tempNd -> vertex;
				u = tempNd -> weight;
				cout << setw(5) << v << "/" << setw(6) << u << " |";
				tempNd = tempNd -> next;
			}
		}
		else{
			cout << "    None" << endl;
			continue;
		}

		cout << " " << endl;
	}

	cout << endl;
}

/*---------------------------------------
FUNCTION NAME:	isValidProject
PARAMETER(S):	N/A
RETURN TYPE:	bool
POSTCONDITION(S): Determines validity of graph data.
---------------------------------------*/
bool ganttUtils::isValidProject()
{
	bool * visited;
    visited = new bool[mileStoneCount];
    bool * marked;
    marked = new bool[mileStoneCount];

    for (int i=0; i<mileStoneCount; i++){
    	visited[i] = false;
    	marked[i] = false;
    }

    for (int i=0; i<mileStoneCount; i++){
    	if (!visited[i]){
	        if (isCycle(i, visited, marked)){
	        	delete [] visited;
	        	delete [] marked;
	            return false;
	        }
    	}
    }

    delete [] visited;
    delete [] marked;
    return true;
}

/*---------------------------------------
FUNCTION NAME:	isCycle
PARAMETER(S):	vertex, visited array, marked array
RETURN TYPE:	bool
POSTCONDITION(S): Determines whether graph is cyclical
---------------------------------------*/
bool ganttUtils::isCycle(int v, bool visited[], bool marked[])
{
	int u = 0;

    if (!visited[v]){
        visited[v] = true;
        marked[v] = true;
        graphNode * tempNd = graphList[v].head;
        while (tempNd != NULL){
            u = tempNd -> vertex;
            if (isCycle(u, visited, marked)){
                return true;
            }
            else if (marked[u])
                return true;
            tempNd = tempNd -> next;
        }
        marked[v] = false;
    }
    //marked[v] = false;
    return false;
}

/*---------------------------------------
FUNCTION NAME:	getArguments
PARAMETER(S):	arg count, arg vector, input file name, print spec
RETURN TYPE:	bool
POSTCONDITION(S): Determines validity of command line arguments,
	processes arguments if valid.
---------------------------------------*/
bool ganttUtils::getArguments(int argc, char * argv[], string& graphFile, bool& showGraph)
{
	if (argc == 1){
		cout << "Usage: ./projectInfo -f <fileName> [-p]" << endl;
		return false;
	}

	bool fSpec = false; //graphFile spec
	bool pSpec = false; //showGraph spec
	int fileV;

	if (argc < 3 || argc > 4){
		cout << "Error, invalid command line options." << endl;
		return false;
	}

	for (int i=1; i<argc; i++){
		if (fSpec != true && strcmp(argv[i], "-f") == 0){
			fSpec = true;
			fileV = i+1;
		}
		if (argc == 4 && pSpec != true && strcmp(argv[i], "-p") == 0){
			pSpec = true;
			showGraph = true;
		}
	}

	if (fSpec == false){
		cout << "Error, invalid project file name specifier." << endl;
		return false;
	}

	if (pSpec == false && argc > 3){
		cout << "Error, invalid print specifier." << endl;
		return false;
	}

	graphFile = argv[fileV];

	ifstream infile;
	infile.open(graphFile);
	if (!infile.is_open()){
		cout << "Error, can not find project file." << endl;
		return false;
	}

	infile.close();

	return true;
}

/*---------------------------------------
FUNCTION NAME:	inDegreeCalc
PARAMETER(S):	inDegree array
RETURN TYPE:	void
POSTCONDITION(S): Calculates indegrees of vertices,
	returns data in array by reference.
---------------------------------------*/
void ganttUtils::inDegreeCalc(int*& inDegs)
{
	//cout << "msCount: " << mileStoneCount << endl;

	graphNode * tempNd = NULL;
	for (int i=0; i<mileStoneCount; i++){
		tempNd = graphList[i].head;
		while (tempNd != NULL){
			inDegs[tempNd -> vertex]++;

			//TEST
			//cout << "inDeg[" << i << "]: " << inDegs[i] << endl;

			tempNd = tempNd -> next;
		}
	}
}

/*---------------------------------------
FUNCTION NAME:	findAPsHelper
PARAMETER(S):	vertex, visited, discovered, low, parent, articulation points arrays
RETURN TYPE:	void
POSTCONDITION(S): Calculates articulation points.
---------------------------------------*/
void ganttUtils::findAPsHelper(int v, bool*& visited, int*& discovered, 
	int*& low, int*& parent, bool*& aps)
{
	int children = 0;
	int time = 0;
	int u = 0;

	visited[v] = true;

	discovered[v] = low[v] = ++time;

	graphNode * tempNd = graphList[v].head;
		while (tempNd != NULL){
			u = tempNd -> vertex;
			
			if (!visited[u]){
				children++;
				parent[u] = v;
				findAPsHelper(u, visited, discovered, low, parent, aps);
				low[v] = min(low[v],low[u]);
				if(parent[v] == -1 and children > 1)
					aps[v] = true;
				if (parent[v] != -1 and low[u]>=discovered[u])
					aps[v] = true;
			}

			else if (v != parent[v])
				low[v] = min(low[v], discovered[u]);

			tempNd = tempNd -> next;
		}
}