#ifndef PROJECT_INCLUDE_LIB_LIB_HPP
#define PROJECT_INCLUDE_LIB_LIB_HPP

#include <string>

namespace library {

  class libClientServer {
  public:
    int64_t addition(int32_t i, int32_t j);
    void generate_throw();
  };

  class Server {
  public:
    int create_endpoint();
  };
}
#endif //PROJECT_INCLUDE_LIB_LIB_HPP