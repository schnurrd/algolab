#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::property<boost::edge_weight_t, int>
		> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;

int n, e, s, a, b; 

void calculate_mst(graph& g, weight_map& weights, int h, graph& graph_final, weight_map&weights_final){
	std::priority_queue<std::tuple<int,int, int>, std::vector<std::tuple<int,int, int>>, 
						std::greater<std::tuple<int,int, int>>> min_heap;
	std::vector<bool> visited(n, false); 
	min_heap.push(std::tuple<int, int, int>(0, h, h));
	while(!min_heap.empty()){
		auto current_vertex = min_heap.top(); min_heap.pop();
		int weight_edge = std::get<0>(current_vertex);
		int origin = std::get<1>(current_vertex);
		int vertex = std::get<2>(current_vertex);
		if (visited[vertex]){
			continue;
		}
		// add edge to graph_final
		edge_desc e_g;
		e_g = boost::add_edge(vertex, origin, graph_final).first; weights_final[e_g] = weight_edge;
		visited[vertex] = true;
		out_edge_it oe_beg, oe_end;
		for (boost::tie(oe_beg, oe_end) = boost::out_edges(vertex, g); oe_beg != oe_end; ++oe_beg) {
			if (!visited[boost::target(*oe_beg, g)]){
				min_heap.push(std::tuple<int, int, int>(weights[*oe_beg], vertex, boost::target(*oe_beg, g)));
			}
		}
	}
}

int dijkstra_dist(const graph &G, int start, int t) {
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, start,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

void testcase() {
	// TODO:
	std::cin >> n >> e >> s >> a >> b;
	std::vector<graph> graphs(s, graph(n));
	std::vector<weight_map> weight_maps(s);
	for (int i = 0; i < s; i++){
		weight_maps[i] = boost::get(boost::edge_weight, graphs[i]);
	}
	graph graph_final(n);
	weight_map weights_final = boost::get(boost::edge_weight, graph_final);
	// number of trees, edges, species, start and finishing tree
	edge_desc e_g;
	for (int i = 0; i < e; i++){
		int t_1, t_2; std::cin >> t_1 >> t_2;
		for (int j = 0; j < s; j++){
			int s_i; std::cin >> s_i;
			e_g = boost::add_edge(t_1, t_2, graphs[j]).first; weight_maps[j][e_g] = s_i;
		}
	}
	for (int i = 0; i < s; i++){
		int h_i; std::cin >> h_i;
		calculate_mst(graphs[i], weight_maps[i], h_i, graph_final, weights_final);
	}
	std::cout << dijkstra_dist(graph_final, a, b) << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
