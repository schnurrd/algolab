#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >	graph;
// Interior Property Maps
typedef	boost::graph_traits<graph>::edge_descriptor			edge_desc;
typedef	boost::graph_traits<graph>::out_edge_iterator			out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int n; std::vector<std::vector<bool>> board;

void add_edge(edge_adder& adder, int i, int j, int i2, int j2){
	if (i2 < 0 or i2 >= n or j2 < 0 or j2 >=n){
		return;
	}
	//edge from i+j even to odd
	if ((i+j)%2 == 0){
		adder.add_edge(n*i+j, n*(i2)+j2, 1);
	} else {
		adder.add_edge(n*(i2)+j2, n*i+j, 1);
	}
}

void testcase(){
	std::cin >> n; // length of chessboard side, max 65
	// we want the sice of a maximum independent set
	// is V - size maximum matching
	board = std::vector<std::vector<bool>>(n, std::vector<bool>(n));
	int num_vertices = 0; 
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			int c; std::cin >> c;
			board[i][j] = c;
			num_vertices += c;
		}
	}
	graph G(n*n + 2);
	const int src = n*n;
	const int sink = n*n + 1;
	edge_adder adder(G);
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (!board[i][j]) continue;
			if ((i+j)%2 == 0){
				adder.add_edge(src, n*i+j, 1);
			} else {
				adder.add_edge(n*i+j, sink, 1);
			}
			add_edge(adder, i, j, i-1, j-2);
			add_edge(adder, i, j, i-1, j+2);
			add_edge(adder, i, j, i+1, j-2);
			add_edge(adder, i, j, i+1, j+2);
			add_edge(adder, i, j, i-2, j-1);
			add_edge(adder, i, j, i-2, j+1);
			add_edge(adder, i, j, i+2, j-1);
			add_edge(adder, i, j, i+2, j+1);
		}
	}
	int flow = boost::push_relabel_max_flow(G, src, sink);
	std::cout << num_vertices - flow << "\n";

}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
