/*
*													Hamiltonian Cycle
*/

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
using namespace std;


/************************************************************************************************************************
-------------------------------------------------------------------------------------------------------------------------
														  Graph Class
-------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************/

struct Node {
	Node* next;						 // gives the next connected node.
	int data;						// value
	int weight;					   // Delivery time.	

	// Parametrized Constructor with default argument.
	Node(int data = 0, int weight = 0) {
		this->next = NULL;
		this->data = data;
		this->weight = weight;
	}
};


class AdjList {
public:
	Node* head;

	// Default Constructor
	AdjList() {
		this->head = NULL;
	}

	void Insert(int data, int weight) {
		Node* NodeToInsert = new Node(data, weight);
		if (head == NULL) {
			head = NodeToInsert;
			return;
		}

		Node* ptr = head;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = NodeToInsert;
		return;
	}


	// For printing all the values of the node.
	void Traverse() {
		Node* trav = head;

		while (trav != NULL) {
			char data = (trav->data + 64);
			if (trav->data == 0)
				data = 'h';

			cout << "|" << data << "| " << trav->weight << endl;
			if (trav->next != NULL)
				cout << "-> ";
			trav = trav->next;
		}
		cout << "\n\n";
	}

	// Counts the edges, a vertex is attached to.
	int No_of_Nodes() {
		if (head == NULL)
			return 0;

		Node* trav = head->next;
		int count = 0;

		while (trav != NULL) {
			count++;
			trav = trav->next;
		}
		return count;
	}

	// Gives the neighbouring vertices a vertex is connected to using each edge.
	Node* Neighbour(int i) {
		if (head == NULL)
			return NULL;
		Node* trav = head->next;
		for (int j = 0; j < i; j++) {
			trav = trav->next;
		}
		return trav;
	}

	// Destructor
	~AdjList() {
		Node* temp = head;

		while (temp != NULL) {
			head = head->next;
			delete temp;
			temp = head;
		}
	}
};


class ListGraph {

public:
	int noOfVertices;
	int maxVertices;
	int TimeLimit;
	AdjList* list;

	ListGraph(int v) {
		this->noOfVertices = 0;
		this->maxVertices = v;
		this->TimeLimit = 0;
		this->list = new AdjList[maxVertices];
	}

	void SetTimeLimit(int n) {
		this->TimeLimit = n;
	}

	int no_of_maxVertices() {
		return this->maxVertices;
	}

	int TravelCost(int src, int des) {
		int n = list[src].No_of_Nodes();
		for (int i = 0; i < n; i++) {
			if (list[src].Neighbour(i)->data == des)
				return list[src].Neighbour(i)->weight;
		}
	}

	Node* CreateVertex(int data, int timeConstraint) {
		list[noOfVertices].Insert(data, timeConstraint);
		noOfVertices++;
		return list[noOfVertices].head;
	}

	void Insert_Edge(char src, char des, int weight) {
		if (src == 'h')
			src = 0;
		else
			src -= 64;

		if (des == 'h')
			des = 0;
		else
			des -= 64;

		list[(int)src].Insert((int)des, weight);

		if(src != des)
			list[(int)des].Insert((int)src, weight);
	}

	void Traverse() {
		for (int i = 0; i < noOfVertices; i++) {
			list[i].Traverse();
		}
	}

	~ListGraph() {
		for (int i = 0; i < maxVertices; i++) {
			list[i].~AdjList();
		}
	}

	// Depth-First Travesal.   O(V + E)
	// We can also find number of connected components using dfs
	/*
	* for(int i = 1; i <= maxVertices; i++){
	*	if(visited[i] == false){
	*		DFS(visited, i);
		}
	}
	*/
	bool DFS(bool*& visited, int root) {
		visited[root] = true;
		cout << root << "  ";
		for (int i = 0; i < list[root].No_of_Nodes(); i++) {
			int neighbour = list[root].Neighbour(i)->data;
			if (visited[neighbour] == false) {
				DFS(visited, neighbour);
			}
		}
		return true;
	}
};


