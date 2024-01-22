#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iterator>

void testcase() {
	// TODO:
	int n, m; std::cin >> n >> m; //number of friends, number of boxes
	std::vector<int> strength_per_person(n);
	for (int i = 0; i < n; i++){
		std::cin >> strength_per_person[i];
	}
	std::sort(strength_per_person.begin(), strength_per_person.end());
	std::vector<int> box_weights(m);
	for (int i = 0; i < m; i++){
		std::cin >> box_weights[i];
	}
	std::vector<int> boxes_per_person(n, 0);
	bool possible = true;
	for (int box_weight : box_weights){
		auto person = std::lower_bound(strength_per_person.begin(), 
										strength_per_person.end(), box_weight);
		if (person == strength_per_person.end()){
			//impossible
			possible = false;
		} else {
			boxes_per_person[(int) std::distance(strength_per_person.begin(), person)]++;
		}
	}
	if (possible){
		int max_num_packages_per_person = 0;
		int overall_number_of_packages = 0;
		for (int i = n-1; i >= 0; i--){
			overall_number_of_packages += boxes_per_person[i];
			max_num_packages_per_person = std::max(max_num_packages_per_person, (int) std::ceil(overall_number_of_packages/((double) n-i)));
		}
		std::cout << std::max((max_num_packages_per_person-1)*3 + 2, 0) << "\n";
	} else {
		std::cout << "impossible" << "\n";
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
