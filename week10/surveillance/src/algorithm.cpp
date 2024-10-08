#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};


void testcase() {
	// TODO:
	int n, m, k, l; std::cin >> n >> m >> k >> l; // number of intersections, number of streets, number of police stations, number of photographs
	graph G(2*n);
	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_sink = boost::add_vertex(G);
  	edge_adder adder(G);

	int min_val = std::min(k, l);
	for (int i = 0; i < k; i++){
		int x; std::cin >> x; // location of police station (in 0 to n-1)
		adder.add_edge(v_source, x, 1);
		adder.add_edge(n+x, v_sink, 1);
	}
	for (int i = 0; i < l; i++){
		int x; std::cin >> x; // location of photographs
		adder.add_edge(x, n+x, 1);
	}
	for (int i = 0; i < m; i++){
		int x, y; std::cin >> x >> y; //one way connections between x and y
		adder.add_edge(x, y, min_val);
		adder.add_edge(n+x, n+y, 1);
	}
	int flow1 = boost::push_relabel_max_flow(G, v_source, v_sink);
	min_val = std::min(min_val, flow1);
	std::cout << min_val << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
