#ifndef LAB_6
#define LAB_6


#include <algorithm>
#include <cfloat>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>


enum class Color { White, Gray, Black };

template<typename Vertex, typename Distance = double>
class Graph {
public:
	struct Edge {
		Vertex from, to;
		Distance distance;
	};

	bool has_vertex(const Vertex& v) const;
	void add_vertex(const Vertex& v);
	bool remove_vertex(const Vertex& v);

	bool has_edge(const Edge& e) const;
	bool has_edge(const Vertex& from, const Vertex& to) const;
	void add_edge(const Vertex& from, const Vertex& to, const Distance& dist);
	bool remove_edge(const Edge& e); 
	bool remove_edge(const Vertex& from, const Vertex& to);

	std::pair<typename std::vector<Vertex>::iterator, typename std::vector<Vertex>::iterator> vertices(); 

	
	std::pair<typename std::vector<Vertex>::iterator, typename std::vector<Vertex>::iterator> edges(const Vertex& v);
	std::vector<Edge> exiting_edges(const Vertex& v) const; 
	std::vector<Edge> incoming_edges(const Vertex& v) const; 

	size_t order() const; 
	size_t degree(const Vertex& v) const; 

	std::vector<Edge> shortest_path(const Vertex& start, const Vertex& end) const; 
	std::vector<Vertex> walk(const Vertex& start_vertex) const; 
	Distance length_shortest_path(const Vertex& start, const Vertex& end) const;

	void print_vertices() const;
	void print_edges() const;

	Vertex find_farthest_vertex();

private:
	std::vector<Vertex> _vertices;
	std::unordered_map < Vertex, std::vector<Edge> > _edges;
};


//_____________________________________________________________

template<typename Vertex, typename Distance> 
bool Graph<Vertex, Distance>::has_vertex(const Vertex& v) const {
	return std::find(_vertices.begin(), _vertices.end(), v) != _vertices.end();
}

