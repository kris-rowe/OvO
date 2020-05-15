#include <iostream>
#include <cstdlib>
#include <cmath>
#include <complex>
using namespace std;
bool almost_equal(complex<float> x, complex<float> gold, float tol) {
        return abs(gold) * (1-tol) <= abs(x) && abs(x) <= abs(gold) * (1 + tol);
}
#pragma omp declare reduction(+: complex<float>: omp_out += omp_in)
void test_target__teams__loop__parallel_for__simd(){
 const int L = 64;
 const int M = 64;
 const int N = 64;
 complex<float> counter{};
#pragma omp target map(tofrom:counter) 
#pragma omp teams reduction(+: counter)
#pragma omp loop
    for (int i = 0 ; i < L ; i++ )
    {
#pragma omp parallel for reduction(+: counter)
    for (int j = 0 ; j < M ; j++ )
    {
#pragma omp simd reduction(+: counter)
    for (int k = 0 ; k < N ; k++ )
    {
counter += complex<float> { 1.0f };
    }
    }
    }
if ( !almost_equal(counter,complex<float> { L*M*N }, 0.1)  ) {
    std::cerr << "Expected: " << L*M*N << " Got: " << counter << std::endl;
    std::exit(112);
}
}
int main()
{
    test_target__teams__loop__parallel_for__simd();
}