#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

void testcase() {
  // TODO:
  long n, m, x, k; std::cin >> n >> m >> x >> k; // number of holes, number of canals, claimed score, claimed moves
  // hole 0 corresponds to Angvariationu-toke
  std::vector<std::vector<std::pair<int, int>>> movements(n);
  std::vector<long> scores(n, 0), scores_old(n, 0);
  for (int i = 0; i < m; i++){
    int u, v, p; std::cin >> u >> v >> p;
    movements[u].push_back(std::make_pair(v, p));
  }
  for (int i = 0; i < k; i++){
    for (int j = 0; j < n; j++){
      long max_val = -1;
      for (auto it = movements[j].cbegin(); it != movements[j].cend(); ++it){
        max_val = std::max(max_val, scores_old[(*it).first] + (*it).second);
      }
      scores[j] = (max_val == -1) ?  scores[0] : max_val;
    }
    scores_old = scores;
    if (scores[0] >= x){
      std::cout << i+1 << "\n";
      return;
    }
  }
  std::cout << "Impossible" << "\n";
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
