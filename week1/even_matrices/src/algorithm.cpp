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
	std::vector<std::vector<int>> matrix(n+1, std::vector<int>(n+1, 0));
	for (int i = 1; i <= n; i++){
		for (int j = 1; j <= n; j++){
			int val; std::cin >> val;
			matrix[i][j] = (val + matrix[i-1][j] + matrix[i][j-1] - matrix[i-1][j-1])%2;
		}
	}
	int num_sol = 0;
	for (int i = 0; i <= n; i++){
		for (int i2 = i+1; i2 <= n; i2++){
			int num_even = 1;
			int num_odd = 0;
			for (int j = 1; j <= n; j++){
				if ((matrix[i2][j] - matrix[i][j])%2 == 0){
					num_sol += num_even;
					num_even++;
				} else {
					num_sol += num_odd;
					num_odd++;
				}

			}
		}
	}
	std::cout << num_sol << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
