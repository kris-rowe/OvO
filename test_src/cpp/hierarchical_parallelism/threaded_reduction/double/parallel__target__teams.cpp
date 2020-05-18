#include <iostream>
#include <cstdlib>
#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_teams()   {return 1;}
int omp_get_num_threads() {return 1;}
#endif
bool almost_equal(double x, double gold, float tol) {
        return gold * (1-tol) <= x && x <= gold * (1 + tol);
}
void test_parallel__target__teams(){
 double counter{};
#pragma omp parallel reduction(+: counter)
    {
const int num_threads = omp_get_num_threads();
#pragma omp target map(tofrom: counter) 
#pragma omp teams reduction(+: counter)
    {
const int num_teams = omp_get_num_teams();
counter += double { 1.0f/(num_teams*num_threads) } ;
    }
    }
if ( !almost_equal(counter,double { 1 }, 0.1)  ) {
    std::cerr << "Expected: " << 1 << " Got: " << counter << std::endl;
    std::exit(112);
}
}
int main()
{
    test_parallel__target__teams();
}
