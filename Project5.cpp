#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

void printBits(unsigned char s) {
	//print the bit pattern
	for (int i = 0; i < 8; i++)
		if (s & (1 << (7 - i)))
			cout << 1;
		else
			cout << 0;
}

class BitMatrixGraph {
protected:
	unsigned char** bitAdjacencyMatrix; // the unsigned chararcter matrix
	int n; //number of nodes in the graph
	void copy(const BitMatrixGraph& otherGraph); // copy method used by copy constructor and operator=
	void deleteRows(); // this method is used in the deconstructor
public:
	BitMatrixGraph();//default constructor
	BitMatrixGraph(int numNodes);//constructor given number of nodes
	BitMatrixGraph(const BitMatrixGraph& otherGraph);//copy constructor, uses the copy method
	~BitMatrixGraph();//deconstructor
	void display(ostream& s);//display method displaying in edge list format, used by the ostream operator<<
	//ostream operator
	friend ostream& operator<<(ostream& s, BitMatrixGraph& b) {
		b.display(s);
		return(s);
	}
	//overloaded = operator
	void operator=(const BitMatrixGraph& otherGraph);
	void addEdge(int u, int v);//add edge method given two nodes
	bool isAnEdge(int u, int v);//check if there is an edge between two nodes
	//other that you want and will be useful
	void BFS(int p);//breadth first search method, prints out parent array
	//void depthFirstSearch(int p);//method that calls the recursive method DFS
	void DFS(int p);//recursive method that helps the depthFirstSearch method
};

BitMatrixGraph::BitMatrixGraph() {
	int n = 10;//default number of vertices
	int size;//created to account for the unsigned char
	// Create the bit Adjacency Matrix structure
	//start by finding the number of unsigned char needed per row
	if (n % 8 == 0)
		size = n / 8;
	else
		size = (int)(n / 8) + 1;
	//assign number of rows
	bitAdjacencyMatrix = new unsigned char*[n];
	//each row has size number of columns
	for (int i = 0; i < n; i++) {
		bitAdjacencyMatrix[i] = new unsigned char[size];
	}
	//Initialize the bit Adjacency Matrix structure
	for (int i = 0; i < n; i++)
		for (int j = 0; j < size; j++)
			bitAdjacencyMatrix[i][j] = 0x00 << 8;

}

BitMatrixGraph::BitMatrixGraph(int numNodes) {
	n = numNodes;//number of vertices is given
	int size;//created to account for the unsigned char
	// Create the bit Adjacency Matrix structure
	//start by finding the number of unsigned char needed per row
	if (n % 8 == 0)
		size = n / 8;
	else
		size = (int)(n / 8) + 1;
	//assign number of rows
	bitAdjacencyMatrix = new unsigned char*[n];
	//each row has size number of columns
	for (int i = 0; i < n; i++) {
		bitAdjacencyMatrix[i] = new unsigned char[size];
	}
	//Initialize the bit Adjacency Matrix structure
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < size; j++) {
			
			bitAdjacencyMatrix[i][j] = 0x00 << 8;
			
		}
	}
}

void BitMatrixGraph::deleteRows() {
	//go through the matrix and delete everything
	if (bitAdjacencyMatrix != NULL) {
		for (int i = 0; i < n; i++) {
			delete[] bitAdjacencyMatrix[i];
			bitAdjacencyMatrix[i] = NULL;
		}
		delete[] bitAdjacencyMatrix;
		bitAdjacencyMatrix = NULL;
	}
}

BitMatrixGraph::~BitMatrixGraph() {
	deleteRows();
}

void BitMatrixGraph::copy(const BitMatrixGraph& otherGraph) {
	n = otherGraph.n; // copy the number of vertices
	bitAdjacencyMatrix = new unsigned char*[n];
	//find the number of unsigned char needed per row
	int size;
	// Create the bit Adjacency Matrix structure
	if (n % 8 == 0)
		size = n / 8;
	else
		size = (int)(n / 8) + 1;
	
	//give the number of rows
	for (int i = 0; i < n; i++) {
		bitAdjacencyMatrix[i] = new unsigned char[size];
	}

	//Copy the bit Adjacency Matrix structure
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < size; j++)
			bitAdjacencyMatrix[i][j] = otherGraph.bitAdjacencyMatrix[i][j];
	}
}

BitMatrixGraph::BitMatrixGraph(const BitMatrixGraph& otherGraph) {
	copy(otherGraph);
}

void BitMatrixGraph::display(ostream& s) {
	int size;//calculate the number of char per row
	if (n % 8 == 0)
		size = n / 8;
	else
		size = (int)(n / 8) + 1;
	//Print the bits of the matrix as specified in the project description
	s << "Print Bits: " << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < size; j++) {
			printBits(bitAdjacencyMatrix[i][j]);
			s << " ";
		}
		s << endl;
	}
	s << "Edge List: " << endl;
	s << "[ ";
	//look through the matrix to find edge pairs
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (isAnEdge(i, j)) {
				s << "[" << i << ", " << j << "] ";
			}
		}
	}
	s << "]";
}

void BitMatrixGraph::operator=(const BitMatrixGraph& otherGraph) {
	copy(otherGraph);
}

void BitMatrixGraph::addEdge(int u,int v) {
	//do not allow an edge between the same vertex
	if (u == v)
		return;

	int charPos;//find which char to look at

	int k = v % 8;// find how much to shift
	
	charPos = v / 8;
	//add the edge
	bitAdjacencyMatrix[u][charPos] |= 0x01 << (7 - k);

	charPos = u / 8;

	k = u % 8;//find how much to shift

	//add the mirrored edge
	bitAdjacencyMatrix[v][charPos] |= 0x01 << (7 - k);
}

