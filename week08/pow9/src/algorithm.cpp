#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store: Index + num_bones in raduis
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::tuple<int, int,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2, int> IPoint;
typedef K::Point_2 P;

int n, m, k;

long get_k_bones(Delaunay& t, std::vector<P>& bones, long s){
	EdgeV edges;
	edges.reserve(3*n+m); // there can be no more in a planar graph
	for (int i = 0; i < m; i++){
		auto nearest_tree = t.nearest_vertex(bones[i]);
		edges.emplace_back(nearest_tree->info(), n+i, 4 * CGAL::squared_distance(nearest_tree->point(), bones[i]));
	}
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		int i1 = e->first->vertex((e->second+1)%3)->info();
		int i2 = e->first->vertex((e->second+2)%3)->info();
		// ensure smaller index comes first
		if (i1 > i2) std::swap(i1, i2);
		edges.emplace_back(i1, i2, t.segment(e).squared_length());
	}
	std::sort(edges.begin(), edges.end(),
			[](const Edge& e1, const Edge& e2) -> bool {
			return std::get<2>(e1) < std::get<2>(e2);
				});

	// Compute EMST using Kruskal's algorithm. This step takes O(n alpha(n)) time
	// in theory; for all practical purposes alpha(n) is constant, so linear time.

	// setup and initialize union-find data structure
	boost::disjoint_sets_with_storage<> uf(n+m);
	int n_components = n+m;
	int max_num_bones = 0;
	std::vector<int> bones_per_set(n+m, 0);
	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		if (s != -1 and std::get<2>(*e) > s) return max_num_bones;
		// determine components of endpoints
		int c1 = uf.find_set(std::get<0>(*e));
		int c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			// this edge connects two different components => part of the emst
			if (std::get<0>(*e) >= n){
				bones_per_set[c2]++;
				max_num_bones = std::max(max_num_bones, bones_per_set[c2]);
				uf.link(c1, c2);
			} else if (std::get<1>(*e) >= n){
				bones_per_set[c1]++;
				max_num_bones = std::max(max_num_bones, bones_per_set[c1]);
				uf.link(c2, c1);
			} else{
				int bones_per_set_combined = bones_per_set[c1] + bones_per_set[c2];
				bones_per_set[c1] = bones_per_set_combined;
				bones_per_set[c2] = bones_per_set_combined;
				max_num_bones = std::max(max_num_bones, bones_per_set_combined);
				uf.link(c1, c2);
			}
			if (s == -1 and max_num_bones >= k){
				return std::get<2>(*e);
			}
			if (--n_components == 1) break;
		}
	}
	return max_num_bones;
}

void testcase() {
	// TODO:
	long s; std::cin >> n >> m >> s >> k;
	// number of oak trees, number of bones, radius of oak tree, minimum number of bones to inspect
	std::vector<IPoint> trees(n);
	for (int i = 0; i < n; i++){
		int x, y; std::cin >> x >> y; // position, where tree is planted
		trees[i] = std::make_pair(P(x, y), i);
	}
	Delaunay t;
	t.insert(trees.begin(), trees.end());

	std::vector<P> bones(m);
	for(int i = 0; i < m; i++){
		int x, y; std::cin >> x >> y; // position, where bone is buried
		bones[i] = P(x, y);
		// multiple bones can be buried at the same location
	}
	std::cout << get_k_bones(t, bones, s) << " " << get_k_bones(t, bones, -1) << "\n";

	// get b by binary search on s;
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
