#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <queue>
#include <vector>

// ETH AlgoLab example code: Compute a Euclidean minimum spanning tree (EMST)
// for n points p_0,...,p_{n-1} in O(n log n) time. Output the edges as ordered
// pairs of vertex indices (smaller first) together with the squared length; for
// instance, an edge between p_4=(0,0) and p_2=(1,2) is printed as "2 4 5".

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

struct Vertex_info {
  int color;
  int component;
};
// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Vertex_info, K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

bool is_bipartite(CGAL::Delaunay_triangulation_2<K,Tds> & t, long squared_radius) {
	std::queue<Delaunay::Vertex_handle> q;
	int component_id = 0;
	for(auto f = t.finite_vertices_begin(); f != t.finite_vertices_end(); ++f) {
    	if (f->info().component) {
      		continue;
    	}
		component_id++;
		q.push(f);
		int current_color = 1;
		while(!q.empty()){
			const int current_size = q.size();
			for (int i = 0; i < current_size; i++){
				const auto f_handle = q.front(); q.pop();

				if ((f_handle->info().color | current_color) == 3){
					return false;
				}
				if (f_handle->info().component) {
         			continue;
        		}
				f_handle->info() = {current_color, component_id};
				Delaunay::Vertex_circulator c = t.incident_vertices(f_handle);
				do{
					if(!t.is_infinite(c) && CGAL::squared_distance(f_handle->point(), c->point()) <= squared_radius){
						q.push(c);
					}
				} while (++c != t.incident_vertices(f_handle));
			}
			current_color = ~current_color & 3;
		}
  	}

	std::vector<Delaunay::Vertex_handle> neighbor_set;
	for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); ++v){
		neighbor_set.clear();
		Delaunay::Vertex_circulator c = t.incident_vertices(v);
		do{
			if(!t.is_infinite(c)){
				neighbor_set.push_back(c);
			}
		} while (++c != t.incident_vertices(v));

		for(int i = 0; i < (int) neighbor_set.size(); i++){
			for (int j = i+1; j < (int) neighbor_set.size(); j++){
				const auto v1 = neighbor_set[i];
				const auto v2 = neighbor_set[j];
				if (CGAL::squared_distance(v1->point(), v2->point()) <= squared_radius && v1->info().color == v2->info().color) {
					return false;
				}
			}
		}
	}
	return true;
}


void testcase() {
	// TODO:
	int n, m, r; std::cin >> n >> m >> r; //number of stations, number of clues, operation range of clients
	// read points: first, we read all points and store them into a vector,
	// together with their indices
	typedef std::pair<K::Point_2, Vertex_info> IPoint;
	std::vector<IPoint> points;
	points.reserve(n);
	for (int i = 0; i < n; ++i) {
		int x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), Vertex_info{0, 0});
	}
	// then we build the Delaunay triangulation in one shot, so as to leave the
	// choice of an efficient insertion order to the triangulation structure. By
	// giving the points paired with the indices, these indices are used to
	// initialize the vertex info accordingly.
	// This step takes O(n log n) time (for constructing the triangulation).
	Delaunay t;
	t.insert(points.begin(), points.end());
	long squared_radius = (long) r * r;
	bool bipartite = is_bipartite(t, squared_radius);

	for (int i = 0; i < m; i++){
		int a_i_x, a_i_y, b_i_x, b_i_y; std::cin >> a_i_x >>  a_i_y >> b_i_x >> b_i_y; // describes teh position of Holmes and Watson at the moment when this clue is optained
		K::Point_2 holmes = K::Point_2(a_i_x, a_i_y);
		K::Point_2 watson = K::Point_2(b_i_x, b_i_y);
		if (!bipartite){
			std::cout << "n"; continue;
		}
		if (CGAL::squared_distance(holmes, watson) <= squared_radius){
			std::cout << "y"; continue;
		}
		auto v1 = t.nearest_vertex(holmes);
		auto v2 = t.nearest_vertex(watson);
		bool reachable = std::max(CGAL::squared_distance(holmes, v1->point()), CGAL::squared_distance(watson, v2->point())) <= squared_radius;
		bool same_component = v1->info().component == v2->info().component;
		if(reachable && same_component){
			std::cout << "y"; continue;
		} 
		std::cout << "n";
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
