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

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase(int p) {
  // TODO:
  int a, b; std::cin >> a >> b;
  const int X = 0; 
    const int Y = 1;
  const int Z = 2;
  if (p == 1){
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); 
    lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);  
    lp.set_a(X, 2,  -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);  
    lp.set_c(Y, -b); lp.set_c(X, a); 
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    if (s.is_unbounded()){
      std::cout << "unbounded" << "\n";
    } else if (s.is_infeasible()){
      std::cout << "no" << "\n";
    } else {
      std::cout << (long) std::floor(CGAL::to_double(-s.objective_value())) << "\n";
    }
  } else {
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); 
    lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, a*b);  
    lp.set_a(X, 2,  -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);  
    lp.set_c(Y, -b); lp.set_c(X, -a); lp.set_c(Z, -1); 
    Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
    if (s.is_unbounded()){
      std::cout << "unbounded" << "\n";
    } else if (s.is_infeasible()){
      std::cout << "no" << "\n";
    } else {
      std::cout << (long) std::ceil(CGAL::to_double(s.objective_value())) << "\n";
    }
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