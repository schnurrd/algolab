#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cmath>
#include <boost/math/special_functions/binomial.hpp>

void testcase() {
	// TODO:
	int n; std::cin >> n;
	std::vector<int> numbers(n+1);
	int sum = 0;
	int num_even = 1;
	for (int i = 1; i <= n; i++){
		int val; std::cin >> val;
		sum += val;
		num_even += (sum%2 == 0); 
	}

	std::cout << num_even*(num_even-1)/2 + (n+1-num_even)*((n+1-num_even)-1)/2 << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