template<typename Vertex, typename Distance>
void Graph<Vertex, Distance>::add_vertex(const Vertex& v) {
	if (has_vertex(v)) throw std::invalid_argument("[add_vertex] the vertex already exists");
	_vertices.push_back(v);
	_edges[v] = { };
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::remove_vertex(const Vertex& v) {
	auto& it = std::find(_vertices.begin(), _vertices.end(), v);
	if (it == _vertices.end()) return false;
	_vertices.erase(v);
	_edges.erase(v);
	for (auto& vertex : _vertices) {
		auto& edges = _edges.at(vertex);
		edges.erase(std::remove_if(edges.begin(), edges.end(), [v](const Edge& e) {return e.to == v; }), edges.end());
	}
	return true;
}

template<typename Vertex, typename Distance>
void Graph<Vertex, Distance>::add_edge(const Vertex& from, const Vertex& to, const Distance& dist) {
	if (has_edge({ from, to, dist })) throw std::invalid_argument("[add_edge] the edge already exists");

	auto& edges = _edges[from];
	edges.push_back({ from, to, dist });
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::remove_edge(const Edge& e) {
	if (!has_edge(e)) return false;
	auto& edges = _edges.at(e.from);

	edges.erase(std::remove_if(edges.begin(), edges.end(), [e](const Edge& edge)
		{ return (e.from == edge.from) && (edge.to == e.to) && (std::abs(e.distance - edge.distance) < DBL_EPSILON); }, edges.end()));

	return true;
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::remove_edge(const Vertex& from, const Vertex& to) {
	if (!has_edge(from, to)) return false;
	auto& edges = _edges.at(from);

	edges.erase(std::remove_if(edges.begin(), edges.end(), [&](const Edge& e) { return (e.from == from) && (e.to == to); }), edges.end());

	return false;
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::has_edge(const Edge& e) const {
	auto& edges = _edges.at(e.from);
	for (const auto& edge : edges) {
		if (edge.to == e.to && (std::abs(e.distance - edge.distance) < DBL_EPSILON)) return true;
	}
	return false;
}

template<typename Vertex, typename Distance>
bool Graph<Vertex, Distance>::has_edge(const Vertex& from, const Vertex& to) const {
	auto& edges = _edges.at(from);
	for (const auto& edge : edges) {
		if (edge.to == to) return true;
	}
	return false;
}

template<typename Vertex, typename Distance>
std::pair<typename std::vector<Vertex>::iterator, typename std::vector<Vertex>::iterator> Graph<Vertex, Distance>::vertices() {
	return { _vertices.begin(), _vertices.end() };
}

template<typename Vertex, typename Distance>
std::pair<typename std::vector<Vertex>::iterator, typename std::vector<Vertex>::iterator> edges(const Vertex& v) {
	return { exiting_edges(v).begin(), exiting_edges(v).end() };
}

template<typename Vertex, typename Distance>
std::vector<typename Graph<Vertex, Distance>::Edge> Graph<Vertex, Distance>::exiting_edges(const Vertex& v) const {
	if (!has_vertex(v)) throw std::invalid_argument("[exiting_edges] there is no such vertex in the graph");

	return _edges.at(v);
}

template<typename Vertex, typename Distance>
std::vector<typename Graph<Vertex, Distance>::Edge> Graph<Vertex, Distance>::incoming_edges(const Vertex& v) const {
	if (!has_vertex(v)) throw std::invalid_argument("[incoming_edges] there is no such vertex in the graph");
	std::vector<Edge> edges;
	for (auto& vertex : _vertices) {
		for (auto& edge : _edges.at(vertex))
			if (edge.to == v) edges.push_back(edge);
	}

	return edges;
}

template<typename Vertex, typename Distance>
size_t Graph<Vertex, Distance>::order() const {
	return _vertices.size();
}

template<typename Vertex, typename Distance>
size_t Graph<Vertex, Distance>::degree(const Vertex& v) const {
	if (!has_vertex(v)) throw std::invalid_argument("[degree] there is no such vertex in the graph");

	return incoming_edges.size() + exiting_edges.size();
}


template<typename Vertex, typename Distance> 
std::vector<typename Graph<Vertex, Distance>::Edge> Graph<Vertex, Distance>::shortest_path(const Vertex& start, const Vertex& end) const {
	if (!has_vertex(start) || !has_vertex(end)) throw std::invalid_argument("[shortest_path] one or two vertices do not exist in the graph");

	std::unordered_map<Vertex, Distance> distance;

	for (const Vertex& vertex : _vertices) {
		distance[vertex] = std::numeric_limits<Distance>::max();
	}
	distance[start] = 0;

	std::unordered_map<Vertex, Vertex> prev;

	for (size_t i = 0; i < _vertices.size(); ++i) {
		for (const auto& [from, edges] : _edges) {
			for (const auto& edge : edges) {
				if (distance[from] + edge.distance < distance[edge.to]) {
					distance[edge.to] = distance[from] + edge.distance;
					prev[edge.to] = edge.from;
				}
			}
		}
	}
	for (const auto& [from, edges] : _edges) {
		for (const auto& edge : edges) {
			if (distance[from] + edge.distance < distance[edge.to])
				throw std::runtime_error("[shortest_path] the graph contains a negative cycle");
		}
	}
	std::vector<Edge> result;
	Vertex current = end;
	while (current != start) {
		auto it = std::find_if(_edges.at(prev[current]).begin(), _edges.at(prev[current]).end(), [&](const Edge& e) { return e.to == current; });
		result.push_back(*it);
		current = prev[current];
	}
	std::reverse(result.begin(), result.end());

	return result;
}

template<typename Vertex, typename Distance>
std::vector<Vertex> Graph<Vertex, Distance>::walk(const Vertex& start_vertex) const { 
	if (!has_vertex(start_vertex)) throw std::invalid_argument("[walk] there is no such vertex in the graph");

	std::vector<Vertex> result;
	std::unordered_map<Vertex, Color> colors;

	for (const auto& vertex : _vertices) {
		colors[vertex] = Color::White;
	}

	std::queue <Vertex> queue;
	queue.push(start_vertex);
	colors[start_vertex] = Color::Gray;

	while (!queue.empty()) {
		Vertex current_vertex = queue.front();
		queue.pop();

		for (auto& edge : _edges.at(current_vertex)) {
			if (colors[edge.to] == Color::White) {
				queue.push(edge.to);
				colors[edge.to] = Color::Gray;
			}
		}
		result.push_back(current_vertex);
		colors[current_vertex] = Color::Black;

	}
	return result;
}

template<typename Vertex, typename Distance>
Distance Graph<Vertex, Distance>::length_shortest_path(const Vertex& start, const Vertex& end) const {
	std::vector<Edge> edges = shortest_path(start, end);
	Distance len = 0;
	for (const auto& edge : edges) {
		len += edge.distance;
	}
	return len;
}

template<typename Vertex, typename Distance>
void Graph<Vertex, Distance>::print_vertices() const {
	std::cout << "Vertices: [ ";
	for (const Vertex& vertex : _vertices) {
		if (vertex != _vertices.back()) std::cout << vertex << ", ";
		else std::cout << vertex << " ]";
	}
}

template<typename Vertex, typename Distance>
void Graph<Vertex, Distance>::print_edges() const {
	std::cout << "Edges: " << std::endl;
	for (const Vertex& vertex : _vertices) {
		for (const Edge& edge : _edges.at(vertex)) {
			std::cout << edge.from << " -> " << edge.to << "(" << edge.distance << ")" << std::endl;
		}
	}
}

template<typename Vertex, typename Distance>
Vertex Graph<Vertex, Distance>::find_farthest_vertex() {
	Vertex farthest_vertex;
	Distance min_avg_distance = 100000000;

	for (const auto& vertex : _vertices) {
		Distance avg_distance = 0;
		size_t num_neighbors = 0;
		for (const auto& edge : exiting_edges(vertex)) {
			avg_distance += length_shortest_path(vertex, edge.to);
			num_neighbors++;
		}
		if (num_neighbors > 0) {
			avg_distance /= num_neighbors;
		}

		if (avg_distance < min_avg_distance) {
			min_avg_distance = avg_distance;
			farthest_vertex = vertex;
		}
	}

	return farthest_vertex;
}

#endif 