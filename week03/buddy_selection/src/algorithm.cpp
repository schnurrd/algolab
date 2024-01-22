#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void testcase() {
	// TODO:
	int n, c, f; std::cin >> n >> c >> f; //number of students, number of charactersitics, number of common characteristics
	std::vector<std::unordered_set<std::string>> student_to_char(n, std::unordered_set<std::string>());
	for (int i = 0; i < n; i++){ //students
		for (int j = 0; j < c; j++){ //characteristics
			std::string characteristic; std::cin >> characteristic;
			student_to_char[i].insert(characteristic);
		}
	}
	graph G(n);
	for (int i = 0; i < n; i++){
		for (int j = i + 1; j < n; j++){
			int count = 0;
			for (auto chara:student_to_char[i]){
				if (student_to_char[j].find(chara) != student_to_char[j].end()){
					count++;
				}
			}
			if (count > f){
				boost::add_edge(i, j, G);
			}
		}
	}
	std::vector<vertex_desc> mate_map(n);  // exterior property map
	const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

	boost::edmonds_maximum_cardinality_matching(G,
		boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
	int matching_size = boost::matching_size(G,
		boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

	if (matching_size == n/2){
		std::cout << "not ";
	}
	std::cout << "optimal" << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
