// main.cpp
// Barnes , Tyler
// trbarnes

#include<iostream>
# include<limits.h>
#include<list>
#include<vector>
#include<algorithm>
# define INF INT_MAX
using namespace std;

// Node struct to store x and y coordinates and terrain
struct Node{
	int x_coordinate;
	int y_coordinate;
	char terrain;
	// The value of the the terrain in the node
	int terrain_value = 0;
	// The cost of getting from the start to that node
	int gScore = 10000;
	// The cost of getting from the current node to the end ~
	int fScore = 10000;
	// Keeps track of the previous node
	Node* came_from = NULL;
	// Keeps track of if in closed set
	bool in_closed_set = false;
	// Keeps track of if in open set
	bool in_open_set = false;

	// Default constructor for my nodes
	Node(int x, int y, char t);
};

// Default contructor to set up Nodes
Node::Node(int x, int y, char t) {
	x_coordinate = x;
	y_coordinate = y;
	terrain = t;
}

// Functions that setup A*-Search
// Takes in the input values for the nodes in the grid
void gridInput(Node* ** grid);
// Deletes the grid after the search has finished
void deleteGrid(Node* ** grid);
// Heuristic function to calculate Manhatten distance
int heuristic(int start_x, int start_y, int goal_x, int goal_y);
// A search function to find the  shortest path
bool ASearch(Node* ** grid, int start_x, int start_y, int goal_x, int goal_y);
// ReconstructPath function to format output
void ReconstructPath(Node* current, Node* ** grid, int start_x, int goal_x,
	int start_y, int goal_y);
// Function to get the neighbors of the current node
vector<Node*> getNeighbor(Node* current, Node* ** grid);
// Distance function to calculate scores
int Dist(Node* neighbor);

// Global variables that will be needed throughout the program
int width;
int height;

int main(int argc, char* argv[]) {
	// Taking the first two command line arguments for width and height
	width = atoi(argv[1]);
	height = atoi(argv[2]);
	// Defaulting command line values that may or may not be set
	int start_x = 0;
	int start_y = 0;
	int goal_x = width - 1;
	int goal_y = height - 1;
	// If start coordinates are given
	if (argc > 3) {
		 start_x = atoi(argv[3]);
		 start_y = atoi(argv[4]);
	}
	// If goal coordinates are given
	if (argc > 5) {
		 goal_x = atoi(argv[5]);
		 goal_y = atoi(argv[6]);
	}
	// Allocating memory for the array (named grid)
	Node* ** grid = new Node**[width];
	for (int i = 0; i < width; ++i) {
		grid[i] = new Node*[height];
	}
	gridInput(grid);
	ASearch(grid, start_x, start_y, goal_x, goal_y);

	deleteGrid(grid);
	return 0;
}

void deleteGrid(Node* ** grid) {
	// De-Allocating memory used for the grid
	for (int i = 0; i < height; i++) {
		delete [] grid[i];
	}
	delete [] grid;
}

// Takes in the input and stores it into the grid
void gridInput(Node* ** grid) {
	// Creating a node at each location in the grid
	char terrainInput;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			// Taking in the input terrain for each node
			cin >> terrainInput;
			// Calling the default contructor to fill our grid
			grid[i][j] = new Node(i,j, terrainInput);
			// Setting the costs for each terrain value for the nodes
			if (grid[i][j]->terrain == '.') {
				grid[i][j]->terrain_value = 1;
			} else if (grid[i][j]->terrain == '*') {
				grid[i][j]->terrain_value = 3;
		 	} else if (grid[i][j]->terrain == 'o') {
				grid[i][j]->terrain_value = 5;
			} else if (grid[i][j]->terrain == '~') {
				grid[i][j]->terrain_value = 7;
			} else if (grid[i][j]->terrain == 'x') {
				grid[i][j]->terrain_value = 10000;
			}
		}
	}
}

