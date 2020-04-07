#include <iostream>
#include <stdexcept>
#include <omp.h>

#include <cmath>
#include <limits>
template<class T>
bool almost_equal(T x, T y, int ulp) {
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<T>::min();
}

template<class T>
void test_target_simd(){

 // Input and Outputs
 
 const int L = 5;

T counter{};

// Main program

#pragma omp target simd  reduction(+:counter)   defaultmap(tofrom:scalar) 

    for (int i = 0 ; i < L ; i++ )

{




counter = counter + 1;



}


// Validation
if ( !almost_equal(counter,T{ L }, 1)  ) {
    std::cerr << "Expected: " << L << " Get: " << counter << std::endl;
    throw std::runtime_error( "target_simd give incorect value when offloaded");
}

}
int main()
{
    test_target_simd<double>();
}