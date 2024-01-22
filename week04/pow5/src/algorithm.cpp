#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void testcase() {
	// TODO:
	int n, m, k, T; std::cin >> n >> m >> k >> T;
	weighted_graph G(2*n);
	weight_map weights = boost::get(boost::edge_weight, G);
	// number of plantes, number of edges, number of warehouses, number of planets in teleportation network
	std::vector<int> planets_in_teleportation_network(T);
	for (int i = 0; i < T; i++){
		std::cin >> planets_in_teleportation_network[i]; //planet in teleportation network
	}
	edge_desc e;
	for (int i = 0; i < m; i++){
		int u, v, c; std::cin >> u >> v >> c;
		e = boost::add_edge(v, u, G).first; weights[e]=c;
	}
	
	std::vector<int> scc_map(2*n); // exterior property map
	int nscc = boost::strong_components(G,
				boost::make_iterator_property_map(scc_map.begin(),
				boost::get(boost::vertex_index, G)));
	
	std::vector<int> linked_components(nscc, 0);
	for (int planet : planets_in_teleportation_network) {
		linked_components[scc_map[planet]]++;
	}
	for (int planet: planets_in_teleportation_network){
		e = boost::add_edge(planet, n+scc_map[planet], G).first; weights[e]=0;
		e = boost::add_edge(n+scc_map[planet], planet, G).first; weights[e]=linked_components[scc_map[planet]]-1;
	}

	std::vector<int> dist_map(2*n);

	boost::dijkstra_shortest_paths(G, n-1,
		boost::distance_map(boost::make_iterator_property_map(
		dist_map.begin(), boost::get(boost::vertex_index, G))));

	int min_time = 1000001;
	for (int i = 0; i < k; i++){
		min_time = std::min(min_time, dist_map[i]);
	}
	if(min_time <= 1000000){
		std::cout << min_time << "\n";
	} else {
		std::cout << "no" << "\n";
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
