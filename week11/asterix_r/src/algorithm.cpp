#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

// example: how to solve a simple explicit LP
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

void testcase() {
	// TODO:
	// create an LP with Ax <= b, lower bound 0 and no upper bounds
  	Program lp (CGAL::SMALLER, false, 0, false, 0); 
	
	const int M = 0;
	const int X = 1; 
  	const int Y = 2;

	lp.set_l(M, true, 0);
	lp.set_c(M, -1);

	long x_s, y_s, n; std::cin >> x_s >> y_s >> n; //location of asterix and panoramix, number of legions
	for (int i = 0; i < n; i++){
		long a, b, c, v; std::cin >> a >> b >> c >> v; //equation of straight line where the legion is positioned ax+by+c=0, speed of legion
		long normalization = std::sqrt(a*a + b*b);
		long sign = (a*x_s + b*y_s + c) >= 0 ? 1 : -1;  
		lp.set_a(X, i, -sign*a); lp.set_a(Y, i, -sign*b); lp.set_a(M, i, normalization*v);lp.set_b(i, sign*c);
	}
	// solve the program, using ET as the exact type
	Solution s = CGAL::solve_linear_program(lp, ET());
	
	// output solution
	std::cout << (long)std::floor(CGAL::to_double(-s.objective_value())) << "\n"; 
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
