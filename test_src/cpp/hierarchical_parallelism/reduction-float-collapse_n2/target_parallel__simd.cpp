#include <iostream>
#include <cstdlib>
#include <cmath>
#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_threads() {return 1;}
#endif
bool almost_equal(float x, float gold, float tol) {
  return gold * (1-tol) <= x && x <= gold * (1 + tol);
}
void test_target_parallel__simd() {
  const int N0 { 512 };
  const int N1 { 512 };
  const float expected_value { N0*N1 };
  float counter_parallel{};
  #pragma omp target parallel map(tofrom: counter_parallel) reduction(+: counter_parallel)
  {
    #pragma omp simd reduction(+: counter_parallel) collapse(2)
    for (int i0 = 0 ; i0 < N0 ; i0++ )
    for (int i1 = 0 ; i1 < N1 ; i1++ )
    {
      counter_parallel = counter_parallel + float { float { 1. } / omp_get_num_threads() };
    }
  }
  if (!almost_equal(counter_parallel, expected_value, 0.1)) {
    std::cerr << "Expected: " << expected_value << " Got: " << counter_parallel << std::endl;
    std::exit(112);
  }
}
int main()
{
    test_target_parallel__simd();
}
