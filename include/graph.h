#ifndef LAB_6
#define LAB_6


#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iterator>
#include <cfloat>



template<typename Vertex, typename Distance = double>
class Graph {
public:
	struct Edge {
		Vertex from, to;
		Distance distane;
	};

	bool has_vertex(const Vertex& v) const;
	void add_vertex(const Vertex& v);
	bool remove_vertex(const Vertex& v);

	bool has_edge(const Edge& e) const; 
	bool has_edge(const Vertex& from, const Vertex& to) const;
	void add_edge(const Vertex& from, const Vertex& to, const Distance& dist);
	bool remove_edge(const Edge& e); 
	bool remove_edge(const Vertex& from, const Vertex& to);

	std::vector<Vertex> vertices() const; 
	std::vector<Edge> edges(const Vertex& v) const; 

	size_t order() const; 
	size_t degree(const Vertex& v) const; 

	std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const; 
	std::vector<Vertex> walk(const Vertex& start_vertex) const; 


private:
	std::vector<Vertex> _vertices;
	std::unordered_map < Vertex, std::vector<Edge> > _edges;
};












template<typename Vertex, typename Distance> //проверяет наличие вершины в графе.
bool Graph<Vertex, Distance>::has_vertex(const Vertex& v) const {
	return std::find(_vertices.begin(), _vertices.end(), v) != _vertices.end();
}

template<typename Vertex, typename Distance> //добавляет вершину в граф.
void Graph<Vertex, Distance>::add_vertex(const Vertex& v) {
	if (has_vertex(v)) throw std::invalid_argument("[add_vertex] the vertex already exists");
	_vertices.push_back(v);
	//_edges[v] = { };
}

template<typename Vertex, typename Distance> //удаляет вершину из графа.
bool Graph<Vertex, Distance>::remove_vertex(const Vertex& v) {
	auto& it = std::find(_vertices.begin(), _vertices.end(), v);
	if (it == _vertices.end()) return false;
	_vertices.erase(v);
	_edges.erase(v);
	for (auto& vertex : _vertices) {
		auto& edges = _edges.at(vertex);
		edges.erase(std::remove_if(edges.begin(), edges.end(), [v](const Edge& e) {return e.to == v; } edges.end()));
	}
	return true;
}

template<typename Vertex, typename Distance> //добавляет ребро в граф.
void Graph<Vertex, Distance>::add_edge(const Vertex& from, const Vertex& to, const Distance& dist) {
	if (has_edge({ from, to, dist })) throw std::invalid_argument("[add_edge] the edge already exists");

	auto& it = _edges.find(from);
	it->second.push_back({ from, to, dist });
}

template<typename Vertex, typename Distance> //удаляет ребро из графа.
bool Graph<Vertex, Distance>::remove_edge(const Edge& e) {
	if (!has_edge(e)) return false;
	auto& it = _edges.find(e.from);
	for (const Edge& edge : it->second) {
		if (e.to == edge.to && (abs(e.distane - edge.distance) < DBL_EPSILON)) {
			it->second.erase(edge);
			return true;
		}
	}
	return false;
}

template<typename Vertex, typename Distance> //проверяет наличие рёбра в графе.
bool Graph<Vertex, Distance>::has_edge(const Edge& e) const {
	auto it = _edges.find(e.from);
	if (it != _edges.end()) {
		for (const auto& edge : it->second) {
			if (e.to == edge.to && (abs(e.distane - edge.distance) < DBL_EPSILON))
				return true;
		}
	}
	return false;
}

template<typename Vertex, typename Distance> //возвращает вектор вершин графа.
std::vector<Vertex> Graph<Vertex, Distance>::vertices() const { return _vertices; } 

template<typename Vertex, typename Distance> //возвращает вектор рёбер, исходящих из вершины v.
std::vector<typename Graph<Vertex, Distance>::Edge> Graph<Vertex, Distance>::edges(const Vertex& v) const {
	if (!has_vertex(v)) throw std::invalid_argument("[edges] there is no such vertex in the graph");
	std::vector<Edge> exiting_edges;

	auto it = _edges.find(v);
	if (it != _edges.end()) {
		exiting_edges = it->second;
	}
	return exiting_edges;
}
#endif 