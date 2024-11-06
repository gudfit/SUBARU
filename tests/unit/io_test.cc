#include "../../include/io.h"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("IO Basic File Operations", "[io]") {
  SECTION("Opening existing test files") {
    REQUIRE_NOTHROW(IO("tests/test.vvtb"));
    REQUIRE_NOTHROW(IO("tests/test1.vvtb"));
    REQUIRE_NOTHROW(IO("tests/test2.vvtb"));
  }

  SECTION("Opening non-existent file throws") {
    REQUIRE_THROWS_AS(IO("tests/nonexistent.vvtb"), std::runtime_error);
  }
}

TEST_CASE("IO Reading Operations", "[io]") {
  IO io("tests/test.vvtb");

  SECTION("Reading characters sequentially") {
    // Store initial character
    char first = io.peek();
    REQUIRE_FALSE(io.eof());

    io.next();
    REQUIRE(io.peek() != first);
  }

  SECTION("Reset functionality") {
    char first = io.peek();
    io.next();
    io.next();
    io.reset();
    REQUIRE(io.peek() == first);
  }
}

TEST_CASE("IO String Operations", "[io]") {
  IO io("tests/test.vvtb");

  SECTION("to_string basic functionality") {
    char buffer[50];
    io.to_string(buffer, 5);
    REQUIRE(buffer[5] == '\0'); // Null terminator
    REQUIRE(strlen(buffer) <= 5);
  }
}

TEST_CASE("IO File Navigation", "[io]") {
  IO io("tests/test.vvtb");

  SECTION("Seek operations") {
    io.seek(5, std::ios::beg);
    char at_5 = io.peek();

    io.seek(0, std::ios::beg);
    io.next();
    io.next();
    io.next();
    io.next();
    io.next();

    REQUIRE(io.peek() == at_5);
  }
}

TEST_CASE("IO File Switching", "[io]") {
  IO io("tests/test1.vvtb");

  SECTION("Switch to another test file") {
    char first_file_char = io.peek();
    io.set("tests/test2.vvtb");
    REQUIRE(io.peek() != first_file_char);
  }

  SECTION("Switch to invalid file throws") {
    REQUIRE_THROWS_AS(io.set("tests/nonexistent.vvtb"), std::runtime_error);
  }
}

#define CATCH_CONFIG_MAIN
