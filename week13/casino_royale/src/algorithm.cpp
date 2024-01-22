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
typedef boost::graph_traits<graph>::edge_iterator           edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  edge_desc add_edge(int from, int to, long capacity, long cost) {
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
	return e;
  }
};


void testcase() {
  // TODO:
  int n, m, l; std::cin >> n >> m >> l; // number of train stops, number of mission, number of agents at max on train
  graph G(n+2);
  const int v_source = n;
  const int v_target = n+1;
  edge_adder adder(G);  
  adder.add_edge(v_source, 0, l, 0);
  for(int i = 0; i < n-1; i++){
    adder.add_edge(i, i+1, l, 129);
  }
  adder.add_edge(n-1, v_target, l, 0);
  std::vector<edge_desc> edges_mission(m);
  for (int i = 0; i < m; i++){
    int x, y, q; std::cin >> x >> y >> q; //travel from t_x to t_y, mission priority q [1-128]
    edges_mission[i] = adder.add_edge(x, y, 1, (y-x)*129-q);
  }
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  long cost = 0;
  edge_it e_beg, e_end;
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  auto w_map = boost::get(boost::edge_weight, G); // new!
  for (auto edge: edges_mission) {
	long flow = c_map[edge] - rc_map[edge]; // Actual flow through the edge
    long edge_cost = ((boost::target(edge, G)-boost::source(edge,G))*129 - w_map[edge]) * flow;
    cost += edge_cost;

  }
  std::cout << cost << "\n";
  //maximize priorities
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
