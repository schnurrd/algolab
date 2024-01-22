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
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 P;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

long get_time_for_dist(double dist){
  dist = std::sqrt(dist)/2;
  dist -= 0.5;
  if (dist <= 0){
    return 0;
  } else {
    return (long) std::ceil(std::sqrt(dist));
  }
}

void testcase(int n) {
  // TODO:
  int l, b, r, t_1; std::cin >> l >> b >> r >> t_1;
  // dish consists of all points (x, y), for which x in [l, r], y in [b, t_1]
  std::vector<IPoint> centers(n);
  for (Index i = 0; i < n; i++){
    int x_i, y_i; std::cin >> x_i >> y_i;
    centers[i] = std::make_pair(P(x_i, y_i), i);
  }
  int boundarie = n;
  Delaunay t;
  t.insert(centers.begin(), centers.end());

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
  for (auto e = t.finite_vertices_begin(); e != t.finite_vertices_end(); ++e){
    double vertex_x = e->point().x();
    double vertex_y = e->point().y();
    double min_x_dist = std::min(vertex_x - l, r - vertex_x);
    double min_y_dist = std::min(vertex_y - b, t_1 - vertex_y);
    // add boundaries left, right
    edges.emplace_back(e->info(), n, std::pow(2*min_x_dist, 2));
    edges.emplace_back(e->info(), n, std::pow(2*min_y_dist, 2));
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
      return std::get<2>(e1) < std::get<2>(e2);
        });

  int number_hit = 0;
  double dist_when_first_hit = -1;
  double dist_when_mid_hit = -1;
  double dist_when_last_hit = -1;
  std::vector<bool> got_hit(n, false);
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    Index c1 = std::get<0>(*e);
    Index c2 = std::get<1>(*e);
    if (c1 != n and !got_hit[c1]) {
      number_hit++;
      got_hit[c1] = true;
    }
    if (c2 != n and !got_hit[c2]) {
      number_hit++;
      got_hit[c2] = true;
    }
    if (number_hit >= 1 and dist_when_first_hit == -1)
      dist_when_first_hit = std::get<2>(*e);
    
    if (number_hit > n/2 and dist_when_mid_hit == -1)
      dist_when_mid_hit = std::get<2>(*e);
    
    if (number_hit >= n and dist_when_last_hit == -1)
      dist_when_last_hit = std::get<2>(*e);
    
  }
  std::cout << get_time_for_dist(dist_when_first_hit) << " "
        << get_time_for_dist(dist_when_mid_hit) << " "
        << get_time_for_dist(dist_when_last_hit) << "\n";

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
