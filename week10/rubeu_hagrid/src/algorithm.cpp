#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// BGL includes 
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;


std::vector<long> galleons, weights, values;

void fill_stats(weighted_graph& G, const int start_vertex){
	weight_map weight_map = boost::get(boost::edge_weight, G);
	out_edge_it oe_beg, oe_end;
	for (boost::tie(oe_beg, oe_end) = boost::out_edges(start_vertex, G); oe_beg != oe_end; ++oe_beg) {
		int tgt = boost::target(*oe_beg, G);
		fill_stats(G, tgt);
		weights[start_vertex] += weight_map[*oe_beg] + weights[tgt];
		values[start_vertex] += values[tgt]; 
	// boost::source(*e_beg, G), boost::target(*e_beg, G)
	}
}

long solve(weighted_graph& G, const int start_vertex, int time){
	weight_map weight_map = boost::get(boost::edge_weight, G);
	std::vector<std::pair<int, int>> children;
	out_edge_it oe_beg, oe_end;
	for (boost::tie(oe_beg, oe_end) = boost::out_edges(start_vertex, G); oe_beg != oe_end; ++oe_beg) {
		children.push_back({boost::target(*oe_beg, G), weight_map[*oe_beg]});
	}
	std::sort(children.begin(), children.end(),
		[](const std::pair<int, int>& e1, const std::pair<int, int>& e2) -> bool {
			auto [tgt1, w1] = e1;
			auto [tgt2, w2] = e2;

			return (w1 + weights[tgt1]) * values[tgt2] < (w2 + weights[tgt2]) * values[tgt1];
		}
	);
	long sum = galleons[start_vertex] - time;
	for (const auto [tgt, weight]: children) {
		sum += solve(G, tgt, time + weight);
		time += 2 * (weight + weights[tgt]);
	}

	return sum;
}

void testcase() {
	int n; std::cin >> n; //number of chambers
	weighted_graph G(n);

	galleons = std::vector<long>(n + 1, 0);
	weights = std::vector<long>(n + 1, 0);
	values = std::vector<long>(n + 1, 1);

	for (int i = 1; i <= n; i++){
		std::cin >> galleons[i];
	}
	edge_desc e;
	for (int i = 0; i < n; i++){
		int u, v, l; std::cin >> u >> v >> l; 
  		boost::add_edge(u, v, l, G); // connects chamber u and v with length l (u is closer to the surface than v), surface labeled as u=0
	}
	fill_stats(G, 0);
	
	std::cout << solve(G, 0, 0) << "\n";

	// TODO:
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
