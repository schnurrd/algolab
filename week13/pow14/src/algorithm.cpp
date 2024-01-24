#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc_g;
typedef traits::edge_descriptor edge_desc_g;

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


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map_wg;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc_wg;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc_wg;

void testcase() {
	// TODO:
	int n, m, a, s, c, d; std::cin >> n >> m >> a >> s >> c >> d;
	//n number of relevant positions, m number of slopes and ski lifts,
	//a number of secret agents, s number of shelters
	//c capacity of each shelter, d thime an agent needs to perfrom protocol at shelter
	std::vector<std::vector<int>> dist_per_agent(a, std::vector<int>(s, std::numeric_limits<int>::max()));
	weighted_graph G(n+s);
  	weight_map_wg weights = boost::get(boost::edge_weight, G);
	edge_desc_wg e;
	for (int i = 0; i < m; i++){
		char w; int x, y, z; std::cin >> w >> x >> y >> z;
		// if w = S -> slope can only be used in one direction
		// else -> lift can be used in both directions
		// x to y in z time
		e = boost::add_edge(x, y, G).first; weights[e]=z;
		if (w == 'L'){
			e = boost::add_edge(y, x, G).first; weights[e]=z;
		}
	}
	std::vector<int> agents(a);
	for (int i = 0; i < a; i++){
		std::cin >> agents[i]; //starting positions of agents
	}
	std::vector<int> shelters(s);
	for (int i = 0; i < s; i++){
		int s_i; std::cin >> s_i; //selter position
		shelters[i] = s_i;
		e = boost::add_edge(s_i, n+i, G).first; weights[e]=d;
	}
	int max_time = 0;
	for (int i = 0; i < a; i++){
		std::vector<int> dist_map(n+s);

		boost::dijkstra_shortest_paths(G, agents[i],
			boost::distance_map(boost::make_iterator_property_map(
			dist_map.begin(), boost::get(boost::vertex_index, G))));

		for(int j = 0; j < s; j++){
			dist_per_agent[i][j] = dist_map[n+j];
			if (dist_per_agent[i][j] != std::numeric_limits<int>::max()){
				max_time = std::max(max_time, dist_per_agent[i][j] + d);
			}
		}
	}
	int min_time = 1;
	while (min_time < max_time){
		int threshold = (max_time - min_time)/2 + min_time;
		graph G_M(a+2*s+2);
		edge_adder adder(G_M);
		const int v_source = a+2*s;
		const int v_sink = a+2*s+1;
		for (int i = 0; i < a; i++){
			adder.add_edge(v_source, i, 1);
			for (int j = 0; j < s; j++){
				if (dist_per_agent[i][j] <= threshold){
					adder.add_edge(i, a+j, 1);
					if ((dist_per_agent[i][j] + d) <= threshold and c==2){
						adder.add_edge(i, a+s+j, 1);
					}
				}
			}
		}
		for (int j = 0; j < s; j++){
			adder.add_edge(a+j, v_sink, 1);
			adder.add_edge(a+s+j, v_sink, 1);
		}
		int flow = boost::push_relabel_max_flow(G_M, v_source, v_sink);
		if (flow == a){
			max_time = threshold;
		} else {
			min_time = threshold + 1;
		}
	}

	std::cout << max_time << "\n";
	
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
