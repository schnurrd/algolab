#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

void testcase() {
	// TODO:
	int m, n; std::cin >> m >> n;
	std::vector<P> points(m);
	for (int i = 0; i < m; i++){ // paths
		int x_i, y_i; std::cin >> x_i >> y_i; //hiking path p_i
		points[i] = P(x_i, y_i);
		// modeled as a straigt path from p_i to p_(i+1)
	}
	std::vector<P> points_triangle(6);
	std::vector<std::vector<int>> map_contains(n);
	for (int i = 0; i < n; i++){
		for (int j = 0; j < 6; j++){
			int x_i_j, y_i_j; 
			std::cin >> x_i_j >> y_i_j; //one edge of map
			points_triangle[j] = P(x_i_j, y_i_j);
		}
		//order points
		for (int j = 0; j < 3; j++){
			if (CGAL::orientation(points_triangle[2*j], points_triangle[2*j+1], points_triangle[(2*(j+1))%6]) != CGAL::RIGHT_TURN){
				std::swap(points_triangle[2*j], points_triangle[2*j+1]);
			}
		}

		bool last_inside = false;
		for (int j = 0; j < m; j++){
			bool inside = true;
			for (int k = 0; k < 3; k++){
				if (CGAL::orientation(points_triangle[2*k], points_triangle[2*k+1], points[j]) == CGAL::LEFT_TURN){
					inside = false;
				}
			}
			if (inside and last_inside){
				map_contains[i].push_back(j-1);
			}
			last_inside = inside;
		}
	}
	std::vector<int> currently_contained(m-1);
	int not_contained = m-1;
	int pointer_i = 0;
	int pointer_j = 0;
	int min_cost = std::numeric_limits<int>::max(); 
	while (true){
		if (not_contained == 0){
			min_cost = std::min(min_cost, pointer_i-pointer_j);
			if(pointer_j == n){
				break;
			}
			//remove all pointer in map j
			for (int path: map_contains[pointer_j]){
				currently_contained[path]--;
				if(currently_contained[path] == 0){
					not_contained++;
				}
			}
			pointer_j++;
		} else {
			if(pointer_i == n){
				break;
			}
			//add all pointer in map j
			for (int path: map_contains[pointer_i]){
				currently_contained[path]++;
				if(currently_contained[path] == 1){
					not_contained--;
				}
			}
			pointer_i++;
		}
	}
	std::cout << min_cost << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
