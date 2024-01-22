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

void add_edge(edge_adder adder, int start, int end1, int end2, int n){
	if (end1 >= 0 and end1 < n and end2 >= 0 and end2 < n){
		adder.add_edge(start, n*n+end1*n+end2, 1);
	}
}
void testcase() {
	// TODO:
	int n; std::cin >> n; // length of each side of the chessboard (max 64)
	graph G(2*(n*n));
	edge_adder adder(G);
	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_sink = boost::add_vertex(G);
	int num_present = 0;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			bool present; std::cin >> present;
			if (present){
				adder.add_edge(n*n+i*n+j, v_sink, 1);
				num_present++;
				if ((i + j)%2 == 0){
					adder.add_edge(v_source, n*i+j, 1);
					add_edge(adder, i*n+j, (i-1), (j-2), n);
					add_edge(adder, i*n+j, (i-1), (j+2), n);
					add_edge(adder, i*n+j, (i+1), (j-2), n);
					add_edge(adder, i*n+j, (i+1), (j+2), n);
					add_edge(adder, i*n+j, (i-2), (j-1), n);
					add_edge(adder, i*n+j, (i-2), (j+1), n);
					add_edge(adder, i*n+j, (i+2), (j-1), n);
					add_edge(adder, i*n+j, (i+2), (j+1), n);
				}
			}
		}
	}
	int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
	std::cout << num_present - flow << "\n";

}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
