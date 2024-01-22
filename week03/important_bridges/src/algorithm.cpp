// ref: https://algolab.inf.ethz.ch/doc/boost/doc/libs/1_74_0/libs/graph/example/biconnected_components.cpp.html
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>

// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

struct edge_component_t
{
	enum
	{
		num = 555
	};
	typedef boost::edge_property_tag kind;
} edge_component;


// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list< boost::vecS, boost::vecS, boost::undirectedS, 
		boost::no_property,
        boost::property< edge_component_t, std::size_t > >graph;
typedef boost::graph_traits<graph>::vertex_descriptor    vertex_desc;    // Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).  
typedef boost::graph_traits<graph>::edge_descriptor    edge_desc;    
typedef boost::graph_traits<graph>::edge_iterator    edge_it;   

void testcase() {
	// TODO:
	int n, m; std::cin >> n >> m; //number of islands, number of bridges
	graph G(n);
	std::vector<std::vector<int>> adj_list(n, std::vector<int>(0));
	for (int i = 0; i < m; i++){
		int u, v; std::cin >> u >> v;
		boost::add_edge(u, v, G);
		adj_list[u].push_back(v);
		adj_list[v].push_back(u);
	}
	// Connected components
	// ====================
	boost::property_map< graph, edge_component_t >::type components = 
			boost::get(edge_component, G);

    biconnected_components(G, components);

	// first iterate over edges and count how many edges component has
    std::unordered_map<int,int> component_sizes;
	edge_it iter, iter_end;
	for(boost::tie(iter, iter_end) = edges(G); iter != iter_end; ++iter) {
		component_sizes[components[*iter]]++;
	}

	std::vector<std::pair<int, int>> solution;
	// iterate over the edges save the ones that are the sole edge of a compontn	
	for(boost::tie(iter, iter_end) = edges(G); iter != iter_end; ++iter) {
		const auto edge = *iter;
		if (component_sizes[components[edge]] == 1) {
			const int v1 = boost::source(edge, G);
			const int v2 = boost::target(edge, G);
			solution.emplace_back(std::min(v1, v2), std::max(v1, v2));
		}
	}
	//print results
	std::sort(solution.begin(), solution.end());
	std::cout << solution.size() << "\n";
	for (std::pair<int, int> sol : solution){
		std::cout << sol.first << " " << sol.second << "\n";
	}

	//either end part with only one edge or 
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
