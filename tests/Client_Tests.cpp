#include <catch.hpp>
#include "Client.hpp"

using namespace clientServer::client;

TEST_CASE("testing client create_endpoint", "[client]") {
  auto cl = Client{};

  SECTION("returns endpoint if params are valid") {
    auto ep = cl.create_endpoint("127.0.0.1", 3333);
    REQUIRE(ep.port() == 3333);
    REQUIRE(ep.address().is_v4());
    REQUIRE(ep.address().to_string() == "127.0.0.1");
  }

  SECTION("throw if provided ip address is invalid") {
    REQUIRE_THROWS_MATCHES(
        cl.create_endpoint("abcd", 3333),
        std::invalid_argument,
        Catch::Matchers::Predicate<std::invalid_argument>(
            [](std::invalid_argument const & e)
            {
              return e.what() == std::string(
                  "Failed to parce IP address. Error #: 22. Message: Invalid argument");
            }
        )
    );
    REQUIRE_THROWS_MATCHES(
        cl.create_endpoint("3555.0.0.0", 3333),
        std::invalid_argument,
        Catch::Matchers::Predicate<std::invalid_argument>(
            [](std::invalid_argument const & e)
            {
              return e.what() == std::string(
                  "Failed to parce IP address. Error #: 22. Message: Invalid argument");
            }
        )
    );
  }
}
