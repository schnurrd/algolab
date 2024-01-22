// ref https://github.com/simon-hrabec/Algolab-2020/blob/main/problems/Week%2011%20-%20Fighting%20Pits%20of%20Meereen/fighting_pits_of_meereen.cpp

#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <unordered_map>
#include <boost/container_hash/hash.hpp>

int n, k, m; 
std::vector<int> fighter_type;

int calculate_bonus(const int a, const int b, const int c) {
  return ((a && a!=b && a != c) + (b && b!= c) + 1) * 1000;
}

void try_insert(std::unordered_map<uint32_t, int>& next, const uint32_t key, const int round_score, const int current_best) {
    if (round_score < 0) {
        return;
    }

    auto it = next.find(key);
    if (it != std::end(next)) {
        it->second = std::max(it->second, current_best + round_score);
    } else {
        next.emplace(key, current_best + round_score);
    }
}


const int difference_offset = 12;

void testcase() {
	// TODO:
	std::cin >> n >> k >> m; 
	//number of fighters in queue, number of different types of fighters, number of fighters that audience can remember
	fighter_type = std::vector<int>(n);

  	const int mask = m == 3 ? 255 : 15;
	for (int i = 0; i < n; i++){
		int val; std::cin >> val;
		fighter_type[i] = val+1;
	}

	std::unordered_map<uint32_t, int> current, next;
	next.emplace(difference_offset<<16, 0);

	for(const auto fighter: fighter_type){
		std::swap(current, next);
		next.clear();
		
		for (const auto& kv : current){
			const uint32_t key = kv.first;  
    		const int value = kv.second;
			const int difference = (key>>16)-difference_offset;
			const int left_hist= key & 255;
			const int right_hist = (key>>8) & 255;

			const int left_new = ((left_hist << 4) + fighter) & mask;
			const int right_new = ((right_hist << 4) + fighter) & mask;

			const int left_score = calculate_bonus(left_hist & 15, (left_hist>>4) & 15, fighter) - (1<<std::abs(difference-1));
			const int right_score = calculate_bonus(right_hist & 15, (right_hist>>4) & 15, fighter) - (1<<std::abs(difference+1));

			const uint32_t left_key = ((difference+difference_offset-1) << 16) + (right_hist<<8) + left_new;
			const uint32_t right_key = ((difference+difference_offset+1) << 16) + (right_new<<8) + left_hist;

			try_insert(next, left_key, left_score, value);
			try_insert(next, right_key, right_score, value);
		}
	}
	int best = 0;
	for(const auto& kv : next) {
		const int value = kv.second;
		best = std::max(best, value);
	}

  	std::cout << best << "\n";
	
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
