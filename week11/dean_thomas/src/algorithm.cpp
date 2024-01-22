#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef std::pair<bool, K::FT>                              Info;
typedef CGAL::Triangulation_face_base_with_info_2<Info,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Finite_faces_iterator Finite_Face_iterator;

void testcase() {
	int n, m; std::cin >> n >> m; // number of Dementors, number of fugitives
	std::vector<K::Point_2> pts;
	pts.reserve(n);
	long d; std::cin >> d; // distance of Dementor of last observed pos
	for (int i = 0; i < n; i++){
		long x, y; std::cin >> x >> y; // last observed position of Dementor
		pts.push_back(K::Point_2(x, y));
	}
	// construct triangulation
  	Triangulation t;
  	t.insert(pts.begin(), pts.end());

	std::priority_queue<std::pair<K::FT, Triangulation::Face_handle>> q;
	for (auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++) {
		f->info() = {false, CGAL::squared_distance(f->vertex(0)->point(), t.dual(f))};

		for (int i = 0; i < 3; i++) {
			if (!t.is_infinite(f->neighbor(i))) // only update faces adjecant to infinite faces
				continue;
			// t.segment(f, i) is the i-th edge of the segment
			f->info().second = std::max(f->info().second, t.segment(f, i).squared_length()); // 4*dist
		}
		q.push({f->info().second, f});
	}

	while (!q.empty()) {
		auto curr = q.top(); q.pop();
		auto face = curr.second;

		if (!face->info().first) {
			for (int i = 0; i < 3; i++) {
				auto neighbor = face->neighbor(i);
				if (t.is_infinite(neighbor))
					continue;

				if (!neighbor->info().first) {
					auto capacity = std::min(t.segment(face, i).squared_length(), face->info().second);
					neighbor->info().second = std::max(neighbor->info().second, capacity);
					q.push(std::make_pair(neighbor->info().second, neighbor));
				}
			}

			face->info().first = true;
		}
	}

	for (int i = 0; i < m; i++){
		long x, y, s; std::cin >> x >> y >> s; // starting postion of person p_i and safe disance s_i with patronous
		K::FT r = K::FT(s + d) * K::FT(s + d);

		K::Point_2 p_i(x, y);
		if (CGAL::squared_distance(p_i, t.nearest_vertex(p_i)->point()) < r) {
			std::cout << "n";
			continue;
		}

		auto f = t.locate(p_i);
		if (t.is_infinite(f))
			std::cout << "y";
		else if (f->info().second < 4 * r)
			std::cout << "n";
		else
			std::cout << "y";
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