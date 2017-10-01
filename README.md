# NPC-Travelling-Sales-Man
Solving TSP problem by BnB, Local Search, and Approximation Algorithm
Group AE
Xun Cao, Jonathan Kho, Xian Li, Han Liu

Compile
g++ -o exec -std=c++11 TSPAll.cpp

(May use -O2 with LS1 and LS2)

Run exec −inst < filename > −alg [BnB|MSTApprox|Heur|LS1|LS2] −time < cutoff in secods > −seed < random seed >

example: ./exec -inst Atlanta.tsp -alg MSTApprox -time 5 -seed 2
----------------------------------------------------------------------
2optLS.cpp: Generate a tour given a graph using shuffle functions and
then swap 2 cities to get the changes of tour length, if the length is
shorter than the original one, it's acceptable to swap those 2
cities. We aim to find the largest changes everytime and swap those
two cities. Keep this procedure until the changes is equal to zero
then we get local optimal solution.

BnB.cpp: Branch and Bound algorithm uses DFS and MST bound for the
implementation.
	*DFS is implemented by the stack with iterated steps
	*MST bound is implemented by Prim's algorithm to calculate sum of MST,
two shortest edges weight and the length of path

FarthestInsertion.cpp: Farthes Insertion main program contains three
parts: Read Map is the part to converse the data into a adjancent
matrix. Farthest_Node selects the farthest unvisited node to the
path. Select_Node find the position in the path that should be
inserted. Insert_Node finishes the insertion step. The main function
is the FarthestInsertion.

FarthestInsertion.h: Farthest insertion.h is a head file for farthest insertion.cpp and BnB.cpp

MSTapprox.cpp: MST-Approx has two main parts: Prim's Algorithm to construct MST, and Depth First Search to find a approximate tour on MST.
  *Prim's algorithm includes sort part and add edge part. It includes these parts:
	--parseEdge: transfer coordinates of location into edges and nodes
	--MSTcompute: compute MST

  *DFS is implemented by recursively visit neighbor's node
	--DFSUtil: recursion function to travel through neighbours
	--DFS: DFS funtion to start from first nodes
	--tourweight: output data

SimulatedAnnealing.cpp: runs the local search algorithm simulated
annealing with reheating on a TSP and returns a near optimal tour

common.h: includes common structure other TSP algorithms use, including;
	--Struct City: store the map data
	--Dis_Cal function: calculate distances between cities
	--creatGraph: read in the input file and form the graph


TSPAll.cpp: solve the TSP problem with your choice of
Branch-and-Bound, Farthest Insertion, Simulated Annealing with
Reheating, Hill Climbing with 2OPT, and MST Approximiation.
