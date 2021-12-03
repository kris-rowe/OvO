#include <iostream>
#include <cstdlib>
#include <algorithm>
#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_teams() {return 1;}
void omp_set_num_teams(int _) {}
#endif
bool almost_equal(float x, float gold, float rel_tol=1e-09, float abs_tol=0.0) {
  return std::abs(x-gold) <= std::max(rel_tol * std::max(std::abs(x), std::abs(gold)), abs_tol);
}
void test_target_teams() {
  const float expected_value { 1 };
  omp_set_num_teams(32768);
  float counter_teams{};
  #pragma omp target teams reduction(+: counter_teams)
  {
    counter_teams = counter_teams + float { float{ 1. } / omp_get_num_teams() };
  }
  if (!almost_equal(counter_teams, expected_value, 0.01)) {
    std::cerr << "Expected: " << expected_value << " Got: " << counter_teams << std::endl;
    std::exit(112);
  }
}
int main()
{
    test_target_teams();
}
