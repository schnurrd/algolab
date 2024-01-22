#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
  // TODO:
  int n; std::cin >> n; // number of parasols
  std::vector<int> parasols_pos(n);
  for (int i = 0; i < n; i++){
    std::cin >> parasols_pos[i];
  }
  std::sort(parasols_pos.begin(), parasols_pos.end());

  int pointer_i = 0;
  int pointer_j = 0;
  int max_number = 0;
  int min_distance = 200;
  std::vector<int> solutions;
  while (pointer_i != n-1){
    int pointer_diff = pointer_j - pointer_i + 1;
    int value_diff = std::abs(parasols_pos[pointer_j] - parasols_pos[pointer_i]);
    bool two_sol = value_diff%2 == 1;
    int dist = (int) std::ceil(value_diff/2.0);
    int pos = parasols_pos[pointer_i] + dist;
    int add_pos = false;
    if (dist <= 100){
      if (pointer_diff > max_number){
        max_number = pointer_diff;
        min_distance = dist;
        solutions.clear();
        add_pos = true;
      } else if (pointer_diff == max_number){
        if(min_distance == dist){
          add_pos = true;
        } else if (min_distance > dist){
          solutions.clear();
          add_pos = true;
          min_distance = dist;
        }
      }
    }
    if (add_pos){
      if (two_sol){
        solutions.push_back(pos-1);
      }
      solutions.push_back(pos);
    }
    if (parasols_pos[pointer_j] - parasols_pos[pointer_i] <= 200 and 
        pointer_j != n - 1){
      pointer_j++;
    } else {
      pointer_i++;
    }
  }
  std::cout << max_number << " " << min_distance << "\n";
  for(auto sol : solutions){
    std::cout << sol << " ";
  }
  std::cout << "\n";
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}