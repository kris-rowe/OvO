
#include <cmath>


#include <limits>
#include <iostream>
#include <stdexcept>

using namespace std;

bool almost_equal(long long int x, long long int y, int ulp) {

    return x == y ; 

}

void test_llround(){
   
   double x {  0.42 };
   

   long long int o_host = llround( x);

   long long int o_gpu ; 
   #pragma omp target defaultmap(tofrom:scalar)
   {
   o_gpu = llround( x);
   }

   if ( !almost_equal(o_host,o_gpu,1) ) {
        std::cerr << "Host: " << o_host << " GPU: " << o_gpu << std::endl;
        throw std::runtime_error( "llround give incorect value when offloaded");
    }
}

int main()
{
    test_llround();
}
