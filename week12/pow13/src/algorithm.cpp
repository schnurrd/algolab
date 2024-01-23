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


int n, m; 
std::vector<std::vector<int>> adjacency_list;
std::vector<std::pair<int, std::vector<int>>> mine_info;
std::vector<std::vector<int>> shop_info;
std::vector<std::vector<int>> memo;
int eq;
int current_idx_lp;
Program lp; 
// current_minerals, previous restricion idx in lp
std::pair<std::vector<int>, std::vector<int>> solve(int node){
	std::vector<int> current_minerals = mine_info[node].second;
	std::vector<int> restriced_nodes_below;
	for (auto child : adjacency_list[node]){
		auto return_val = solve(child);
		std::vector<int>& minerals = return_val.first;
		// add minerals
		for (int i = 0; i < m; i++){
			current_minerals[i] += minerals[i];
		}
		std::vector<int>& restriced_nodes_child = return_val.second;
		for (int val: restriced_nodes_child){
			restriced_nodes_below.push_back(val);
		}
	}
	if(mine_info[node].first < 0){ //normal mine
		return std::make_pair(current_minerals, restriced_nodes_below);
	} else { // restricted mine
		for (int i = 0; i < m; i++){
			lp.set_u(current_idx_lp, true, current_minerals[i]);
			lp.set_a(current_idx_lp, eq, 1);
			lp.set_a(current_idx_lp, eq+i+1, -1); // add to new summarization equation
			current_idx_lp++;
		}
		for (int restriced_node: restriced_nodes_below){
			std::vector<int>& restriced_idx_lp = memo[restriced_node];
			for (int i = 0; i < m; i++){
				lp.set_a(restriced_idx_lp[i], eq,  1);
				lp.set_a(restriced_idx_lp[i], eq+i+1, -1); // add to new summarization equation
			}
		}
		lp.set_b(eq, mine_info[node].first);
		//summarize all different minerals
		for (int i = 0; i < m; i++){
			lp.set_a(current_idx_lp, eq+1+i, 2); // introduce the halving
			lp.set_b(eq+1+i, 0);
			memo[node][i] = current_idx_lp;
			current_idx_lp++;
		}
		eq+=(m+1);
		return std::make_pair(std::vector<int>(m, 0), std::vector<int>(1, node));
	}
}

void testcase() {
	// TODO:
	std::cin >> n >> m; //number mines, minerals
	eq = 0;
	current_idx_lp = 0;
	lp = Program(CGAL::SMALLER, true, 0, false, 0); 
	adjacency_list = std::vector<std::vector<int>>(n);
	mine_info = std::vector<std::pair<int, std::vector<int>>>(n);
	shop_info = std::vector<std::vector<int>>(m, std::vector<int>(3));
	//store lp idx if restriced
	memo = std::vector<std::vector<int>> (n, std::vector<int>(m, -1));

	//entry mine is i = 0;
	for (int i = 0; i < n; i++){
		int danger_threshold; std::cin >> danger_threshold; // danger threshold
		// if d_i >= 0 mine is dangerous
		// then no minerals are available
		std::vector<int> mineals(m);
		for (int j = 0; j < m; j++){
			std::cin >> mineals[j]; // quantity of minearal j in mine i
		}
		mine_info[i] = std::make_pair(danger_threshold, mineals);
	}
	for (int i = 0; i < n - 1; i++){
		int u, v; std::cin >> u >> v; //railway track between u and v
		adjacency_list[v].push_back(u);
		//v lies on the (unique) path from u to the entry
	}
	for (int i = 0; i < m; i++){
		int c, s, p; std::cin >> c >> s >> p; 
		shop_info[i][0] = c;
		shop_info[i][1] = s;
		shop_info[i][2] = p;
		//c the amount of minerals, s the supply of the mineral at the shop, p price per unit
	}
	std::pair<std::vector<int>, std::vector<int>> result = solve(0);
	for (int i = 0; i < m; i++){
		shop_info[i][0] -= result.first[i]; 
	}
	// objects to buy
	for (int i = 0; i < m; i++){
		lp.set_u(current_idx_lp, true, shop_info[i][1]);
		lp.set_a(current_idx_lp, eq, -1);
		for (int restriced_node: result.second){
			std::vector<int>& restriced_idx_lp = memo[restriced_node];
			lp.set_a(restriced_idx_lp[i], eq,  -1);
		}
		lp.set_b(eq, -shop_info[i][0]);
		lp.set_c(current_idx_lp, shop_info[i][2]);
		eq++;
		current_idx_lp++;
	}
  	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible()){
		std::cout << "Impossible!" << "\n";
	} else {
		//std::cout << s << "\n";
		std::cout << (long) std::floor(CGAL::to_double(s.objective_value())) << "\n";
	}
	//output price or if not possible: impossible
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
