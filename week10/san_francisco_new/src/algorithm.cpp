#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	// TODO:
	int n, m, k; long x; std::cin >> n >> m >> x >> k;
	std::vector<std::vector<std::pair<int, int>>> adj_list(n);
	std::vector<long> current_best(n, 0);
	std::vector<long> last_best(n, 0);
	for (int i = 0; i < m; i++){
		int u, v, p; std::cin >> u >> v >> p;
		adj_list[u].push_back(std::make_pair(v, p));
	}
	for (int i = 0; i < k; i++){
		for (int j = 0; j < n; j++){
			long current_best_score = 0;
			if (adj_list[j].empty()){
				for (std::pair<int, int> new_node_and_weight : adj_list[0]){
					current_best_score = std::max(current_best_score, 
						last_best[new_node_and_weight.first] + 
						new_node_and_weight.second);
				
				}
			} else {
				for (std::pair<int, int> new_node_and_weight : adj_list[j]){
					current_best_score = std::max(current_best_score, 
						last_best[new_node_and_weight.first] + 
						new_node_and_weight.second);
				}
			}
			current_best[j] = current_best_score;
		}
		std::swap(current_best, last_best);
		if (last_best[0] >= x){
			std::cout << i+1 << "\n";
			return;
		}
	}
	std::cout << "Impossible" << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
