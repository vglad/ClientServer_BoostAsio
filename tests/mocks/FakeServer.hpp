#ifndef CLIENTSERVER_TESTS_MOCKS_FAKESERVER_HPP
#define CLIENTSERVER_TESTS_MOCKS_FAKESERVER_HPP

#include <catch.hpp>
#include <catch2/trompeloeil.hpp>
#include "Server.hpp"

namespace clientServer::mocks {
  using namespace clientServer::server;

  class FakeServer : public Server {
  public:
    int get_ec_value(boost::system::error_code const & ec) const noexcept override {
      return Server::get_ec_value(ec);
    }
  };

  class MockServer : public Server {
  public:
    MAKE_MOCK1(get_ec_value, int(boost::system::error_code const & ec), const noexcept);
  };

}

#endif //CLIENTSERVER_TESTS_MOCKS_FAKESERVER_HPP
