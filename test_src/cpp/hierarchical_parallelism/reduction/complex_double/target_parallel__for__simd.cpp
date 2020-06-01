#include <iostream>
#include <cstdlib>
#include <cmath>
#include <complex>
using namespace std;
bool almost_equal(complex<double> x, complex<double> gold, float tol) {
        return abs(gold) * (1-tol) <= abs(x) && abs(x) <= abs(gold) * (1 + tol);
}
#if !defined(_NO_UDR)
#pragma omp declare reduction(+: complex<double>: omp_out += omp_in)
#endif
void test_target_parallel__for__simd(){
 const int L = 4096;
 const int M = 64;
 complex<double> counter{};
#pragma omp target parallel reduction(+: counter) map(tofrom: counter) 
#pragma omp for
    for (int i = 0 ; i < L ; i++ )
    {
#pragma omp simd reduction(+: counter)
    for (int j = 0 ; j < M ; j++ )
    {
counter += complex<double> { 1.0f };
    }
    }
if ( !almost_equal(counter,complex<double> { L*M }, 0.1)  ) {
    std::cerr << "Expected: " << L*M << " Got: " << counter << std::endl;
    std::exit(112);
}
}
int main()
{
    test_target_parallel__for__simd();
}
