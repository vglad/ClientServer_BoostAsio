#include <catch.hpp>

#include "Server.hpp"
#include "Common.hpp"

using namespace clientServer::server;
using namespace clientServer::common;
using namespace boost::asio;

TEST_CASE("testing server create_endpoint", "[server]") {
  auto srv = Server{};

  SECTION("returns endpoint with specified port number, "
          "TCP protocol and IP address v4") {
    auto ep = srv.create_endpoint<ip::tcp::endpoint, ip::address_v4>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::TCP));
  }

  SECTION("returns endpoint with specified port number, "
          "TCP protocol and IP address v6") {
    auto ep = srv.create_endpoint<ip::tcp::endpoint, ip::address_v6>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v6());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::TCP));
  }

  SECTION("returns endpoint with specified port number, "
          "UDP protocol and IP address v4") {
    auto ep = srv.create_endpoint<ip::udp::endpoint, ip::address_v4>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::UDP));
  }

  SECTION("returns endpoint with specified port number, "
          "UDP protocol and IP address v6") {
    auto ep = srv.create_endpoint<ip::udp::endpoint, ip::address_v6>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v6());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::UDP));
  }

}