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
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n) {
	// TODO:
	int d; std::cin >> d; //n number of inequalities, d dimension
	Program lp (CGAL::SMALLER, false, 0, false, 0); 
	const int R = d; 
	lp.set_l(R, true, 0);
	int eq = 0;
	for (int i = 0; i < n; i++){
		long norm = 0;
		for (int j = 0; j < d; j++){
			int a_j; std::cin >> a_j;
			lp.set_a(j, eq,  a_j);
			norm += a_j * a_j;
		}
		norm = std::sqrt(norm);
		lp.set_a(R, eq,  norm);
		int b_i; std::cin >> b_i;
		lp.set_b(eq, b_i);
		eq++;
	}
	lp.set_c(R, -1);
	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_unbounded()){
		std::cout << "inf" << "\n";
	} else if (s.is_infeasible()){
		std::cout << "none" << "\n";
	} else {
		std::cout << (int) - CGAL::to_double(s.objective_value()) << "\n";
	}
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	while(t != 0){
		testcase(t);
		std::cin >> t;
	}
}
