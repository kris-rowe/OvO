#include <iostream>
#include <cmath>
#include <stdexcept>
#ifdef _OPENMP
#include <omp.h>
#else
int omp_get_num_teams() {return 1;}
int omp_get_num_threads() {return 1;}
#endif
bool almost_equal(double x, double gold, float tol) {
    return gold * (1-tol) <= x && x <= gold * ( 1+tol );
}
void test_target_teams__parallel__for__simd(){
 const int L = 5;
 const int M = 6;
double counter{};
#pragma omp target teams  map(tofrom:counter) 
    {
const int num_teams = omp_get_num_teams();
double partial_counter{};
#pragma omp parallel  reduction(+: counter)  
    {
#pragma omp for 
    for (int i = 0 ; i < L ; i++ )
    {
#pragma omp simd 
    for (int j = 0 ; j < M ; j++ )
    {
partial_counter += double { 1.0f/num_teams } ;
   } 
   } 
   } 
#pragma omp atomic update
counter += partial_counter;
   } 
if ( !almost_equal(counter,double { L*M }, 0.1)  ) {
    std::cerr << "Expected: " << L*M << " Got: " << counter << std::endl;
    throw std::runtime_error( "target_teams__parallel__for__simd give incorect value when offloaded");
}
}
int main()
{
    test_target_teams__parallel__for__simd();
}
