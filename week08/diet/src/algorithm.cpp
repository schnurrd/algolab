#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n) {
	Program lp (CGAL::SMALLER, true, 0, false, 0); 
	// TODO:
	int m; std::cin >> m;
	std::vector<std::pair<int, int>> nutrients(n);
	for (int i = 0; i < n; i++){ //nutrients
		int min_i, max_i; std::cin >> min_i >> max_i;
		nutrients[i] = std::make_pair(min_i, max_i);
	}
	std::vector<int> prices(m);
	std::vector<std::vector<int>> nutrients_per_food(m, std::vector<int>(n));
	for (int i = 0; i < m; i++){ //foods
		std::cin >> prices[i]; //price
		for (int j = 0; j < n; j++){
			std::cin >> nutrients_per_food[i][j];
		}
	}
	int eq = 0;
	for (int i = 0; i < n; i++){
		// constrain nutrients
		for (int j = 0; j < m; j++){
			lp.set_a(j, eq,  nutrients_per_food[j][i]);
			lp.set_a(j, eq+1,  -nutrients_per_food[j][i]);
		}
		lp.set_b(eq, nutrients[i].second);
		lp.set_b(eq+1, -nutrients[i].first);
		eq += 2;
	}
	for (int i = 0; i < m; i++){
		lp.set_c(i, prices[i]);  
	}
  	Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
	if (s.is_infeasible()){
		std::cout << "No such diet." << "\n";
	} else {
		std::cout << (long) std::floor(CGAL::to_double(s.objective_value())) << "\n";
	}
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	while (t != 0){
		testcase(t);
		std::cin >> t;
	}
}
