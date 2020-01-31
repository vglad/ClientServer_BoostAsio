#ifndef PROJECT_INCLUDE_DETAIL_TIMEMEASURMENT_HPP
#define PROJECT_INCLUDE_DETAIL_TIMEMEASURMENT_HPP

#include <chrono>

namespace detail {

//*****************************************************************************
// Execution time measurement functions
// Can return container of any type

  using namespace std::chrono;
  using time_point = high_resolution_clock::time_point;

  [[maybe_unused]] static auto time_now() -> time_point {
    return high_resolution_clock::now();
  }

// supress MSVS warning about unreferenced local function
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4505)
#endif
  [[maybe_unused]] static auto time_elapsed(time_point const& start_time) -> float {
    return duration_cast<duration<float>>(time_now() - start_time).count();
  }
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
//*****************************************************************************

}

#endif //PROJECT_INCLUDE_DETAIL_TIMEMEASURMENT_HPP
