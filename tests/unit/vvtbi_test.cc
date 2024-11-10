#include "../../include/vvtbi.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("VVTBI Basic Execution", "[vvtbi]") {
    SECTION("Running test.vvtb") {
        // Capture output
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        // Run interpreter within a lambda passed to REQUIRE_NOTHROW
        REQUIRE_NOTHROW([&]() {
            VVTBI interpreter("tests/test.vvtb");
            interpreter.run();
        }());

        // Restore cout
        std::cout.rdbuf(old_cout);

        // Check output
        REQUIRE(output.str() == "Hello, World!\n");
    }
}

TEST_CASE("VVTBI REM Statement Handling", "[vvtbi]") {
    SECTION("Running test1.vvtb") {
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            VVTBI interpreter("tests/test1.vvtb");
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);

        REQUIRE(output.str() == "Hello, World!\n");
    }
}

TEST_CASE("VVTBI IF and GOTO Statements", "[vvtbi]") {
    SECTION("Running test2.vvtb") {
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            VVTBI interpreter("tests/test2.vvtb");
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);

        REQUIRE(output.str() == "The value of a is: 5\nDone!\n");
    }
}

TEST_CASE("VVTBI REM with IF and GOTO", "[vvtbi]") {
    SECTION("Running test3.vvtb") {
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            VVTBI interpreter("tests/test3.vvtb");
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);

        REQUIRE(output.str() == "The value of a is: 5\nDone!\n");
    }
}

TEST_CASE("VVTBI Complex Arithmetic", "[vvtbi]") {
    SECTION("Running test4.vvtb") {
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            VVTBI interpreter("tests/test4.vvtb");
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);

        std::string expected_output = "a + b = 8\n"
                                      "a - b = 2\n"
                                      "a * b = 15\n"
                                      "a / b = 1\n"
                                      "(a + b) * 2 = 16\n"
                                      "a * b + c / d = 19\n";

        REQUIRE(output.str() == expected_output);
    }
}
#define CATCH_CONFIG_MAIN
