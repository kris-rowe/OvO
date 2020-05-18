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
void test_target_parallel(){
 float counter{};
#pragma omp target parallel map(tofrom: counter) 
    {
const int num_threads = omp_get_num_threads();
#pragma omp atomic update
counter += float { 1.0f/num_threads };
    }
if ( !almost_equal(counter,float { 1 }, 0.1)  ) {
    std::cerr << "Expected: " << 1 << " Got: " << counter << std::endl;
    std::exit(112);
}
}
int main()
{
    test_target_parallel();
}
