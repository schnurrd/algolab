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
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase() {
	// TODO:
	int n, m, h, w; std::cin >> n >> m >> h >> w;
	std::vector<std::pair<int, int>> new_poster(n);
	for (int i = 0; i < n; i++){ // new poster
		int x, y; std::cin >> x >> y;
		new_poster[i] = std::make_pair(x, y);
	}
	std::vector<std::pair<int, int>> old_poster(m);
	for (int i = 0; i < m; i++){ // old poster
		int x, y; std::cin >> x >> y; 
		old_poster[i] = std::make_pair(x, y);
	}

	Program lp (CGAL::SMALLER, true, 1, false, 0); 
	int eq = 0;
	for (int i1 = 0; i1 < n; i1++){
		// min_dist to new_poster
		long min_dist = std::numeric_limits<long>::max();
		for (int i2 = 0; i2 < n; i2++){
			long diff_x = std::abs(new_poster[i1].first - new_poster[i2].first);
			long diff_y = std::abs(new_poster[i1].second - new_poster[i2].second);
			long max_val = std::max(diff_x * h, diff_y * w);
			if (max_val > min_dist or i1 == i2) continue;
			if (diff_x * h >= diff_y * w){
				lp.set_a(i1, eq,    w); lp.set_a(i2,     eq, w); lp.set_b(eq,   2 * diff_x);
			} else {
				lp.set_a(i1, eq,    h); lp.set_a(i2,     eq, h); lp.set_b(eq,   2 * diff_y);
			}
			min_dist = std::min(min_dist, 2*max_val);
			eq++;
		}
		long closest_dist_to_old_poster = std::numeric_limits<long>::max();
		long closest_dist_to_old_poster_u = std::numeric_limits<long>::max();
		bool x_direction = true;
		// min_dist to old_poster
		for (int j = 0; j < m; j++){
			long diff_x = std::abs(new_poster[i1].first - old_poster[j].first);
			long diff_y = std::abs(new_poster[i1].second - old_poster[j].second);
			if (closest_dist_to_old_poster > std::max(diff_x*h, diff_y*w)){
				closest_dist_to_old_poster = std::max(diff_x*h, diff_y*w);
				if (diff_x*h > diff_y*w){
					closest_dist_to_old_poster_u = diff_x;
					x_direction = true;
				} else{
					closest_dist_to_old_poster_u = diff_y;
					x_direction = false;
				}
			}
		}
		if (closest_dist_to_old_poster_u != std::numeric_limits<long>::max()){
			lp.set_a(i1, eq, x_direction ? w : h); lp.set_b(eq, 2*closest_dist_to_old_poster_u - (x_direction ? w : h));
			eq++;
		}
		lp.set_c(i1, -1);
	}
  	Solution s = CGAL::solve_linear_program(lp, ET());
	std::cout << (long) std::ceil(2*(w + h)*-CGAL::to_double(s.objective_value())) << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
