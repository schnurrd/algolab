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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;
typedef K::Point_2 P;

Index n, m; 

void find_min_power_consumtion(std::vector<std::pair<P, P>>& missions, Delaunay& t, EdgeV& edges){
	double min_power = 0;
	EdgeV::const_iterator edge_pointer = edges.begin();
	boost::disjoint_sets_with_storage<> uf(n);
	for(auto mission : missions){
		min_power = std::max(min_power, CGAL::squared_distance(t.nearest_vertex(mission.first)->point(), mission.first) * 4);
		min_power = std::max(min_power, CGAL::squared_distance(t.nearest_vertex(mission.second)->point(), mission.second) * 4);

		auto nearest_vertex_start = t.nearest_vertex(mission.first);
		auto nearest_vertex_end = t.nearest_vertex(mission.second);

		while(uf.find_set(nearest_vertex_start->info()) != uf.find_set(nearest_vertex_end->info()) and edge_pointer != edges.end()){
			min_power = std::max(min_power, std::get<2>(*edge_pointer));
			Index c1 = uf.find_set(std::get<0>(*edge_pointer));
			Index c2 = uf.find_set(std::get<1>(*edge_pointer));
			if (c1 != c2) {
				uf.link(c1, c2);
			}
			edge_pointer++;
		}
	}
	std::cout << (long) std::ceil(min_power) << "\n";
}

void testcase() {
	// TODO:
	long p; std::cin >> n >> m >> p; 
	//number of jammers, missions, power consumption
	std::vector<IPoint> jammers(n);
	for (Index i = 0; i < n; i++){
		int x_i, y_i; std::cin >> x_i >> y_i;
		jammers[i] = std::make_pair(P(x_i, y_i), i);
	}
	std::vector<std::pair<P, P>> missions(m);
	for (Index i = 0; i < m; i++){
		int x_i_1, y_i_1, x_i_2, y_i_2; 
		std::cin >> x_i_1 >> y_i_1 >> x_i_2 >> y_i_2;
		missions[i] = std::make_pair(P(x_i_1, y_i_1), P(x_i_2, y_i_2));
	}

	Delaunay t;
	t.insert(jammers.begin(), jammers.end());

	// extract edges and sort by (squared) length
	// This step takes O(n log n) time (for the sorting).
	EdgeV edges;
	edges.reserve(3*n); // there can be no more in a planar graph
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		// ensure smaller index comes first
		if (i1 > i2) std::swap(i1, i2);
		edges.emplace_back(i1, i2, t.segment(e).squared_length());
	}
	std::sort(edges.begin(), edges.end(),
			[](const Edge& e1, const Edge& e2) -> bool {
			return std::get<2>(e1) < std::get<2>(e2);
				});

	boost::disjoint_sets_with_storage<> uf(n);
	Index n_components = n;
	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		if (std::get<2>(*e) > p){
			break;
		}
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			uf.link(c1, c2);
			if (--n_components == 1) break;
		}
	}
	std::vector<std::pair<P, P>> successfull_missions;
	for (Index i = 0; i < m; i++){
		P mission_start = missions[i].first;
		P mission_end = missions[i].second;
		if (CGAL::squared_distance(t.nearest_vertex(mission_start)->point(), mission_start) * 4 <= p and
			CGAL::squared_distance(t.nearest_vertex(mission_end)->point(), mission_end) * 4 <= p and
			uf.find_set(t.nearest_vertex(mission_start)->info()) == uf.find_set(t.nearest_vertex(mission_end)->info())){
			successfull_missions.push_back(missions[i]);
			std::cout << "y";
		}else{
			std::cout << "n";
		}
	}
	std::cout << "\n";
	find_min_power_consumtion(missions, t, edges);
	find_min_power_consumtion(successfull_missions, t, edges);

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}
