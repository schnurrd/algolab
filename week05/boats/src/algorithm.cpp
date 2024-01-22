#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	// TODO:
	int n; std::cin >> n; // number of boats
	std::vector<std::pair<int, int>> length_and_position(n);
	for (int i = 0; i < n; i++){
		int l, p; std::cin >> l >> p;
		length_and_position[i] = std::make_pair(l, p);
	}
	std::sort(length_and_position.begin(), length_and_position.end(), 
				[] (std::pair<int, int> a, std::pair<int, int> b) {
					return a.second < b.second;});
	int pointer_i = 1;
	int num_ships = 1;
	int last_ship_pos = length_and_position[0].second;
	while (pointer_i < n){
		int end_next_ship_if_direct = last_ship_pos + length_and_position[pointer_i].first;
		if (end_next_ship_if_direct <= length_and_position[pointer_i].second){
			num_ships++;
			last_ship_pos = length_and_position[pointer_i].second;
			pointer_i++;
		} else {
			// ship is long, look if we find something better
			int pointer_j = pointer_i + 1;
			int min_ship_end = end_next_ship_if_direct;
			while (pointer_j < n and min_ship_end > length_and_position[pointer_j].second){
				int val = std::max(last_ship_pos + length_and_position[pointer_j].first, length_and_position[pointer_j].second);
				if (val < min_ship_end){
					min_ship_end = val;
				}
				pointer_j++;
			}
			num_ships++;
			last_ship_pos = min_ship_end;
			pointer_i = pointer_j; //maybe to much
		}
	}
	std::cout << num_ships << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
