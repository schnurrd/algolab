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
	int c, g, b, k, a; std::cin >> c >> g >> b >> k >> a;
	//number of cities, number of guides, budget, number kholby, number allahabad
	// from kholby to allahabad

	int v_source = c;
	int v_target = c+1;
    graph G(c+2);
    edge_adder adder(G);  
    
	for (int i = 0; i < g; i++){
		int x, y, d, e; std::cin >> x >> y >> d >> e;
		adder.add_edge(x, y, e, d);
		// city x -> y, cost d per elephat, e elephants
	}
	
	adder.add_edge(a, v_target, b, 0);

	auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(v_source, k, G).first;
	const edge_desc rev_e = boost::add_edge(k, v_source, G).first;
	w_map[e] = 0;   // new assign cost
	w_map[rev_e] = -0;   // new negative cost
	r_map[e] = rev_e;
	r_map[rev_e] = e;
	
	c_map[e] = b;
	c_map[rev_e] = 0; 

	//binary search
	int bottom = 0;
    int top = boost::push_relabel_max_flow(G, v_source, v_target);

	while (bottom < top){
		int mid = (top - bottom)/2 + bottom;
		c_map[e] = mid;
		c_map[rev_e] = 0; // reverse edge has no capacity!
		boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    	long cost = boost::find_flow_cost(G);
		// use min of mid, real flow
		if (cost == b){
			break;
		} else if (cost < b){
			bottom = mid + 1;
		} else {
			top = mid - 1;
		}
	}
	int result = (top - bottom)/2 + bottom;
	c_map[e] = result;
	c_map[rev_e] = 0; // reverse edge has no capacity!
	boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
	long cost = boost::find_flow_cost(G);
	if (cost > b){
		result--;
	}
	std::cout << result << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
