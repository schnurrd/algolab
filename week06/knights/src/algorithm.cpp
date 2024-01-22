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
	int m, n, k, c; std::cin >> m >> n >> k >> c;
	graph G(2*n*m);
	edge_adder adder(G);
	const vertex_desc v_source = boost::add_vertex(G);
	const vertex_desc v_sink = boost::add_vertex(G);
	for (int i = 0; i < n; i++){ // n rows
		for (int j = 0; j < m; j++){ // m colums
			int vertex_pre_c_restric = m*i + j;
			int vertex_post_c_restric = n*m + m*i + j;
			adder.add_edge(vertex_pre_c_restric, vertex_post_c_restric, c);

			// escape top and bottom, left and right
			adder.add_edge(vertex_post_c_restric, v_sink, ((i == 0) + (i == n-1) 
											+ (j == 0) + (j == m-1)));

			// edge to bottom
			if (i != n-1){
				adder.add_edge(vertex_post_c_restric, m*(i+1) + j, 1);
				adder.add_edge(m*(i+1) + j + m*n, vertex_pre_c_restric, 1); // reverse
			}
			
			// edge right
			if (j != m-1){
				adder.add_edge(vertex_post_c_restric, m*i + (j+1), 1);
				adder.add_edge(m*i + (j+1) + m*n, vertex_pre_c_restric, 1); // reverse
			}
		}
	}
	//m number columns, n number rows, k number knights, C intersection durability
	for (int i = 0; i < k; i++){
		int x, y; std::cin >> x >> y;
		adder.add_edge(v_source, x+y*m, 1);
	}
	int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
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
