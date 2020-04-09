#include <iostream>
#include <stdexcept>

#include <omp.h>


#include <cmath>
#include <limits>


#include <complex>
using namespace std;


bool almost_equal(complex<double> x, complex<double> y, int ulp) {

    bool r = std::fabs(x.real()-y.real()) <= std::numeric_limits<double>::epsilon() * std::fabs(x.real()+y.real()) * ulp ||  std::fabs(x.real()-y.real()) < std::numeric_limits<double>::min();
    bool i = std::fabs(x.imag()-y.imag()) <= std::numeric_limits<double>::epsilon() * std::fabs(x.imag()+y.imag()) * ulp ||  std::fabs(x.imag()-y.imag()) < std::numeric_limits<double>::min();
    return r && i;

}


#pragma omp declare reduction(ComplexReduction: complex<double>:   omp_out += omp_in) 


void test_target__teams_distribute__parallel(){

 // Input and Outputs
 
 const int L = 5;

complex<double> counter{};

// Main program

#pragma omp target   map(tofrom:counter) 

{


#pragma omp teams distribute  reduction(  ComplexReduction  :counter)  

    for (int i = 0 ; i < L ; i++ )

{


#pragma omp parallel  reduction(  ComplexReduction  :counter)  

{

const int num_threads = omp_get_num_threads();




counter += complex<double> { 1./num_threads };



}

}

}


// Validation
if ( !almost_equal(counter,complex<double> { L }, 10)  ) {
    std::cerr << "Expected: " << L << " Get: " << counter << std::endl;
    throw std::runtime_error( "target__teams_distribute__parallel give incorect value when offloaded");
}

}
int main()
{
    test_target__teams_distribute__parallel();
}
