#include "../include/graph.h"

using namespace std;

int main() {
	setlocale(LC_ALL, "rus");

	Graph<int> G;
	Graph<int>::Edge edge{ 2, 3, 1 };
	G.add_vertex(1);
	G.add_vertex(2);
	G.add_vertex(3);
	G.add_vertex(4);
	G.add_vertex(5);
	G.add_vertex(6);

	G.add_edge(1, 2, 2);
	G.add_edge(2, 1, 2);

	G.add_edge(1, 3, 3);
	G.add_edge(3, 1, 3);

	G.add_edge(1, 4, 5);
	G.add_edge(4, 1, 5);

	G.add_edge(1, 5, 7);
	G.add_edge(5, 1, 7);

	G.add_edge(1, 6, 9);
	G.add_edge(6, 1, 9);

	G.add_edge(2, 3, 4);
	G.add_edge(3, 2, 4);

	G.add_edge(2, 4, 6);
	G.add_edge(4, 2, 6);

	G.add_edge(2, 5, 8);
	G.add_edge(5, 2, 8);

	G.add_edge(2, 6, 10);
	G.add_edge(6, 2, 10);

	G.add_edge(3, 4, 2);
	G.add_edge(4, 3, 2);

	G.add_edge(3, 5, 3);
	G.add_edge(5, 3, 3);

	G.add_edge(3, 6, 4);
	G.add_edge(6, 3, 4);

	G.add_edge(4, 5, 5);
	G.add_edge(5, 4, 5);

	G.add_edge(4, 6, 3);
	G.add_edge(6, 4, 3);

	G.add_edge(5, 6, 2);
	G.add_edge(6, 5, 2);

	G.print_edges();
	

	cout << G.length_shortest_path(2, 1) << endl;
	cout << G.length_shortest_path(2, 3) << endl;
	cout << G.length_shortest_path(2, 4) << endl;
	cout << G.length_shortest_path(2, 5) << endl;
	cout << G.length_shortest_path(2, 6) << endl;

	cout << G.find_farthest_vertex() << endl;


	

	return 0;
}