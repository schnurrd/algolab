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
	int l, p; std::cin >> l >> p; //number locations, paths
	graph G(l);
  	edge_adder adder(G);
	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_sink = boost::add_vertex(G);
	std::vector<std::vector<int>> supply_and_demand(l, std::vector<int>(2));
	for (int i = 0; i < l; i++){
		std::cin >> supply_and_demand[i][0] >> supply_and_demand[i][1]; 
		//number soldiers stationed at i, number of soldiers nedded to defend i
	}
	for (int i = 0; i < p; i++){
		int f_i, t_i, c_i, C_i; std::cin >> f_i >> t_i >> c_i >> C_i;
		supply_and_demand[f_i][1] += c_i; //increase demand
		supply_and_demand[t_i][0] += c_i; //increase supply
		adder.add_edge(f_i, t_i, C_i - c_i); // suptract c_min
		// i-th path from loc f_i to t_i, needs to be traversed at least c_i and at most C_j
	}
	int needed_flow = 0;
	for (int i = 0; i < l; i++){
		adder.add_edge(v_source, i, supply_and_demand[i][0]);
		adder.add_edge(i, v_sink, supply_and_demand[i][1]);
		needed_flow += supply_and_demand[i][1];
	}
	int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
	if (needed_flow == flow){
		std::cout << "yes" << "\n";
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