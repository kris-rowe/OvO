#include <cmath>
#include <iomanip> 
#include <limits>
#include <iostream>
#include <cstdlib>
using namespace std;
bool almost_equal(long double x, long double y, int ulp) {
     return std::fabs(x-y) <= std::numeric_limits<long double>::epsilon() * std::fabs(x+y) * ulp ||  std::fabs(x-y) < std::numeric_limits<long double>::min();
}
void test_assoc_legendrel(){
   unsigned in0 {  1 };
   unsigned in1 {  1 };
   long double in2 {  0.42 };
   long double out3_host;
   long double out3_device;
    out3_host =  assoc_legendrel( in0, in1, in2);
   #pragma omp target map(from: out3_device )
   {
     out3_device =  assoc_legendrel( in0, in1, in2);
   }
   if ( !almost_equal(out3_host,out3_device, 4) ) {
        std::cerr << std::setprecision (std::numeric_limits<long double>::max_digits10 ) << "Host: " << out3_host << " GPU: " << out3_device << std::endl;
        std::exit(112);
    }
}
int main()
{
    test_assoc_legendrel();
}
