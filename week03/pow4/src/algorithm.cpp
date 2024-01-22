#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int n, k, w; 

std::vector<int> dijkstra_dist(const weighted_graph &G) {
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, 0,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map;
}

void testcase() {
	// TODO:
	std::cin >> n >> k >> w; //number of islands, ironborn men, waterways that start at Pyke
	std::vector<int> men_required_per_island(n);
	for (int i = 0; i < n; i++){
		std::cin >> men_required_per_island[i];
	}
	weighted_graph G(n);
	weight_map weights = boost::get(boost::edge_weight, G);
	edge_desc e;
	std::vector<std::vector<int>> water_ways(w, std::vector<int>());
	for (int i = 0; i < w; i++){
		int l; std::cin >> l; //length of waterway
		int r_last; std::cin >> r_last; //always 0
		water_ways[i].push_back(0);
		for (int j = 1; j < l; j++){
			int r_j; std::cin >> r_j; // islands along waterway, r_0 = 0
			water_ways[i].push_back(r_j);
			e = boost::add_edge(r_last, r_j, G).first; weights[e]=men_required_per_island[r_j];
			r_last = r_j;
		}
	}
	std::vector<int> dist_map = dijkstra_dist(G);
	//std::vector<int> best_val(n, std::numeric_limits<int>::max());
	std::unordered_map<int, int> best_val;
	std::vector<std::pair<int, int>> to_insert;
	int num_islands = 0;
	//check each individal waterway
	for (int i = 0; i < w; i++){
		to_insert.clear();
		int pointer_i = 0;
		int pointer_j = 0;
		while (pointer_i < (int) water_ways[i].size()){
			int val_pointer_j = dist_map[water_ways[i][pointer_j]] + men_required_per_island[water_ways[i][pointer_i]];
			int val_pointer_i =  dist_map[water_ways[i][pointer_i]];
			if (pointer_i == 0){
				to_insert.push_back(std::make_pair(val_pointer_j, pointer_j+1));
				auto search = best_val.find(k - val_pointer_j + men_required_per_island[water_ways[i][0]]);
				if (search != best_val.end()){
					num_islands = std::max(num_islands, pointer_j + search->second);
				}
				
			}
			if(val_pointer_j - val_pointer_i == k){
				num_islands = std::max(num_islands, pointer_j-pointer_i+1);
			}
			if(val_pointer_j - val_pointer_i < k and pointer_j+1 < (int) water_ways[i].size()){
				pointer_j++;
			} else {
				pointer_i++;
			}
		}
		for(std::pair<int, int> new_connection: to_insert){
			auto search = best_val.find(new_connection.first);
			if (search != best_val.end()){
				best_val[new_connection.first] = std::max(best_val[new_connection.first], new_connection.second);
			} else {
				best_val[new_connection.first] = new_connection.second;
			}
		}
	}
	std::cout << num_islands << "\n";

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
