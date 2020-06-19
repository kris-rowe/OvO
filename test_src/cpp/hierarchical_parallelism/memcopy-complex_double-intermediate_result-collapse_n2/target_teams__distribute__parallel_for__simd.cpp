#include <iostream>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <complex>
using std::complex;
bool almost_equal(complex<double> x, complex<double> y, int ulp) {
  return std::abs(x-y) <= std::numeric_limits<double>::epsilon() * std::abs(x+y) * ulp ||  std::abs(x-y) < std::numeric_limits<double>::min();
}
void test_target_teams__distribute__parallel_for__simd() {
  const int N0 { 8 };
  const int N1 { 8 };
  const int N2 { 8 };
  const int N3 { 8 };
  const int N4 { 8 };
  const int N5 { 8 };
  const int size = N0*N1*N2*N3*N4*N5;
  std::vector<complex<double>> dst(size), src(size);
  std::generate(src.begin(), src.end(), std::rand);
  complex<double> *pS { src.data() };
  complex<double> *pD { dst.data() };
  #pragma omp target teams map(from: pS[0:size]) map(to: pD[0:size])
  #pragma omp distribute collapse(2)
  for (int i0 = 0 ; i0 < N0 ; i0++ )
  for (int i1 = 0 ; i1 < N1 ; i1++ )
  {
    #pragma omp parallel for collapse(2)
    for (int i2 = 0 ; i2 < N2 ; i2++ )
    for (int i3 = 0 ; i3 < N3 ; i3++ )
    {
      #pragma omp simd collapse(2)
      for (int i4 = 0 ; i4 < N4 ; i4++ )
      for (int i5 = 0 ; i5 < N5 ; i5++ )
      {
        const int idx = i5+N5*(i4+N4*(i3+N3*(i2+N2*(i1+N1*(i0)))));
        pD[idx] = pS[idx];
      }
    }
  }
  for (int i = 0 ;  i < size ; i++)
    if ( !almost_equal(dst[i],src[i],1) ) {
      std::cerr << "Expected: " << dst[i] << " Got: " << src[i] << std::endl;
      std::exit(112);
  }
}
int main()
{
    test_target_teams__distribute__parallel_for__simd();
}
