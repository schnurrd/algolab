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
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
	Program lp (CGAL::SMALLER, false, 0, false, 0); 
	const int b = 0;
	const int c = 1;
	const int d = 2;
	const int r = 3;
	//lp.set_l(s_a, true, 0);
	//lp.set_l(s_b, true, 0);
	// TODO:
	int n, m; long s; std::cin >> n >> m >> s;
	// number of noble houses, common houses, max allowale sum of lengths of all sewage pipes
	int eq = 0;
	std::vector<std::pair<int, int>> houses(n+m);
	long sum_x = 0;
	long sum_y = 0;
	for (int i = 0; i < n; i++){
		// positon of the nobel houses
		int x, y; std::cin >> x >> y;
		lp.set_a(b, eq, -y);
		lp.set_a(c, eq, -1);
		lp.set_b(eq, -x); //-by -c <= -x
		houses[i] = std::make_pair(x, y);
		sum_x -= x;
		sum_y += y;
		eq++;
	}
	for (int i = 0; i < m; i++){
		//position of the common houses
		int x, y; std::cin >> x >> y;
		lp.set_a(b, eq, y);
		lp.set_a(c, eq, 1);
		lp.set_b(eq, x); //by + c <= x
		houses[n+i] = std::make_pair(x, y);
		sum_x += x;
		sum_y -= y;
		eq++;
	}
  	Solution sol = CGAL::solve_linear_program(lp, ET());
	if (sol.is_infeasible()){
		std::cout << "Yuck!" << "\n";
		return;
	}
	if (s != -1){
		//check for bankrupt
		lp.set_a(b, eq, sum_y);
		lp.set_a(c, eq, (n-m));
		lp.set_b(eq, s-sum_x); // b * sum_y + c (n-m) <= s - sum_x
		eq++;
		sol = CGAL::solve_linear_program(lp, ET());
		if (sol.is_infeasible()){
			std::cout << "Bankrupt!" << "\n";
			return;
		}
	}

	// minimize max dist
	for (auto house : houses){
		lp.set_a(b, eq, -house.first);
		lp.set_a(d, eq, 1);
		lp.set_a(r, eq, -1);
		lp.set_b(eq, house.second); //-bx_i+d-r <= y_i
		eq++;

		lp.set_a(b, eq, house.first);
		lp.set_a(d, eq, -1);
		lp.set_a(r, eq, -1);
		lp.set_b(eq, -house.second); //bx_i-d-r <= -<_i
		eq++;
	}
	lp.set_c(r, 1);
	sol = CGAL::solve_linear_program(lp, ET());
	std::cout << (long) std::ceil(CGAL::to_double(sol.objective_value())) << "\n";	
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
