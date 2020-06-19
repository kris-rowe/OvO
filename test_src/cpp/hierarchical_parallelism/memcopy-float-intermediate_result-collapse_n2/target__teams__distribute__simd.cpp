#include <iostream>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
bool almost_equal(float x, float y, int ulp) {
  return std::fabs(x-y) <= std::numeric_limits<float>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<float>::min();
}
void test_target__teams__distribute__simd() {
  const int N0 { 23 };
  const int N1 { 23 };
  const int N2 { 23 };
  const int N3 { 23 };
  const int size = N0*N1*N2*N3;
  std::vector<float> dst(size), src(size);
  std::generate(src.begin(), src.end(), std::rand);
  float *pS { src.data() };
  float *pD { dst.data() };
  #pragma omp target map(from: pS[0:size]) map(to: pD[0:size])
  #pragma omp teams
  #pragma omp distribute collapse(2)
  for (int i0 = 0 ; i0 < N0 ; i0++ )
  for (int i1 = 0 ; i1 < N1 ; i1++ )
  {
    #pragma omp simd collapse(2)
    for (int i2 = 0 ; i2 < N2 ; i2++ )
    for (int i3 = 0 ; i3 < N3 ; i3++ )
    {
      const int idx = i3+N3*(i2+N2*(i1+N1*(i0)));
      pD[idx] = pS[idx];
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
    test_target__teams__distribute__simd();
}
