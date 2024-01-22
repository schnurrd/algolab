#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <boost/container_hash/hash.hpp>

int n, m, P, H, W;
long a, b;
std::vector<std::pair<int, int>> power_and_happiness_of_potions_A;
std::vector<int> wit_of_potions_B;
std::vector<int> num_portions_b_needed_for_i_potions_of_a;
std::vector<std::unordered_map<std::tuple<int, int, int>, int, boost::hash<std::tuple<int, int, int>>>> memo;

int solve(int pointer, int needed_p, int needed_h, int number_potions_a){
	if(memo[pointer].find({needed_p, needed_h, number_potions_a}) != memo[pointer].end()){
		return memo[pointer][{needed_p, needed_h, number_potions_a}];
	}
	if (needed_p + num_portions_b_needed_for_i_potions_of_a[number_potions_a] * b  <= 0 
			and needed_h <= 0 
			and  num_portions_b_needed_for_i_potions_of_a[number_potions_a] != std::numeric_limits<int>::max()){
		return number_potions_a + num_portions_b_needed_for_i_potions_of_a[number_potions_a];
	}
	int result = std::numeric_limits<int>::max();
	if (pointer == n){
		return result;
	}
	//take current item
	std::pair<int, int> current_potion = power_and_happiness_of_potions_A[pointer];
	result = std::min(result, solve(pointer + 1, needed_p - current_potion.first, needed_h - current_potion.second, number_potions_a + 1));
	result = std::min(result, solve(pointer + 1, needed_p, needed_h, number_potions_a));
	memo[pointer][{needed_p, needed_h, number_potions_a}] = result;
	return result;
}

void testcase() {
	// TODO:
	std::cin >> n >> m; // potions of type A and B
	std::cin >> a >> b; 
	// a wit Harray loses with each potion of type A, 
	// b magical power lost with each potion of type B
	std::cin >> P >> H >> W; //needed powers
	power_and_happiness_of_potions_A = std::vector<std::pair<int, int>> (n);
	for (int i = 0; i < n; i++){
		int p_i, h_i; std::cin >> p_i >> h_i;
		power_and_happiness_of_potions_A[i] = std::make_pair(p_i, h_i);
		// magical power and happiness of poitnion i of type A
	}
	wit_of_potions_B = std::vector<int> (m);
	for (int i = 0; i < m; i++){
		std::cin >> wit_of_potions_B[i];
	}
	std::sort(wit_of_potions_B.begin(), wit_of_potions_B.end(), std::greater<int>());
	long sum = 0;
	num_portions_b_needed_for_i_potions_of_a = std::vector<int> (n+1, std::numeric_limits<int>::max());
	memo = std::vector<std::unordered_map<std::tuple<int, int, int>, int, boost::hash<std::tuple<int, int, int>>>> (n+1, 
			std::unordered_map<std::tuple<int, int, int>, int, boost::hash<std::tuple<int, int, int>>>());
	for (int i = 0; i < m; i++){
		sum += wit_of_potions_B[i];
		for (int j = 0; j <= n; j++){
			if (W+j*a <= sum){
				num_portions_b_needed_for_i_potions_of_a[j] = 
					std::min(num_portions_b_needed_for_i_potions_of_a[j], i+1);
			} else {
				break;
			}
		}
	}
	int result = solve(0, P, H, 0);
	if (result == std::numeric_limits<int>::max()){
		std::cout << -1 << "\n";
		return;
	}
	std::cout << result << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
