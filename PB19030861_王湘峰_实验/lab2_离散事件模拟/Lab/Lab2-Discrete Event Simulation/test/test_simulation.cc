#include "../queue.h"
#include "../priority_queue.h"
#include "../util/util.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Simulation 1", "[simulation]")
{
    REQUIRE(test_simulation(400, 1000, 3, 1, 2, 3));
}

TEST_CASE("Simulation 2", "[simulation]")
{
    REQUIRE(test_simulation(100, 500, 1, 4, 7, 2));
}

TEST_CASE("Simulation 3", "[simulation]")
{
    REQUIRE(test_simulation(100, 200, 100, 60, 110, 2));
}