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
	int n, m, a, s, c, d; std::cin >> n >> m >> a >> s >> c >> d;
	//n number of relevant positions, m number of slopes and ski lifts,
	//a number of secret agents, s number of shelters
	//c capacity of each shelter, d thime an agent needs to perfrom protocol at shelter
	const int v_source = n*2;
	const int v_target = n*2+1;
	graph G(n*2+2);
    edge_adder adder(G);  
	auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
	for (int i = 0; i < m; i++){
		char w; int x, y, z; std::cin >> w >> x >> y >> z;
		// if w = S -> slope can only be used in one direction
		// else -> lift can be used in both directions
		// x to y in z time
		adder.add_edge(x, y, a, z);
		if (w == 'L'){
			adder.add_edge(y, x, a, z);
		}
	}
	for (int i = 0; i < a; i++){
		int a_i; std::cin >> a_i; //starting positions of agents
		adder.add_edge(v_source, a_i, 1, 0);
	}
	for (int i = 0; i < n; i++){
		adder.add_edge(i, n+i, 1, d);
	}
	for (int i = 0; i < s; i++){
		int s_i; std::cin >> s_i; //selter position
		adder.add_edge(n+s_i, v_target, a, 0);
	}
	int cost = boost::find_flow_cost(G);
    std::cout << cost << "\n";
	for (int i = 0; i < 2*n+2; i++){
		out_edge_it e, eend;
		for(boost::tie(e, eend) = boost::out_edges(boost::vertex(i,G), G); e != eend; ++e) {
			std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
				<< " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
		}
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
