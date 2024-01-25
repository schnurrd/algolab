#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef K::Point_2 P;

std::vector<int> color_map;
std::vector<int> component_map;

void testcase() {
	// TODO:
	int n, m; long r; std::cin >> n >> m >> r;
	std::vector<K::Point_2> stations;
	color_map = std::vector<int>(n, -1);
	component_map = std::vector<int>(n, -1);
  	stations.reserve(n);
	for (int i = 0; i < n; i++){
		// positions of the stations
		int x_i, y_i; std::cin >> x_i >> y_i;
		stations.push_back(P(x_i, y_i));
	}
	Triangulation t;
  	t.insert(stations.begin(), stations.end());
	int vertex_id = 0;
	int component_id = 0;
	for (Triangulation::Vertex_iterator v = t.vertices_begin(); v != t.vertices_end(); ++v){
		v->info() = vertex_id;
		vertex_id++;
	}
	bool intersection = false;
	for (Triangulation::Vertex_iterator v = t.vertices_begin(); v != t.vertices_end(); ++v){
		int vertex_id = v->info();
		if (color_map[vertex_id] == -1){
			color_map[vertex_id] = 0;
			component_map[vertex_id] = component_id;
			std::queue<std::pair<Triangulation::Vertex_handle, int>> q;
			q.push({v, 0});
			while(!q.empty()){
				std::pair<Triangulation::Vertex_handle, int> current_obj = q.front(); 
				Triangulation::Vertex_handle current_v = current_obj.first;
				int current_info = current_v->info();
				int color = current_obj.second;
				q.pop();
				Triangulation::Vertex_circulator c = t.incident_vertices(current_v);
				do {
					int neighbor_info = c->info();
           			double dist = CGAL::squared_distance(current_v->point(), c->point());
					if (dist > r*r or dist == 0 or neighbor_info == current_info) {
						continue; //currently not relevant
					}
					if (color_map[c->info()] == -1){
						color_map[c->info()] = (1 - color);
						component_map[c->info()] = component_id;
						q.push({c, 1 - color});
						Triangulation::Vertex_circulator c2 = t.incident_vertices(c);
						do {
							double dist1 = CGAL::squared_distance(current_v->point(), c2->point());
							double dist2 = CGAL::squared_distance(c->point(), c2->point());
							if (dist1 <= r*r and dist2 <= r*r and dist1!=0 and dist2!=0) {
								intersection = true;
							}
						} while (++c2 != t.incident_vertices(c));
					} else if (color_map[c->info()] == (1 - color)){
						continue; //already correct
					} else{
						intersection = true;
						//break;
					}
				} while (++c != t.incident_vertices(current_v));
							
			}
			component_id++;
		}
		if (intersection){
			break;
		}
	}

	for (int i = 0; i < m; i++){
		//position a for holems and b of watson when clue is obtained
		int a_x, a_y, b_x, b_y; std::cin >> a_x >> a_y >> b_x >> b_y;
		if (intersection){
			std::cout << "n";
			continue;
		}
		P watson = P(a_x, a_y);	P holmes = P(b_x, b_y);
		auto watson_nearest_point = t.nearest_vertex(watson);
		auto holems_nearest_point = t.nearest_vertex(holmes);
		if (CGAL::squared_distance(watson, holmes) <= r*r){
			std::cout << "y";
			continue;
		}
		if (CGAL::squared_distance(watson, watson_nearest_point->point()) <= r*r and
			CGAL::squared_distance(holmes, holems_nearest_point->point()) <= r*r and 
			component_map[watson_nearest_point->info()] == component_map[holems_nearest_point->info()]){
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
	for (int i = 0; i < t; ++i)
		testcase();
}
