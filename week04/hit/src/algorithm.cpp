#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;

void testcase(int n) {
	// TODO:
	long x, y, a, b; std::cin >> x >> y >> a >> b;

	IK::Point_2 start_point(x, y), middle_point(a, b);
	EK::Ray_2 shot_ray(EK::Point_2(x,y), EK::Point_2(a,b));
	bool hit = false;
	// (x, y) starting pos of ray, (a, b) another point on ray
	for (int i = 0; i < n; i++){
		long r, s, t, u; std::cin >> r >> s >> t >> u;
		if (hit){
			continue;
		}
		IK::Point_2 point_1(r, s), point_2(t, u);
		if (CGAL::orientation(start_point, middle_point, point_1) != CGAL::orientation(start_point, middle_point, point_2) or CGAL::orientation(start_point, middle_point, point_2) == CGAL::COLLINEAR){
			if(CGAL::do_intersect(shot_ray, EK::Segment_2(EK::Point_2(r, s), EK::Point_2(t, u)))){
				hit = true;
			}
		}
		// (r, s) and (t, u) are endpoints of segment
	}
	if(hit){
		std::cout << "yes" << "\n";
	} else {
		std::cout << "no" << "\n";
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
