#include <iostream>
#include <cstdlib>
#include <cmath>
bool almost_equal(float x, float gold, float tol) {
  return gold * (1-tol) <= x && x <= gold * (1 + tol);
}
void test_target_teams_distribute__parallel_for__simd() {
  const int N0 { 64 };
  const int N1 { 64 };
  const int N2 { 64 };
  const float expected_value { N0*N1*N2 };
  float counter_N0{};
  #pragma omp target teams distribute map(tofrom: counter_N0) reduction(+: counter_N0)
  for (int i0 = 0 ; i0 < N0 ; i0++ )
  {
    float counter_N1{};
    #pragma omp parallel for reduction(+: counter_N1)
    for (int i1 = 0 ; i1 < N1 ; i1++ )
    {
      float counter_N2{};
      #pragma omp simd reduction(+: counter_N2)
      for (int i2 = 0 ; i2 < N2 ; i2++ )
      {
        counter_N2 = counter_N2 +  1. ;
      }
      counter_N1 = counter_N1 +  counter_N2 ;
    }
    counter_N0 = counter_N0 +  counter_N1 ;
  }
  if (!almost_equal(counter_N0, expected_value, 0.1)) {
    std::cerr << "Expected: " << expected_value << " Got: " << counter_N0 << std::endl;
    std::exit(112);
  }
}
int main()
{
    test_target_teams_distribute__parallel_for__simd();
}
