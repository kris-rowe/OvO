#include <iostream>
#include <stdexcept>


#include <cmath>
#include <limits>



bool almost_equal(float x, float y, int ulp) {

     return std::fabs(x-y) <= std::numeric_limits<float>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<float>::min();

}



void test_target__parallel_loop(){

 // Input and Outputs
 
 const int L = 5;

float counter{};

// Main program

#pragma omp target   map(tofrom:counter) 

{


#pragma omp parallel loop  reduction(  +  :counter)  

    for (int i = 0 ; i < L ; i++ )

{




counter += float { 1 };



}

}


// Validation
if ( !almost_equal(counter,float { L }, 10)  ) {
    std::cerr << "Expected: " << L << " Get: " << counter << std::endl;
    throw std::runtime_error( "target__parallel_loop give incorect value when offloaded");
}

}
int main()
{
    test_target__parallel_loop();
}
