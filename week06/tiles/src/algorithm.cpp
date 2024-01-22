#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void testcase() {
	// TODO:
	int w, h; std::cin >> w >> h;
	graph G(w*h);
	std::vector<std::vector<bool>> positions(h, std::vector<bool>(w, false));
	int number_pos = 0;
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			char val; std::cin >> val;
			if(val == '.'){
				positions[i][j] = true;
				number_pos++;
			}
		}
	}
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			if (!positions[i][j]){
				continue;
			}
			// edge down
			if (i+1 < h and positions[i+1][j]){
				boost::add_edge(i*w+j, (i+1)*w+j, G);
			}
			// edge to the right
			if (j+1 < w and positions[i][j+1]){
				boost::add_edge(i*w+j, i*w+j+1, G);
			}
		}
	}
	std::vector<vertex_desc> mate_map(w*h);  // exterior property map
  	boost::edmonds_maximum_cardinality_matching(G,
    	boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
	int matching_size = boost::matching_size(G,
		boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
	if (number_pos%2 == 0 and matching_size == number_pos/2){
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