void ReconstructPath(Node* current, Node* ** grid, int start_x, int goal_x,
	int start_y, int goal_y) {
	// Until we have backtracked all the way to the starting node
	while (current->came_from != NULL) {
		// Seting the directions based on where the previous node was
		if (current->came_from->x_coordinate < current->x_coordinate) {
			current->came_from->terrain = '>';
		} else if (current->came_from->x_coordinate > current->x_coordinate) {
			current->came_from->terrain = '<';
		} else if (current->came_from->y_coordinate < current->y_coordinate) {
			current->came_from->terrain = 'v';
		} else if (current->came_from->y_coordinate > current->y_coordinate) {
			current->came_from->terrain = '^';
		}
		current = current->came_from;
	}
	// Setting the start and goal nodes
	grid[start_x][start_y]->terrain = 'S';
	grid[goal_x][goal_y]->terrain = 'G';
	// Outputting the terrain grid
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			cout << grid[i][j]->terrain;
		}
		cout << endl;
	}
}

// Returns the cost to get to the neighbor from the current node
int Dist(Node* neighbor) {
	return neighbor->terrain_value;
}

int heuristic(int start_x, int start_y, int goal_x, int goal_y) {
	// Returns Manhatten distance of current spot to end
	return (abs(start_x - goal_x) + abs(start_y - goal_y));
}

vector<Node*> getNeighbor(Node* current, Node* ** grid) {
	vector<Node*> neighbors;
	// If the neighbors are within the grid then add them to the closed_set
	if (current->y_coordinate + 1 < height) {
		neighbors.push_back(grid[current->x_coordinate][current->y_coordinate+ 1]);
	}
	if (current->y_coordinate - 1 >= 0) {
		neighbors.push_back(grid[current->x_coordinate][current->y_coordinate - 1]);
	}
	if (current->x_coordinate + 1 < width) {
		neighbors.push_back(grid[current->x_coordinate + 1][current->y_coordinate]);
	}
	if (current->x_coordinate - 1 >= 0) {
		neighbors.push_back(grid[current->x_coordinate - 1][current->y_coordinate]);
	}
	return neighbors;
}

auto sortLambda = [] (const Node* n1, const Node* n2) -> bool
	 {
			return n1->fScore < n2->fScore;
	 };

bool ASearch(Node* ** grid, int start_x, int start_y, int goal_x, int goal_y) {
	int score;
	Node* current;
	vector<Node*> neighbor;
	// The set of nodes already evaluated
	list<Node*> closed_set;
	// The set of currently discovered nodes that are not evaluated yet
	list<Node*> open_set (1, grid[start_x][start_y]);
	grid[start_x][start_y]->in_open_set = true;
	// Setting the starting gScore and fScore
	grid[start_x][start_y]->gScore = 0;
	grid[start_x][start_y]->fScore = heuristic(start_x,start_y,goal_x,goal_y);
	// Until there are no discovered nodes left
	while ((int)open_set.size() > 0) {
		// Set the current to the node with lowest fScore
		open_set.sort(sortLambda);
		current = open_set.front();
		// If you have reached the goal then create a path
		if (current == grid[goal_x][goal_y]) {
			ReconstructPath(current, grid, start_x, goal_x, start_y, goal_y);
		}
		// Removing the curret node from the open set and tracking that
		open_set.remove(current);
		current->in_open_set = false;
		// Adding the current node to the closed set and tracking that
		closed_set.push_back(current);
		current->in_closed_set = true;
		// Getting the neighbors of the current node
		neighbor = getNeighbor(current, grid);
		// For each neighbor of the current node
		reverse(neighbor.begin(), neighbor.end());
		for (int i = 0; i < (int)neighbor.size(); i++) {
			// If the neighbor has already been evaluated loop again
			if (neighbor[i]->in_closed_set) {
				continue;
			}
			// Set the score to the gscore of the current plus the disnace to its neighbor
			score = current->gScore + Dist(neighbor[i]);
			// If the neighbor is not already in the open set
			if (!neighbor[i]->in_open_set) {
				// Add it to the open set
				open_set.push_back(neighbor[i]);
			// If the neighbor is in the open set and has a lower score
			} else if (score >= neighbor[i]->gScore) {
				// Reset the loop
				continue;
			}
			// Set the neighbor's previous node to the current node
			neighbor[i]->came_from = current;
			// Update the gScore for the neighbor
			neighbor[i]->gScore = score;
			// Update the fScore for the neighbor
			neighbor[i]->fScore = neighbor[i]->gScore +
			heuristic(neighbor[i]->x_coordinate, neighbor[i]->y_coordinate, goal_x, goal_y);
		}
	}
	// If the goal was never found return false
	return false;
}
