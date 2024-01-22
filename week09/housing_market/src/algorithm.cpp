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
	int n, m, s; std::cin >> n >> m >> s; //number of buyers, number of sites, number of states
	const int v_source = n+m+s;
	const int v_target = n+m+s+1;
	graph G(n+m+s+2);
    edge_adder adder(G);  
	for (int i = 0; i < s; i++){
		int state_limit; std::cin >> state_limit; 
		adder.add_edge(n+m+i, v_target, state_limit, 0);
	}
	for (int i = 0; i < m; i++){
		int state_dependence_of_site; std::cin >> state_dependence_of_site;
		adder.add_edge(n+i, n+m+state_dependence_of_site-1, 1, 0);
	}
	for (int i = 0; i < n; i++){
		adder.add_edge(v_source, i, 1, 0);
		// prices for each buyer
		for (int j = 0; j < m; j++){
			int price_j; std::cin >> price_j;
			adder.add_edge(i, n+j, 1, 100-price_j);
		}
	}
    int flow = boost::push_relabel_max_flow(G, v_source, v_target);
	// Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
	std::cout << flow << " " << 100*flow - cost << "\n";
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
