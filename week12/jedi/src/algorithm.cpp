///3
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

// BGL includes
#include <boost/pending/disjoint_sets.hpp>

int n;

int construct_mst(const std::vector<std::tuple<int, int, int>> edges){
	std::vector<std::pair<int, int>> edges_in_mst;
	boost::disjoint_sets_with_storage<> uf(n);
	int n_components = n;
	for (std::tuple<int, int, int> edge: edges){
		int c1 = uf.find_set(std::get<1>(edge));
		int c2 = uf.find_set(std::get<2>(edge));
		if (c1 != c2) {
			// this edge connects two different components => part of the emst
			uf.link(c1, c2);
			edges_in_mst.push_back({std::get<1>(edge), std::get<2>(edge)});
			if (--n_components == 1) break;
		}
	}
	int min_val = std::numeric_limits<int>::max();
	for (std::pair<int, int> edge_rem: edges_in_mst){
		boost::disjoint_sets_with_storage<> uf(n);
		int n_components = n;
		int cost = 0;
		for (std::tuple<int, int, int> edge: edges){
			if ((std::get<1>(edge) == edge_rem.first or std::get<1>(edge) == edge_rem.second) and 
				(std::get<2>(edge) == edge_rem.first or std::get<2>(edge) == edge_rem.second)){
					continue;
				}
			int c1 = uf.find_set(std::get<1>(edge));
			int c2 = uf.find_set(std::get<2>(edge));
			if (c1 != c2) {
				// this edge connects two different components => part of the emst
				uf.link(c1, c2);
				cost += std::get<0>(edge);
				if (--n_components == 1) break;
			}
		}
		min_val = std::min(min_val, cost);
	}
	return min_val;

}
void testcase() {
	// TODO:
	int i; std::cin >> n >> i; // number of planets, index of Tatooine (start)
	std::vector<std::tuple<int, int, int>> edges;
	for (int i = 0; i < n-1; i++){
		for (int j = i+1; j < n; j++){
			int cost_i_j; std::cin >> cost_i_j; //cost between planet i and j
			edges.push_back({cost_i_j, i, j});
		}
	}
	std::sort(edges.begin(), edges.end(),
	[](const std::tuple<int, int, int>& e1, const std::tuple<int, int, int>& e2) -> bool {
		return std::get<0>(e1) < std::get<0>(e2);
		});
	std::cout << construct_mst(edges) << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
