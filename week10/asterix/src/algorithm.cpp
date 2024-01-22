#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n;
std::vector<int> num_children;
std::vector<std::vector<int>> adj_list;
std::vector<int> repair_costs;

std::tuple<int, int, int> dfs(int node){
	int cost = repair_costs[node];
	int selected_cost = cost;
	int covered_cost = 0;
	int uncovered_cost = 0;
	
	int min_diff = std::numeric_limits<int>::max();
	for (int child : adj_list[node]) { 
		std::tuple<int, int, int> child_values = dfs(child); 
		int selected_cost_child = std::get<0>(child_values);
		int covered_cost_child = std::get<1>(child_values);
		int uncovered_cost_child = std::get<2>(child_values);
		selected_cost += uncovered_cost_child;
		uncovered_cost += covered_cost_child;
		min_diff = std::min(min_diff, selected_cost_child-covered_cost_child);
		
	}
	covered_cost = uncovered_cost + min_diff;
	covered_cost = std::min(covered_cost, selected_cost);
	uncovered_cost = std::min(uncovered_cost, covered_cost);
	return {selected_cost, covered_cost, uncovered_cost};
}

void testcase() {
	std::cin >> n; // number of cities
	adj_list = std::vector<std::vector<int>>(n);
	for (int k = 0; k < n-1; k++){
		int i, j; std::cin >> i >> j; // start and end of stage
		adj_list[i].push_back(j);
	}
	// is a tree
	repair_costs = std::vector<int>(n);
	for (int k = 0; k < n; k++){
		std::cin >> repair_costs[k]; // cost of repairing road in city c_k
	}
	int min_cost = 0;
	if (adj_list[0].size() > 0){
		min_cost = std::get<1>(dfs(0));
	} else {
		min_cost = repair_costs[0];
	}
	std::cout << min_cost << "\n";
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
