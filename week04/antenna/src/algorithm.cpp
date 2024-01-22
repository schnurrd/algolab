#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 P;

double ceil_to_double(const K::FT& x){
 double a = std::ceil(CGAL::to_double(x));
 while (a < x) a += 1;
 while (a-1 >= x) a -= 1;
 return a;
}

void testcase(int n) {
	// TODO:
	std::vector<P> citizen(n);
	for (int i = 0; i < n; i++){
		long x_i, y_i; std::cin >> x_i >> y_i;
		citizen[i] = P(x_i, y_i);
	}
	Min_circle mc(citizen.begin(), citizen.end(), true);
	Traits::Circle c = mc.circle();
	std::cout << std::setprecision(0) << std::fixed << ceil_to_double(CGAL::sqrt(c.squared_radius())) << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	while(t!=0){
		testcase(t);
		std::cin >> t;
	}
}
