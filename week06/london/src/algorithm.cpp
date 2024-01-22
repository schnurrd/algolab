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
	int h, w; std::cin >> h >> w;
	std::vector<int> num_occurences_of_char_in_letter(26, 0);
	std::string note; std::cin >> note;
	for (char character : note){
		num_occurences_of_char_in_letter[character-65]++;
	}
	graph G(26*26+26);
	edge_adder adder(G);
	const vertex_desc v_source = boost::add_vertex(G);
  	const vertex_desc v_sink = boost::add_vertex(G);
	std::vector<std::vector<int>> letter(h, std::vector<int>(w));
	std::vector<std::vector<int>> occurences_tupel(26, std::vector<int>(26, 0));
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			char val; std::cin >> val;
			letter[i][j] = val-65;
		}
	}
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			char val; std::cin >> val;
			int letter_on_other_side = letter[i][w-1-j];
			occurences_tupel[letter_on_other_side][val-65]++;
		}
	}
	for (int i = 0; i < 26; i++){
		for (int j = 0; j < 26; j++){
			int num_occurences_of_letter_i_and_j = occurences_tupel[i][j];
			adder.add_edge(v_source, i*26 + j, num_occurences_of_letter_i_and_j);
			adder.add_edge(i*26 + j, 26*26 + i, num_occurences_of_letter_i_and_j);
			adder.add_edge(i*26 + j, 26*26 + j, num_occurences_of_letter_i_and_j);
		}
	}
	for (int i = 0; i < 26; i++){
		adder.add_edge(26*26 + i, v_sink, num_occurences_of_char_in_letter[i]);
	}
	long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
	if (flow == (int) note.length()){
		std::cout << "Yes" << "\n";
	} else{
		std::cout << "No" << "\n";
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
