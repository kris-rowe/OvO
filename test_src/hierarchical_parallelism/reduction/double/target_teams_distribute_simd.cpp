#include <iostream>
#include <stdexcept>


#include <cmath>
#include <limits>



bool almost_equal(double x, double y, int ulp) {

     return std::fabs(x-y) <= std::numeric_limits<double>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<double>::min();

}



void test_target_teams_distribute_simd(){

 // Input and Outputs
 
 const int L = 5;

double counter{};

// Main program

#pragma omp target teams distribute simd  reduction(  +  :counter)   map(tofrom:counter) 

    for (int i = 0 ; i < L ; i++ )

{




counter += double { 1 };



}


// Validation
if ( !almost_equal(counter,double { L }, 10)  ) {
    std::cerr << "Expected: " << L << " Get: " << counter << std::endl;
    throw std::runtime_error( "target_teams_distribute_simd give incorect value when offloaded");
}

}
int main()
{
    test_target_teams_distribute_simd();
}
