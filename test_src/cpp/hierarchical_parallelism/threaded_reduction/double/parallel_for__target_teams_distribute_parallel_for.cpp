#include <iostream>
#include <cstdlib>
bool almost_equal(double x, double gold, float tol) {
        return gold * (1-tol) <= x && x <= gold * (1 + tol);
}
void test_parallel_for__target_teams_distribute_parallel_for(){
 const int L = 4096;
 const int M = 64;
 double counter{};
#pragma omp parallel for reduction(+: counter)
    for (int i = 0 ; i < L ; i++ )
    {
#pragma omp target teams distribute parallel for reduction(+: counter) map(tofrom: counter) 
    for (int j = 0 ; j < M ; j++ )
    {
counter += double { 1.0f };
    }
    }
if ( !almost_equal(counter,double { L*M }, 0.1)  ) {
    std::cerr << "Expected: " << L*M << " Got: " << counter << std::endl;
    std::exit(112);
}
}
int main()
{
    test_parallel_for__target_teams_distribute_parallel_for();
}
