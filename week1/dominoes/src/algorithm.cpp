#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
	// TODO:
	int n; std::cin >> n;
	std::vector<int> domino_height(n);
	for (int i = 0; i < n; i++){
		std::cin >> domino_height[i];
	}
	int current_length = domino_height[0];
	int num_fallen_dominos = 1;
	for (int i = 1; i < n; i++){
		current_length--;
		if (current_length > 0){ // domino falls
			current_length = std::max(current_length, domino_height[i]);
			num_fallen_dominos++;
		} else {
			break;
		}
	}
	std::cout << num_fallen_dominos << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
