///2
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>

std::vector<int> temeratures;
std::vector<std::vector<int>> adj_list;
int n, m, k;
std::map<int, int> temeratures_map;
std::vector<int> node_ids;

std::vector<bool> possible_nodes;

void remove(int val){
	const auto it = temeratures_map.find(val);
	if (--(it->second) == 0){
		temeratures_map.erase(it);
	}
}

void dfs(int node,
		int index_id,
		bool depth){
  bool change_depth = false;
  int node_id_at_idx = node_ids[index_id];
  temeratures_map[temeratures[node]]++;
  if (depth){
	remove(temeratures[node_id_at_idx]);
  }
  int diff = temeratures_map.rbegin()->first - temeratures_map.begin()->first;
  if (index_id == m-1 and !depth){
	depth = true;
	change_depth = true;
  }
  node_ids[index_id] = node;
  index_id = (index_id + 1)%m;

  if (depth and diff <= k){
    possible_nodes[node_ids[index_id]] = true;
  }
  int size_list = adj_list[node].size();
  for (int i = 0; i < size_list; i++){
	dfs(adj_list[node][i], index_id, depth);
  }
  index_id = (index_id + (m-1))%m;
  node_ids[index_id] = node_id_at_idx;
  if(change_depth){
	depth = false;
  }
  if (depth){
	temeratures_map[temeratures[node_id_at_idx]]++;
  }
  remove(temeratures[node]);
}

void testcase() {
  // TODO:
  std::cin >> n >> m >> k; //number of critical points, optimal trip length for a single day, maximum risk willing to take
  temeratures = std::vector<int>(n);
  adj_list = std::vector<std::vector<int>>(n);
  possible_nodes = std::vector<bool>(n, false);
  node_ids = std::vector<int>(m, 0); 
  temeratures_map = std::map<int, int>();
  for (int i = 0; i < n; i++){
    std::cin >> temeratures[i]; //temperature ate the i-th critical point
  }
  for (int i = 0; i < n-1; i++){
    int u, v; std::cin >> u >> v; // route to fail from the critical point u to the critical point v
    adj_list[u].push_back(v);
  }
  dfs(0, 0, false); 
  bool possible = false;
  for (int i = 0; i < n; i++){
    if (possible_nodes[i]){
      possible = true;
      std::cout << i << " ";
    }
  }
  if (!possible){
    std::cout << "Abort mission";
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
