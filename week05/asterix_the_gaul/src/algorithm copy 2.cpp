#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

int n, m, n_half; long D, T; 
std::vector<std::pair<long, long>> movements;
std::vector<std::vector<std::pair<long, long>>> movements_time_and_num_movements;
std::vector<std::vector<std::pair<long, long>>> movements_time_and_num_movements_2;

void solve_moves1(int pointer_i, int num_moves, long d, long t){
  // iterate over all 2^n possibilities
  if (t >= T) return;
  movements_time_and_num_movements[num_moves].emplace_back(std::make_pair(t, d));
  if (pointer_i == n_half) return;

  solve_moves1(pointer_i + 1, num_moves, d, t);
  solve_moves1(pointer_i + 1, num_moves + 1, d + movements[pointer_i].first, t + movements[pointer_i].second);
}

void solve_moves2(int pointer_i, int num_moves, long d, long t){
  // iterate over all 2^n possibilities
  if (t >= T) return;
  movements_time_and_num_movements_2[num_moves].emplace_back(std::make_pair(t, d));
  if (pointer_i == n-n_half) return;

  solve_moves2(pointer_i + 1, num_moves, d, t);
  solve_moves2(pointer_i + 1, num_moves + 1, d + movements[pointer_i + n_half].first, t + movements[pointer_i + n_half].second);
}

void sort_vectors(std::vector<std::vector<std::pair<long, long>>> &vec_to_sort){
  for (size_t i = 0; i < vec_to_sort.size(); i++){
    std::sort(vec_to_sort[i].begin(), vec_to_sort[i].end(), 
			[](std::pair<long, long> a, std::pair<long, long> b) { 
				return a.first < b.first or 
				(a.first == b.first and a.second > b.second);});

    long distance = 0;
    long shift = 0;
    for (long j = 0; j < vec_to_sort[i].size(); j++){
      if (distance < vec_to_sort[i][j].second){
        vec_to_sort[i][shift] = vec_to_sort[i][j];
        shift++;
        distance = vec_to_sort[i][j].second;
      }
    }
    vec_to_sort[i].resize(shift);
  }
}

void testcase() {
	// TODO:
	std::cin >> n >> m >> D >> T;
	// number of movements, amount of magic potion, 
	//distance to Panoramix, number of seconds it takes romans
	movements = std::vector<std::pair<long, long>>(n);
	for (int i = 0; i < n; i++){
		long d, t; std::cin >> d >> t;
		movements[i] = std::make_pair(d, t);
	}
	std::vector<long> potion_boost(m+1, 0);
	n_half = n/2;
	movements_time_and_num_movements = std::vector<std::vector<std::pair<long, long>>>(n_half + 1);
	movements_time_and_num_movements_2 = std::vector<std::vector<std::pair<long, long>>>(n-n_half + 1);
	for (int i = 1; i <= m; i++){
		std::cin >> potion_boost[i];
	}
	
	solve_moves1(0, 0, 0, 0);
	solve_moves2(0, 0, 0, 0);
	
	sort_vectors(movements_time_and_num_movements);
	sort_vectors(movements_time_and_num_movements_2);

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
	for (int i = 0; i < m+1; i++){
		if (potion_boost[i]>=needed_boost){
			std::cout << i << "\n";
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
