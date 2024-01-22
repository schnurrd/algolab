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
typedef double ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
	// TODO:
	Program lp (CGAL::LARGER, true, 0, true, 1); 
	const int h_1 = 0; 
  	const int h_2 = 1;
  	const int h_3 = 2;  
	int n; std::cin >> n; // number of stalls
	std::vector<std::vector<int>> values(n, std::vector<int>(3));
	for (int i = 0; i < n; i++){
		//int f, c, k, l, m; 
		int f, c;
		std::cin >> f >> c >> values[i][0] >> values[i][1] >> values[i][2]; 
		// f_i initial amount of filth, maximum allowable amount c_i of filfth
		// amount of filth removed form the i-th stall is h_1*k_1+h_2*l_i+h_3*m_i
		// h_1, h_2, h_3 are freely selectable real numbers between 0 and 1 (size of the holes)
		// k_i, l_i, m_i 
		lp.set_a(h_1, i, values[i][0]); lp.set_a(h_2, i, values[i][1]); lp.set_a(h_3, i, values[i][2]); lp.set_b(i, (f-c));
	}
	std::vector<int> cum_sum_water_from_alpheius(25, 0);
	int cum_sum = 0;
	for (int i = 1; i < 25; i++){
		int val; std::cin >> val;
		cum_sum += val;
		cum_sum_water_from_alpheius[i] = cum_sum;
	}
	std::vector<int> cum_sum_water_from_peneius(24);
	cum_sum = 0;
	for (int i = 1; i < 25; i++){
		int val; std::cin >> val;
		cum_sum += val;
		cum_sum_water_from_peneius[i] = cum_sum;
	}
	Solution s;
	s = CGAL::solve_linear_program(lp, ET());
	if(s.is_infeasible()){
		int low = 1, high = 48;
		int result = -1; // Initialize result to an invalid value
		std::vector<int> impossible_i(25, -1);
		while (low <= high) {
			int mid = low + (high - low) / 2;

			// Replace the for-loop with the logic to test if 'mid' is a valid solution
			// If the condition is met for 'mid', try to find a smaller value
			bool possilbe = false;
			for(int i = 0; i < std::min(25, mid+1);i++){
				int j = std::min(24, mid - i);
				if(impossible_i[i] != -1 and impossible_i[i] >= j){
					continue;
				}
				if ((i > 0 and (cum_sum_water_from_alpheius[i] == cum_sum_water_from_alpheius[i-1])) or
					(j < std::min(24, mid - i) and (cum_sum_water_from_peneius[j] == cum_sum_water_from_peneius[j+1])) ){
					continue;
				}
				long a = cum_sum_water_from_alpheius[i];
				long p = cum_sum_water_from_peneius[j];
				for (int k = 0; k < n; k++){
					lp.set_a(h_1, k, (values[k][0]+a*a)); lp.set_a(h_2, k, (values[k][1]+p*p)); lp.set_a(h_3, k, (values[k][2]+a*p)); 
				}
				s = CGAL::solve_linear_program(lp, ET());

				if (!s.is_infeasible()) {
					possilbe = true;
					break;
				} else {
					for(int adjust = i; adjust < 25; adjust++){
						if (impossible_i[i] > j){
							break;
						}
						impossible_i[i] = j;
					}
				}
			}
			if (possilbe) {
				result = mid; // Store the current hour as a possible solution
				high = mid - 1; // Try to find a smaller valid hour
			} else {
				low = mid + 1; // Try to find a valid hour greater than 'mid'
			}
		}
		if(result != -1){
			std::cout << result << "\n";
		} else {
			std::cout << "Impossible!" << "\n";
		}

	} else{
		std::cout << 0 << "\n";
	}
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
