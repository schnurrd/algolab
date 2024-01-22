#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <functional>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
// long min d possible to escape
typedef CGAL::Triangulation_face_base_with_info_2<long, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;

void testcase(int n) {
	// TODO:
	std::vector<P> infected_people(n);
	for (int i = 0; i < n; i++){ //infected people
		int x_i, y_i; std::cin >> x_i >> y_i;
		infected_people[i] = P(x_i, y_i);
	}
	Triangulation t;
 	t.insert(infected_people.begin(), infected_people.end());
	auto cmp = [](std::pair<long, Triangulation::Face_handle> left, std::pair<long, Triangulation::Face_handle> right) 
						{return left.first < right.first;};
	std::priority_queue<std::pair<long, Triangulation::Face_handle>, 
						std::vector<std::pair<long, Triangulation::Face_handle>>,
						decltype(cmp)> pq(cmp); //update largest dist first;
	for (Triangulation::All_faces_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
		f->info() = 0;
	}
	for (Triangulation::All_faces_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
		if (!t.is_infinite(f)){
			continue;
		}
		f->info() = std::numeric_limits<long>::max();
		for (int i = 0; i < 3; i++){
			if (!t.is_infinite(f->neighbor(i))){
				long dist = CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point());
				if (f->neighbor(i)->info() < dist){
					f->neighbor(i)->info() = dist;
					pq.push(std::make_pair(dist, (f->neighbor(i))));
				}
			}
		}
	}
	while (!pq.empty()){
		auto top = pq.top(); pq.pop();
		long dist_max = top.first;
		Triangulation::Face_handle f = top.second;
		for (int i = 0; i < 3; i++){
			if (!t.is_infinite(f->neighbor(i))){
				long dist = std::min(dist_max, (long) CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point()));
				if (f->neighbor(i)->info() < dist){
					f->neighbor(i)->info() = dist;
					pq.push(std::make_pair(dist, (f->neighbor(i))));
				}
			}
		}
	}
	int m; std::cin >> m; // number people that want to escape
	for (int i = 0; i < m; i++){
		int x_i, y_i; long d; std::cin >> x_i >> y_i >> d;
		P current_point = P(x_i, y_i);
		long dist_start = CGAL::squared_distance(t.nearest_vertex(current_point)->point(), current_point);
		long current_face_dist = t.locate(current_point)->info();
		if(dist_start >= d and current_face_dist >= 4*d){
			std::cout << "y";
		} else {
			std::cout << "n";
		}
	}
	std::cout << "\n";
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	int t;
	std::cin >> t;
	while(t != 0){
		testcase(t);
		std::cin >> t;
	}
}
