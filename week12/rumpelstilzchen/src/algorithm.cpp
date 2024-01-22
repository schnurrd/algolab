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
	int n, p_g, p_h, e_g, e_h, f_g, f_h, s_g, s_h;
	// number of islands, p_x number of cities in network , e_x, number of edges in x, 
	// f_x number of edges from x to islands, s_x number of straw available in the castle of x
	std::cin >> n >> p_g >> p_h >> e_g >> e_h >> f_g >> f_h >> s_g >> s_h; 
	graph G(2*n+p_g+p_h+2);
	edge_adder adder(G);  
	const int v_source = 2*n+p_g+p_h;
	const int v_target = 2*n+p_g+p_h+1;
	std::vector<int> preference_scores_for_islands(n);
	for (int i = 0; i < n; i++){
		std::cin >> preference_scores_for_islands[i];
	}
	adder.add_edge(v_source, 0, s_g, 0);
	for (int i = 0; i < e_g; i++){
		// connection from city u to v with cap c;
		int u, v, c; std::cin >> u >> v >> c;
		adder.add_edge(u, v, c, 0);
	}
	for (int i = 0; i < e_h; i++){
		int u, v, c; std::cin >> u >> v >> c;
		adder.add_edge(p_g+2*n+v, p_g+2*n+u, c, 0); //insert backwards
	}
	for (int i = 0; i < f_g; i++){
		// connection from city u to v with cap c;
		int u, v, c; std::cin >> u >> v >> c;
		adder.add_edge(u, p_g+v, c, 0);
	}
	for (int i = 0; i < n; i++){
		adder.add_edge(p_g+i, p_g+n+i, 1, 1024-preference_scores_for_islands[i]);
	}

	for (int i = 0; i < f_h; i++){
		// connection from city u to v with cap c;
		int u, v, c; std::cin >> u >> v >> c;
		adder.add_edge(p_g+v+n, p_g+2*n+u, c, 0); //insert backwards
	}
	adder.add_edge(p_g+2*n, v_target, s_h, 0);
	int flow = boost::push_relabel_max_flow(G, v_source, v_target);
	boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = 1024*flow - boost::find_flow_cost(G);
	std::cout << flow << " " << cost << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
