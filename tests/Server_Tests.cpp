#include <catch.hpp>

#include "Server.hpp"
#include "Common.hpp"

using namespace clientServer::server;
using namespace clientServer::common;

TEST_CASE("testing server create_endpoint", "[server]") {
  auto srv = Server{};

  SECTION("returns endpoint with specified port number and TCP protocol") {
    auto ep = srv.create_endpoint<asio::ip::tcp::endpoint>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::TCP));
  }

  SECTION("returns endpoint with specified port number and UDP protocol") {
    auto ep = srv.create_endpoint<asio::ip::udp::endpoint>(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::UDP));
  }

}