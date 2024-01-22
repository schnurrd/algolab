///1
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double floor_to_double(const K::FT& x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n) {
  // TODO:
  long x, y, a, b; std::cin >> x >> y >> a >> b;

  bool hit = false;
  K::Point_2 nearest_intersection;
  K::Point_2 start_point(x,y);
  K::Point_2 middle_point(a,b);
  K::Segment_2 shortened_ray;

  std::vector<K::Segment_2> obstacles;
  obstacles.reserve(n);
  // (x, y) starting pos of ray, (a, b) another point on ray
  for (int i = 0; i < n; i++){
    long r, s, t, u;
    std::cin >> r >> s >> t >> u; //segment starting at (r,s) to (t,u)
    obstacles.push_back(K::Segment_2(K::Point_2(r, s), K::Point_2(t, u)));
  }
  std::random_shuffle(obstacles.begin(), obstacles.end());

  for (int i = 0; i < n; i++){
	K::Point_2 obs_point_1 = obstacles[i].source();
	K::Point_2 obs_point_2 = obstacles[i].target();
    if (CGAL::orientation(start_point, middle_point, obs_point_1) != CGAL::orientation(start_point, middle_point, obs_point_2) or 
		CGAL::orientation(start_point, middle_point, obs_point_2) == CGAL::COLLINEAR){
      if (!hit){
		K::Ray_2 shot_ray(start_point, middle_point);
        if(CGAL::do_intersect(shot_ray, obstacles[i])){
          auto o = CGAL::intersection(shot_ray, obstacles[i]);
          if (const K::Point_2* op = boost::get<K::Point_2>(&*o)){
            nearest_intersection = *op;
          }
          else if (const K::Segment_2* os = boost::get<K::Segment_2>(&*o))
            if(CGAL::squared_distance(start_point, os->source()) > CGAL::squared_distance(start_point, os->target())){
              nearest_intersection = os->target();
            } else {
              nearest_intersection = os->source();
            }
          hit = true;
		  shortened_ray = K::Segment_2(start_point, nearest_intersection);
        }
      } else {
        if(CGAL::do_intersect(shortened_ray, obstacles[i])){
          auto o = CGAL::intersection(shortened_ray, obstacles[i]);
          if (const K::Point_2* op = boost::get<K::Point_2>(&*o)){
            nearest_intersection = *op;
          }
          else if (const K::Segment_2* os = boost::get<K::Segment_2>(&*o))
            if(CGAL::squared_distance(start_point, os->source()) > CGAL::squared_distance(start_point, os->target())){
              nearest_intersection = os->target();
            } else {
              nearest_intersection = os->source();
            }
		  shortened_ray = K::Segment_2(start_point, nearest_intersection);
        }
      }
    }
    // (r, s) and (t, u) are endpoints of segment
  }
  if(hit){
    std::cout << std::setprecision(0);
    std::cout << std::fixed << floor_to_double(nearest_intersection.x()) << " " << floor_to_double(nearest_intersection.y()) <<"\n";
  } else {
    std::cout << "no" << "\n";
  }
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
