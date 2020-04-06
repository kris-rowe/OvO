#include <cassert>
#include <iostream>
#
void test_target_teams(){

 // Input and Outputs
 

int counter = 0;

// Main program

#pragma omp target teams  reduction(+:counter)   defaultmap(tofrom:scalar) 

{


counter++;

 } 

// Validation
auto bo = ( counter > 0 ) ;
if ( bo != true) {
    std::cerr << "Expected: " << 0 << " Get: " << counter << std::endl;
    assert(bo);
}

}
int main()
{
    test_target_teams();
}