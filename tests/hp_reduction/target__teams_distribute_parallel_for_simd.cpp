#include <cassert>
#include <iostream>
#
void test_target__teams_distribute_parallel_for_simd(){

 // Input and Outputs
 
 const int L = 5;

int counter = 0;

// Main program

#pragma omp target   defaultmap(tofrom:scalar) 

{

#pragma omp teams distribute parallel for simd  reduction(+:counter)  

    for (int i = 0 ; i < L ; i++ )

{


counter++;

 }  } 

// Validation
auto bo = ( counter == L ) ;
if ( bo != true) {
    std::cerr << "Expected: " << L << " Get: " << counter << std::endl;
    assert(bo);
}

}
int main()
{
    test_target__teams_distribute_parallel_for_simd();
}