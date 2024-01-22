#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

void testcase(int n) {
	// TODO:
	std::vector<P> pts(n);
	for (int i = 0; i < n; i++){
		int x_i, y_i; std::cin >> x_i >> y_i;
		pts[i] = P(x_i, y_i);
	}
	Triangulation t;
	t.insert(pts.begin(), pts.end());
	int m; std::cin >> m; // number of new locations
	for (int i = 0; i < m; i++){
		int x_i, y_i; std::cin >> x_i >> y_i;
		std::cout << (long) CGAL::squared_distance(t.nearest_vertex(P(x_i, y_i))->point(), P(x_i, y_i)) << "\n";
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
