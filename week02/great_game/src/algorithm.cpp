#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n, m;
std::vector<std::vector<int>> adj_matrix;
std::vector<int> memo_min, memo_max;

int max_number_of_moves(int position);

int min_number_of_moves(int position){
	if (memo_min[position] != -1){
		return memo_min[position];
	}
	int min_val = std::numeric_limits<int>::max();
	for (int next_pos: adj_matrix[position]){
		min_val = std::min(min_val, 1 + max_number_of_moves(next_pos));
	}
	memo_min[position] = min_val;
	return min_val;
}

int max_number_of_moves(int position){
	if (memo_max[position] != -1){
		return memo_max[position];
	}
	int max_val = 0;
	for (int next_pos: adj_matrix[position]){
		max_val = std::max(max_val, 1 + min_number_of_moves(next_pos));
	}
	memo_max[position] = max_val;
	return max_val;
}

void testcase() {
	// TODO:
	std::cin >> n >> m; // number of positions, number of transitions
	int r, b; std::cin >> r >> b; // starting pos red and black meeple
	r--; b--; //index shift to start from 0
	adj_matrix = std::vector<std::vector<int>>(n);
	memo_min = std::vector<int>(n, -1);
	memo_max = std::vector<int>(n, -1);
	memo_min[n-1] = 0;
	memo_max[n-1] = 0;
	for (int i = 0; i < m; i++){
		int u, v; std::cin >> u >> v;
		u--; v--; // index shift
		adj_matrix[u].push_back(v);
	}
	int moves_red = min_number_of_moves(r);
	int moves_black = min_number_of_moves(b);
	if (moves_red < moves_black or (moves_red == moves_black and (moves_red % 2) == 1)){
		std::cout << 0 << "\n";
	} else {
		std::cout << 1 << "\n";
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