ListGraph* ReadFromFile(string filename) {

	ifstream inputFile(filename);
	string line;

	// Counting Vertices
	inputFile >> line;

	// Counting no. of comma's + 1 = no. of vertices
	int no_of_vertices = 0;
	for (int i = 0; line[i] != '\0'; i++) {
		if (line[i] == ',')
			no_of_vertices++;
	}
	no_of_vertices++;

	// Making a AdjList of (no_of_vertices)
	ListGraph* graph = new ListGraph(no_of_vertices);

	// Edges Data
	string edgesData;
	inputFile >> edgesData;

	// Weights
	string weight;
	inputFile >> weight;
	queue<int> Ttime;

	for (int i = 0; weight[i] != '\0'; i++) {
		string num;
		if (weight[i] == '{' || weight[i] == ',') {
			i++;
			while (weight[i] != ',') {
				num += weight[i++];
				if (weight[i] == '\0')
					break;
			}
			Ttime.push(stoi(num));
			i--;
		}
	}

	// Extracting Delivery time
	string Tc;
	queue<int> Dtime;
	inputFile >> Tc;

	for (int i = 0; Tc[i] != '\0'; i++) {
		if (Tc[i] == '=') {
			string num;
			while (Tc[++i] != ',') {
				if (Tc[i] == '\0') {
					i--;
					break;
				}
				num += Tc[i];
			}
			Dtime.push(stoi(num));
		}
	}


	// Creating Vertices
	for (int i = 0, n = 0; i < no_of_vertices; i++) {
		// for h vertex
		if (i != 0) {
			n = Dtime.front();
			Dtime.pop();
		}
		graph->CreateVertex(i, n);
	}

	// Connecting the Edges
	for (int i = 0; edgesData[i] != '\0'; i++) {
		if (edgesData[i] == '(') {
			char src = edgesData[++i];
			// comma
			i++;
			char des = edgesData[++i];
			graph->Insert_Edge(src, des, Ttime.front());
			Ttime.pop();
		}
	}

	// Time Limit
	string Tlimit;
	inputFile >> Tlimit;

	// Extracting Time Limit
	string str_num;
	for (int i = 0; Tlimit[i] != '\0'; i++) {
		if (Tlimit[i] == '=') {
			i++;
			for (; Tlimit[i] != '\0'; i++)
				str_num += Tlimit[i];
			i--;
		}
	}

	graph->SetTimeLimit(stoi(str_num));

	graph->Traverse();

	inputFile.close();

	return graph;
}


bool hamiltonianCycle(ListGraph*& graph, vector<bool> visited, vector<int>& path, int v, int& cost, int& count) {
	if (path.size() == graph->maxVertices) {
		count++;
		for (int i = 0; i < graph->list[v].No_of_Nodes(); i++) {
			int neighbour = graph->list[v].Neighbour(i)->data;

			if (path[0] == neighbour) {
				cost += graph->TravelCost(v, neighbour);
				if (cost <= graph->TimeLimit) {
					for (int i : path) {
						cout << (i == 0 ? 'h' : (char)(i + 64)) << " ";
					}
					cout << "h" << "  " << cost << endl;
					cost -= graph->TravelCost(v, neighbour);
					return true;
				}
				cost -= graph->TravelCost(v, neighbour);
				// No need to check further neighbours. if(last to first found and travel cost is >)
				break;
			}
		}
		return false;
	}

	bool found = false;
	int n = graph->list[v].No_of_Nodes();
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

	for (int i = 0; i < n; i++) {
		int neighbour = graph->list[v].Neighbour(i)->data;
		if (!visited[neighbour]) {
			int weight = graph->list[neighbour].head->weight + graph->TravelCost(v, neighbour);
			pq.push(make_pair(weight, neighbour));
		}
	}

	while (!pq.empty() && !found) {
		int neighbour = pq.top().second;
		pq.pop();

		visited[neighbour] = true;
		path.emplace_back(neighbour);
		cost += graph->list[neighbour].head->weight;
		cost += graph->TravelCost(v, neighbour);

		found = hamiltonianCycle(graph, visited, path, neighbour, cost, count);

		visited[neighbour] = false;
		cost -= graph->list[neighbour].head->weight;
		cost -= graph->TravelCost(v, neighbour);
		path.pop_back();
	}
	return found;
}

int main() {
	ListGraph* lg = NULL;
	vector<bool> visited;
	vector<int> path;
	int cost = 0;

	const int Files = 4;
	string file_name = "test/P2_test1";
	string txt = ".txt";


	for (int i = 0; i < Files; i++) {
		int count = 0;
		cout << "TEST CASE: " << file_name[file_name.length() - 1] << endl;
		lg = ReadFromFile(file_name + txt);

		visited.assign(lg->no_of_maxVertices(), false);
		path.emplace_back(0);
		visited[0] = true;

		cost = 0;
		if (!(hamiltonianCycle(lg, visited, path, 0, cost, count)))
			cout << "NO FEASIBLE CIRCUIT!" << endl << endl;

		cout << "Permutations Tried: " << count << endl << endl;
		visited.clear();
		path.clear();
		file_name[file_name.length() - 1] += 1;
		lg = NULL;
	}

	cout << "Worst Case T.C = (V - 1)! = O(V!)";
}




/*
Best Case Time Complexity:
The best case occurs when the algorithm finds a Hamiltonian cycle early in the search tree, without having to explore all possible permutations of vertices. In this case, the time complexity is dominated by the sorting of neighbors at each vertex, resulting in a time complexity of O(V^2 * log(V)), where V is the number of vertices. This is because, for each vertex, we process n neighbors in O(log(n)) time.

Worst Case Time Complexity:
The worst case occurs when the algorithm has to explore all possible permutations of vertices before finding a Hamiltonian cycle or when it cannot find a feasible Hamiltonian cycle. In this case, the time complexity is O(V * log(V) + (V-1)!). This is due to the O(V * log(V)) time complexity for processing the neighbors of a single vertex and the
O((V-1)!) time complexity for exploring all possible permutations of vertices.

Here, V represents the number of vertices in the graph.
*/

/*
Maximum number of edges:
The maximum number of edges in an undirected connected graph can be calculated using the formula:
(n * (n - 1)) / 2

In this case, n = 4.

Maximum edges = (4 * (4 - 1)) / 2 = (4 * 3) / 2 = 12 / 2 = 6
*/

