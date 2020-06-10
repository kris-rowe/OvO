#include <iostream>
#include <cstdlib>
#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_teams()   {return 1;}
int omp_get_num_threads() {return 1;}
#endif
bool almost_equal(float x, float gold, float tol) {
        return gold * (1-tol) <= x && x <= gold * (1 + tol);
}
void test_target_teams__parallel__simd(){
 const int N0 = 262144;
 float counter{};
#pragma omp target teams reduction(+: counter) map(tofrom: counter)
    {
const int num_teams = omp_get_num_teams();
#pragma omp parallel reduction(+: counter)
    {
const int num_threads = omp_get_num_threads();
#pragma omp simd reduction(+: counter)
      for (int i0 = 0 ; i0 < N0 ; i0++ )
      {
counter += float { 1.0f/(num_teams*num_threads) } ;
    }
    }
    }
if ( !almost_equal(counter,float { N0 }, 0.1)  ) {
    std::cerr << "Expected: " << N0 << " Got: " << counter << std::endl;
    std::exit(112);
}
}
int main()
{
    test_target_teams__parallel__simd();
}