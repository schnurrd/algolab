#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>

void testcase() {
	// TODO:
	int n; std::cin >> n; // number of balls <= 65536
	std::multiset<int> times;
	int max_level = (int) std::log2(n+1);
	//time + upper_bound, when needed to solve
	std::vector<std::vector<std::pair<int, int>>> times_of_bombs_per_level(max_level, std::vector<std::pair<int, int>>());

	for (int i = 0; i < max_level; i++){
		for (int j = 0; j < std::pow(2, i); j++){
			int time; std::cin >> time;
			times_of_bombs_per_level[i].push_back(std::make_pair(time, std::numeric_limits<int>::max()));
		}
	} //use greedy based on levels
	for (int i = 0; i < max_level; i++){
		for (int j = 0; j < std::pow(2, i); j++){
			std::pair<int, int> item = times_of_bombs_per_level[i][j];
			int time_explosion = item.first;
			int upper_bound = item.second;
			int needs_to_be_solved_at = std::min(time_explosion - 1, upper_bound);
			times.insert(needs_to_be_solved_at);
			
			if (i < max_level-1){
				times_of_bombs_per_level[i+1][2*j] = std::make_pair(times_of_bombs_per_level[i+1][2*j].first, needs_to_be_solved_at-1);
				times_of_bombs_per_level[i+1][2*j+1] = std::make_pair(times_of_bombs_per_level[i+1][2*j+1].first, needs_to_be_solved_at-1);
			}
		}
	}
	int i = 0;
	bool possible = true;
	for (int val: times){
		if (i <= val){
			i++;
		} else{
			possible = false;
			break;
		}
	}
	if (possible){
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
