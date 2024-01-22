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
	int n, m; std::cin >> n >> m; //number of players and number of rounds
	std::vector<std::pair<int, int>> undecided_games;
	std::vector<int> wins_of_player(n, 0);
	for (int i = 0; i < m; i++){
		int a, b, c; std::cin >> a >> b >> c;
		if (c == 1){
			wins_of_player[a]++;
		} else if (c == 2){
			wins_of_player[b]++;
		} else {
			undecided_games.push_back(std::make_pair(a, b));
		}
		// c = 1 -> player a won, c = 2 player b won, c = 0 did not write down
	}
	int number_undecided_games = undecided_games.size();
	graph G(number_undecided_games + n);
	edge_adder adder(G);
	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_sink = boost::add_vertex(G);
	for (int i = 0; i < number_undecided_games; i++){
		adder.add_edge(v_source, i, 1);
		adder.add_edge(i, number_undecided_games + undecided_games[i].first, 1);
		adder.add_edge(i, number_undecided_games + undecided_games[i].second, 1);
	}
	int needed_flow = 0;
	int overall_flow = 0;
	bool possible = true;
	for (int i = 0; i < n; i++){
		int s_i; std::cin >> s_i;
		if (s_i - wins_of_player[i] < 0){
			possible = false;
		}
		needed_flow += s_i - wins_of_player[i];
		overall_flow += s_i;
		adder.add_edge(number_undecided_games + i, v_sink, s_i - wins_of_player[i]);
	}
	int flow = -1;
	if (possible){
		flow = boost::push_relabel_max_flow(G, v_source, v_sink);
	}
	if (overall_flow == m and flow == needed_flow){
		std::cout << "yes" << "\n";
	} else {
		std::cout << "no" << "\n";
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