bool BitMatrixGraph::isAnEdge(int u, int v) {
	if (u == v)
		return false;
	int charPos = v / 8;//find which char to look at
	int k = v % 8;//find how much to shift
	return (bitAdjacencyMatrix[u][charPos] & (1 << (7 - k)));
}

void BitMatrixGraph::BFS(int p) {
	int vertex;//the current vertex being checked
	int* parentArray = new int[n]; // create an array of parents as specified in description
	parentArray[p] = -1; // starting point has no parent
	queue<int> Q;// queue is used in the BFS
	bool* visited = new bool[n];// an array specifying if the vertex has been visited or not
	for (int i = 0; i < n; i++)
		visited[i] = false;
	Q.push(p);
	visited[p] = true;
	//loop used to perform the BFS
	while (!Q.empty()) {
		vertex = Q.front();
		Q.pop();
		//look for neighbors by looking through the matrix
		for (int j = 0; j < n; j++) {
				if (isAnEdge(vertex,j)) {
					//case the found vertex has not been visited
					if (!visited[j]) {
						parentArray[j] = vertex;
						visited[j] = true;
						Q.push(j);
					}
				}
		}
	}
	//print the content of the parent array
	for (int i = 0; i < n; i++) 
		cout << i << " " << parentArray[i] << endl;
	delete[] parentArray;
	delete[] visited;
}

//recursive DFS in commented

//void BitMatrixGraph::depthFirstSearch(int p) {
//	int size;//calculate the number of char per row
//	if (n % 8 == 0)
//		size = n / 8;
//	else
//		size = (int)(n / 8) + 1;
//	int* parentArray = new int[n];//create parent array as specified in project description
//	parentArray[p] = -1;//starting vertex has no parent
//	bool* visited = new bool[n];//create an array specifying if the vertex has been visited
//	for (int i = 0; i < n; i++)
//		visited[i] = false;
//	visited[p] = true;
//	DFS(p, parentArray, size, visited);//call recursive method that will perform the DFS
//	//print out the contents of the parent array
//	for (int i = 0; i < n; i++)
//		cout << i << " " << parentArray[i] << endl;
//	delete[] parentArray;
//}
//
//void BitMatrixGraph::DFS(int p, int* parentArray,int size, bool* visited) {
//	int rowSpot = 0;//used to help with bit manipulation
//	int vertex = p;//variable that holds the current vector being looked at
//	
//	vector<int> nbors;//a vector holding the neighbors of the vertex being checked
//	//loop used to find neighbors
//	for (int j = 0; j < size; j++) {
//		for (int k = 7; k >= 0; k--) {
//			//case where there something was found
//			if (bitAdjacencyMatrix[vertex][j] & (1 << k)) {
//				//case that the found vertex has not been visited
//				if (!visited[rowSpot + 7 - k]) {
//					nbors.push_back(rowSpot + 7 - k);
//					parentArray[rowSpot + 7 - k] = vertex;
//					visited[rowSpot + 7 - k] = true;
//				}
//			}
//		}
//		rowSpot += 8;
//	}
//	rowSpot = 0;
//	//recursively call DFS on each neighbor of the vertex
//	for (int i = nbors.size()-1; i >= 0; i--) {
//			DFS(nbors[i], parentArray, size, visited);
//	}
//}

void BitMatrixGraph::DFS(int p) {
	int vertex;//the current vertex being checked
	int* parentArray = new int[n]; // create an array of parents as specified in description
	parentArray[p] = -1; // starting point has no parent
	stack<int> Q;// stack is used in the DFS
	bool* visited = new bool[n];// an array specifying if the vertex has been visited or not
	for (int i = 0; i < n; i++)
		visited[i] = false;
	Q.push(p);
	visited[p] = true;
	//loop used to perform the DFS
	while (!Q.empty()) {
		vertex = Q.top();
		Q.pop();
		//look for neighbors by looking through the matrix
		for (int j = 0; j < n; j++) {
			if (isAnEdge(vertex, j)) {
				//case the found vertex has not been visited
				if (!visited[j]) {
					parentArray[j] = vertex;
					visited[j] = true;
					Q.push(j);
				}
			}
		}
	}
	//print the content of the parent array
	for (int i = 0; i < n; i++)
		cout << i << " " << parentArray[i] << endl;
	delete[] parentArray;
	delete[] visited;
}

int main() {
	
	int one; // variable created to hold the first integer in each line of input
	int two; // variable created to hold the second integer in each line of input

	cin >> one; // read in the number of vertices

	// call constructor creating a BitMatrixGraph with number of vertices given
	BitMatrixGraph* myG = new BitMatrixGraph(one); 

	//priming read to read in the file
	cin >> one;

	while (!cin.eof()) {
		//read in the second value in the line
		cin >> two;

		//using the two numbers given, create an edge
		myG->addEdge(one, two);

		//read in the input
		cin >> one;
	}

	cout << *myG << endl; // demonstrate the use of the overloaded operator<< to display in edge list format

	//demonstrate the use of the copy constructor as well as the operator=
	BitMatrixGraph copy1(*myG);
	//cout << copy1 << endl;
	BitMatrixGraph copy2 = *myG;
	//cout << copy2 << endl;

	//After the graph is created you will call a function that will perform the Breadth First Search
	//The output of this function is will be a parent array to store the parent of each node in the Breadth First Search
	myG->BFS(0);

	//BONUS Depth first search
	myG->DFS(0);

	//call destructor on the BitMatrixGraph pointer that we created
	delete myG;

	return 0;
}