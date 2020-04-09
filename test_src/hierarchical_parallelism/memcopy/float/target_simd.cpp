#include <cassert>
#include <cmath>
#include <limits>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <algorithm>
#include <vector>

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y)
{
    //Let say 2 ulp is good enough...
	  int ulp = 2;
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp
        // unless the result is subnormal
        || std::fabs(x-y) < std::numeric_limits<T>::min();
}

template<class T>
void test_target_simd(){
  // Input and Outputs
  
  const int L = 5;
  const int size = L;
  std::vector<T> A(size);
  std::vector<T> B(size);
  std::generate(B.begin(), B.end(), std::rand);

  T *pA = A.data();
  T *pB = B.data();

// Main program

#pragma omp target simd   map(from: pA[0:L]) map(to: pB[0:L]) 

    for (int i = 0 ; i < L ; i++ )

{


pA[ i ] = pB [ i ];

 } 

// Validation
assert(std::equal(A.begin(), A.end(), B.begin(), almost_equal<T>));
 
}

int main()
{
    test_target_simd<double>();
}