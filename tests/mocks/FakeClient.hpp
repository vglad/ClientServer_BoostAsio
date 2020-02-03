#ifndef CLIENTSERVER_TESTS_MOCKS_FAKECLIENT_HPP
#define CLIENTSERVER_TESTS_MOCKS_FAKECLIENT_HPP

#include <catch.hpp>
#include <catch2/trompeloeil.hpp>
#include "Client.hpp"

namespace clientServer::mocks {
  using namespace clientServer::client;

  class FakeClient : public Client {
  public:
    int get_ec_value(boost::system::error_code const & ec) const noexcept override {
      return Client::get_ec_value(ec);
    }
  };

  class MockClient : public Client {
  public:
    MAKE_MOCK1(get_ec_value, int(boost::system::error_code const & ec), const noexcept);
  };

}

#endif //CLIENTSERVER_TESTS_MOCKS_FAKECLIENT_HPP
