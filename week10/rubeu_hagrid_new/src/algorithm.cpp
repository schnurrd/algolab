#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n; 
std::vector<std::vector<std::pair<int, int>>> adj_list;
std::vector<int> number_of_gold_in_chamber;
std::vector<std::pair<int, int>> num_nodes_and_needed_time;

// returns num_nodes, needed_time
std::pair<int, int> solve(int node){
	int num_nodes = 1;
	int needed_time = 0;
	std::vector<std::pair<int, int>>& neighbors = adj_list[node];
	for (int i = 0; i < (int) neighbors.size(); i++){
		auto result = solve(neighbors[i].first);
		num_nodes += result.first;
		needed_time += result.second;
		needed_time += neighbors[i].second;
	}
	num_nodes_and_needed_time[node] = {num_nodes, needed_time};
	return {num_nodes, needed_time};
	//sort
}
// returns score, needed_time
std::pair<long, int> solve2(int node, int time){
	long score = number_of_gold_in_chamber[node]-time;
	std::vector<std::pair<int, int>>& neighbors = adj_list[node];
	std::vector<std::pair<double, int>> scores_of_neighbors(neighbors.size());
	for (int i = 0; i < (int) neighbors.size(); i++){
		int node_id = neighbors[i].first;
		scores_of_neighbors[i] = {-num_nodes_and_needed_time[node_id].first/
			(double)(num_nodes_and_needed_time[node_id].second + neighbors[i].second), i};
	}
	std::sort(scores_of_neighbors.begin(), scores_of_neighbors.end());
	for (auto node : scores_of_neighbors){
		auto neighbor = neighbors[node.second];
		time += neighbor.second; //down
		auto result = solve2(neighbor.first, time);
		time = result.second + neighbor.second; //up
		score += result.first;
	}
	return {score, time};
	//sort
}


void testcase() {
	// TODO:
	std::cin >> n; // number of chambers
	adj_list = std::vector<std::vector<std::pair<int, int>>>(n+1);
	number_of_gold_in_chamber = std::vector<int>(n+1, 0);
	num_nodes_and_needed_time = std::vector<std::pair<int, int>> (n+1);
	for (int i = 1; i <= n; i++){
		std::cin >> number_of_gold_in_chamber[i];
	}
	//starting point 0 not included in chambers
	for (int i = 0; i < n; i++){
		int u, v, l; std::cin >> u >> v >> l;  
		adj_list[u].push_back(std::make_pair(v, l));
	}
	solve(0);
	auto score_and_time = solve2(0, 0);
	std::cout << score_and_time.first << "\n";
	// chose number nodes/time of subtree
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
