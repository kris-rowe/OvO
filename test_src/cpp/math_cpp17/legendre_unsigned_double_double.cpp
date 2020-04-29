#include <cmath>
#include <iomanip> 
#include <limits>
#include <iostream>
#include <cstdlib>
using namespace std;
bool almost_equal(double x, double y, int ulp) {
     return std::fabs(x-y) <= std::numeric_limits<double>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<double>::min();
}
void test_legendre(){
   unsigned in0 {  1 };
   double in1 {  0.42 };
   double out2_host;
   double out2_device;
    out2_host =  legendre( in0, in1);
   #pragma omp target map(from: out2_device )
   {
     out2_device =  legendre( in0, in1);
   }
   if ( !almost_equal(out2_host,out2_device, 4) ) {
        std::cerr << std::setprecision (std::numeric_limits<double>::max_digits10 ) << "Host: " << out2_host << " GPU: " << out2_device << std::endl;
        std::exit(112);
    }
}
int main()
{
    test_legendre();
}
