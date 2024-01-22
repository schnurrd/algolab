#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	// TODO:
	int n, k; std::cin >> n >> k;
	std::vector<int> cards(n);
	for (int i = 0; i < n; i++){
		std::cin >>cards[i];
	}
	int pointer_i=0;
	int pointer_j=0;
	int current_val = cards[0];
	std::pair<int, int> solution;
	int min_diff = std::numeric_limits<int>::max();
	while(pointer_i < n){
		if(min_diff > std::abs(current_val - k) and pointer_i <= pointer_j){
			min_diff = std::abs(current_val - k);
			solution = std::make_pair(pointer_i, pointer_j);
		}
		if ((current_val < k and pointer_j < n-1) or (pointer_i > pointer_j)){
			pointer_j++;
			current_val += cards[pointer_j];
		} else {
			current_val -= cards[pointer_i];
			pointer_i++;
		}
	}
	std::cout << solution.first << " " << solution.second << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
