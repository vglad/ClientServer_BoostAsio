#include <catch.hpp>

#include "Client.hpp"
#include "Common.hpp"

using namespace clientServer::client;
using namespace clientServer::common;

TEST_CASE("testing client create_endpoint", "[client]") {
  auto cl = Client{};

  SECTION("returns endpoint if params are valid and protocol is TCP") {
    auto ep = cl.create_endpoint<asio::ip::tcp::endpoint>("127.0.0.1", 3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.address().to_string() == "127.0.0.1");
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::TCP));
  }

  SECTION("returns endpoint if params are valid and protocol is UDP") {
    auto ep = cl.create_endpoint<asio::ip::udp::endpoint>("127.0.0.1", 3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.address().to_string() == "127.0.0.1");
    REQUIRE(ep.protocol().protocol() == static_cast<int>(Protocol::UDP));
  }

  SECTION("throw if provided ip address is invalid") {
    REQUIRE_THROWS_MATCHES(
        cl.create_endpoint<asio::ip::tcp::endpoint>("abcd", 3333),
        std::invalid_argument,
        Catch::Matchers::Predicate<std::invalid_argument>(
            [](std::invalid_argument const & e)
            {
              return e.what() == std::string(
                  "Failed to parce IP address. Error #: 22. Message: "
                  "Invalid argument");
            }
        )
    );
    REQUIRE_THROWS_MATCHES(
        cl.create_endpoint<asio::ip::udp::endpoint>("3555.0.0.0", 3333),
        std::invalid_argument,
        Catch::Matchers::Predicate<std::invalid_argument>(
            [](std::invalid_argument const & e)
            {
              return e.what() == std::string(
                  "Failed to parce IP address. Error #: 22. Message: "
                  "Invalid argument");
            }
        )
    );
  }
}
