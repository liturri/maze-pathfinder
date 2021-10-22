#ifndef BENCHMARK_HXX
#define BENCHMARK_HXX

#include <iostream>
#include <chrono>

class Benchmark
{
public:
   Benchmark() : beg_(clock_::now()) {}
   void reset() { beg_ = clock_::now(); }
   double checkpoint()
   {
      auto t = elapsed();
      reset();
      return t;
   }
   double elapsed() const
   {
      return std::chrono::duration_cast<second_>(clock_::now() - beg_).count();
   }

private:
   typedef std::chrono::high_resolution_clock clock_;
   typedef std::chrono::duration<double, std::ratio<1>> second_;
   std::chrono::time_point<clock_> beg_;
};

#endif
