#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};


void testcase() {
	// TODO:
	int n; std::cin >> n; // number of days to plan

	graph G(n+2);
	const int v_source = n;
	const int v_target = n+1;
	edge_adder adder(G);  
	auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
	for (int i = 0; i < n; i++){
		int a_i, c_i; std::cin >> a_i >> c_i;
		adder.add_edge(v_source, i, a_i, c_i);
		//max amount of menues that can be produced + cost per menu
	}
	int num_overall_meels_needed = 0;
	for (int i = 0; i < n; i++){
		int s_i, p_i; std::cin >> s_i >> p_i;
		adder.add_edge(i, v_target, s_i, 20-p_i);
		num_overall_meels_needed += s_i;
		// number of students who want to eat + menu price set by uni
	}
	for (int i = 0; i < n-1; i++){
		int v_i, e_i; std::cin >> v_i >> e_i;
		adder.add_edge(i, i+1, v_i, e_i);
		// number of menues whihc can be stored overnight from day i to day i+1
		// + cooling energy cost per menu
	}
	boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
	int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
	cost -= 20 * s_flow;
	if (s_flow != num_overall_meels_needed){
		std::cout << "im";
	} 
	std::cout << "possible " << s_flow << " " << -cost << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
