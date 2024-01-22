#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void sort_vec(std::vector<std::vector<std::pair<long, long>>>& vec_to_sort){
	for (int s = 0; s < vec_to_sort.size(); s++){
		std::sort(vec_to_sort[s].begin(), vec_to_sort[s].end(), 
			[](std::pair<long, long> a, std::pair<long, long> b) { return a.first < b.first or (a.first == b.first and a.second > b.second);});
		long dist = -1; long shift = 0;
		for (int i = 0; i < (int) vec_to_sort[s].size(); i++){
			if (vec_to_sort[s][i].second > dist){
				dist = vec_to_sort[s][i].second;
				vec_to_sort[s][shift] =  vec_to_sort[s][i];
				shift++;
			} 
		}
		vec_to_sort[s].resize(shift);
	}
}

void testcase() {
	// TODO:
	int n, m; long D, T; std::cin >> n >> m >> D >> T;
	// number of movements, amount of magic potion, 
	//distance to Panoramix, number of seconds it takes romans
	std::vector<std::pair<long, long>> movements(n);
	for (int i = 0; i < n; i++){
		long d, t; std::cin >> d >> t;
		movements[i] = std::make_pair(d, t);
	}
	std::vector<long> potion_boost(m);
	int n_half = n/2;
	std::vector<std::vector<std::pair<long, long>>> movements_time_and_num_movements(n_half + 1);
	std::vector<std::vector<std::pair<long, long>>> movements_time_and_num_movements_2(n-n_half + 1);
	for (int i = 0; i < m; i++){
		std::cin >> potion_boost[i];
	}
	for (int s = 0; s < 1<<n_half; s++){
		long time = 0; long distance = 0; int num_moves = 0;
		for (int i = 0; i < n_half; i++){
			if (s & 1<<i){
				distance += movements[i].first;
				time += movements[i].second;
				num_moves++;
			}
		}
		movements_time_and_num_movements[num_moves].push_back(std::make_pair(time, distance));
	}
	for (int s = 0; s < 1<<(n-n_half); s++){
		int s_shifted = s<<n_half;
		long time = 0; long distance = 0; int num_moves = 0;
		for (int i = n_half; i < n; i++){
			if (s_shifted & 1<<i){
				distance += movements[i].first;
				time += movements[i].second;
				num_moves++;
			}
		}
		movements_time_and_num_movements_2[num_moves].push_back(std::make_pair(time, distance));
	}
	sort_vec(movements_time_and_num_movements);
	sort_vec(movements_time_and_num_movements_2);

	long needed_boost = std::numeric_limits<long>::max();
	for (int i = 0; i < (n_half + 1); i++){
		for (std::pair<long, long> move : movements_time_and_num_movements[i]){
			//time = move.first, dist = move.second
			for (int j = 0; j < (n - n_half + 1); j++){
				auto it = std::lower_bound(movements_time_and_num_movements_2[j].begin(), movements_time_and_num_movements_2[j].end(), 
											T-move.first, [](std::pair<long, long> a, long b) { return a.first < b; });
				if ((movements_time_and_num_movements_2[j].begin() != it) and (i + j > 0)){
					it--; // we want the one before
					long distance_full = move.second + (*it).second;
					if (D > distance_full){
						needed_boost = std::min(needed_boost, (long) std::ceil((D - distance_full)/(float)(i + j)));
					} else {
						needed_boost = 0;
					}
				}
			}
		}
	}
	if (needed_boost == 0){
		std::cout << 0 << "\n";
		return;
	}
	for (int i = 0; i < m; i++){
		if (potion_boost[i]>=needed_boost){
			std::cout << i+1 << "\n";
			return;
		}
	}
	std::cout << "Panoramix captured" << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
