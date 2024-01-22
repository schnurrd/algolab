#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Vector_2 V;
typedef K::Ray_2 R;

CGAL::Gmpq get_slope(const std::tuple<int64_t,int64_t,int64_t, int> &t) {
  const int64_t y_start = std::get<0>(t);
  const int64_t x_direction = std::get<1>(t);
  const int64_t y_direction = std::get<2>(t);

  return CGAL::Gmpq(y_direction-y_start, x_direction);
}

void testcase() {
	// TODO:
	int n; std::cin >> n; // number of bikers
	// starting y, direction x, direction y, id
	std::vector<std::tuple<int64_t,int64_t,int64_t,int>> bikers(n);
	for(int i = 0; i < n; i++) {
		std::cin >> std::get<0>(bikers[i]) >> std::get<1>(bikers[i]) >> std::get<2>(bikers[i]);
		std::get<3>(bikers[i]) = i;
	}
	std::sort(std::begin(bikers), std::end(bikers), [](const auto &first, const auto &second){
    	return std::get<0>(first) < std::get<0>(second);
  	});
	std::vector<bool> possible(n, true);
  	std::vector<CGAL::Gmpq> slopes;
	for(int i = 0; i < n; i++) {
    	slopes.push_back(get_slope(bikers[i]));
  	}
	// Iterate from bot to top
	CGAL::Gmpq best_slope = slopes.front();
	for(int i = 1; i < n; i++) {
		if (CGAL::abs(slopes[i]) < CGAL::abs(best_slope) || (CGAL::abs(slopes[i]) == CGAL::abs(best_slope) && slopes[i] > best_slope)) {
			best_slope = slopes[i];
		} else if (slopes[i] < best_slope){
			possible[i] = false;
		}
	}
	// Iterate from top to bot
	best_slope = slopes.back();
	for(int i = n-2; i >= 0; i--) {
		if (CGAL::abs(slopes[i]) <= CGAL::abs(best_slope)) {
			best_slope = slopes[i];
		} else if (slopes[i] > best_slope){
			possible[i] = false;
		}
	}
	// Print results
	std::vector<int> results;
	for(int i = 0; i < n; i++) {
		if (possible[i]) {
			results.push_back(std::get<3>(bikers[i]));
		}
	}

	std::sort(std::begin(results), std::end(results));
	for(const int r : results) {
		std::cout << r << " ";
	}
	std::cout << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
