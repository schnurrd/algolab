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
	int n, m, s, d; std::cin >> n >> m >> s >> d; // number of astronomical ojects, number of links, number of starting locaitons, number of destinations
	graph G(2*n);
    edge_adder adder(G);  
	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_sink = boost::add_vertex(G);
	for (int i = 0; i < n; i++){
		adder.add_edge(i, n+i, 1);// add per_object a cap of 1
	}
	for (int k = 0; k < m; k++){
		int i, j; std::cin >> i >> j;
		adder.add_edge(n+i, j, s);
	}
	for (int i = 0; i < s; i++){
		int s_i; std::cin >> s_i;
		adder.add_edge(v_source, s_i, s);
	}
	for (int i = 0; i < d; i++){
		int d_i; std::cin >> d_i;
		adder.add_edge(n+d_i, v_sink, s);
	}
	long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  	std::cout << flow << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
