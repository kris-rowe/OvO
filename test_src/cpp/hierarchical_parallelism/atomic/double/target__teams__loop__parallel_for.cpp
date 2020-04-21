#include <iostream>
#include <cmath>
#include <stdexcept>
#
bool almost_equal(double x, double gold, float tol) {
        return gold * (1-tol) <= x && x <= gold * ( 1+tol );
}
void test_target__teams__loop__parallel_for(){
 // Input and Outputs
 const int L = 5;
 const int M = 6;
double counter{};
// Main program
#pragma omp target  map(tofrom:counter) 
{
#pragma omp teams 
{
#pragma omp loop 
    for (int i = 0 ; i < L ; i++ )
{
#pragma omp parallel for 
    for (int j = 0 ; j < M ; j++ )
{
#pragma omp atomic update
counter += double { 1 };
}
}
}
}
// Validation
if ( !almost_equal(counter,double { L*M }, 0.1)  ) {
    std::cerr << "Expected: " << L*M << " Got: " << counter << std::endl;
    throw std::runtime_error( "target__teams__loop__parallel_for give incorect value when offloaded");
}
}
int main()
{
    test_target__teams__loop__parallel_for();
}