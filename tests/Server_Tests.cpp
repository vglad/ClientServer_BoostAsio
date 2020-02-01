#include <catch.hpp>
#include "Server.hpp"

using namespace clientServer::server;

TEST_CASE("testing server create_endpoint", "[server]") {
  auto srv = Server{};

  SECTION("returns endpoint with specified port number") {
    auto ep = srv.create_endpoint(3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
  }

}