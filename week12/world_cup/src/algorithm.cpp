///4
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;


// choose input type (input coefficients must fit)
typedef float IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase() {
  // TODO:
  int n, m, c; std::cin >> n >> m >> c; // number of warehouses, number of stadiums, number of countour lines
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  std::vector<int> alcohol_content(n);
  std::vector<K::Point_2> warehouse_points(n);
  std::vector<K::Point_2> stadium_points(m);
  std::vector<std::vector<int>> revenue(n, std::vector<int>(m));
  int line_in_lp = 0;
  for (int i = 0; i < n; i++){
    int x, y, s; std::cin >> x >> y >> s >> alcohol_content[i]; //position (x,y) of warehouse, supply, alcohol content
    warehouse_points[i] = K::Point_2(x, y);
    for(int j = 0; j < m; j++){
      lp.set_a(i*m+j, line_in_lp, 1); //a_0_0, a_0_1, ... a_0_(m-1), a_1_0, ...
    }
    lp.set_b(line_in_lp, s);
    line_in_lp++;
  }
  for (int j = 0; j < m; j++){
    int x, y, d, u; std::cin >> x >> y >> d >>u; // position (x,y) of stadium, demand, upper limit on alc.
    stadium_points[j] = K::Point_2(x, y);
    // insert it in the positive and negative direction
    for(int i = 0; i < n; i++){
      lp.set_a(i*m+j, line_in_lp, 1); //a_0_0, a_0_1, ... a_0_(m-1), a_1_0, ...
      lp.set_a(i*m+j, line_in_lp+1, -1);
      lp.set_a(i*m+j, line_in_lp+2, alcohol_content[i]);
    }
    lp.set_b(line_in_lp, d);
    lp.set_b(line_in_lp+1, -d);
    lp.set_b(line_in_lp+2, u*100);
    line_in_lp+=3;
  }
  for (int i = 0; i < n; i++){
    for (int j = 0; j < m; j++){
      int revenue_i_j; std::cin >> revenue_i_j; // revenue
      revenue[i][j] = revenue_i_j*100;
    }
  }
  std::vector<K::Circle_2> active_countour;
  Triangulation t;
  t.insert(warehouse_points.begin(), warehouse_points.end());
  t.insert(stadium_points.begin(), stadium_points.end());
  for (int i = 0; i < c; i++){
    int x, y; long r; std::cin >> x >> y >> r; //(center of counture line ), radius
	if(CGAL::squared_distance(t.nearest_vertex(K::Point_2(x, y))->point(), K::Point_2(x, y)) <= r*r){
		active_countour.push_back(K::Circle_2(K::Point_2(x, y), r*r));
	}
  }
  for (auto countour: active_countour){
	std::vector<bool> warehouse_inside(n, false);
  	std::vector<bool> stadium_inside(m, false);
	for (int k = 0; k < n; k++){
	  if(countour.oriented_side(warehouse_points[k]) == 1){
		warehouse_inside[k] = true;
	  }
    }
    for (int k = 0; k < m; k++){
	  if(countour.oriented_side(stadium_points[k]) == 1){
		stadium_inside[k] = true;
	  }
    }
    for (int i = 0; i < n; i++){
      for(int j = 0; j < m; j++){
		revenue[i][j] -= (warehouse_inside[i] != stadium_inside[j]);
      }
    }
  }
  for (int i = 0; i < n; i++){
    for (int j = 0; j < m; j++){
      lp.set_c(i*m+j, -revenue[i][j]);
    }
  }
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()){
    std::cout << "RIOT!" << "\n";
  } else{
    int result = std::floor(-CGAL::to_double(s.objective_value())/100);
    std::cout << result << "\n";
  }
  return;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
