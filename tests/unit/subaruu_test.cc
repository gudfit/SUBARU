#include "../../include/subaruu.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("SUBARU Basic Execution", "[subaru]") {
    SECTION("Running test.subaru") {
        // Capture output
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        // Run interpreter within a lambda passed to REQUIRE_NOTHROW
        REQUIRE_NOTHROW([&]() {
            SUBARUU interpreter("tests/test.subaru");
            interpreter.run();
        }());

        // Restore cout
        std::cout.rdbuf(old_cout);

        // Check output
        REQUIRE(output.str() == "Hello, World!\n");
    }
}

TEST_CASE("SUBARUU REM Statement Handling", "[subaru]") {
    SECTION("Running test1.subaru") {
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            SUBARUU interpreter("tests/test1.subaru");
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);

        REQUIRE(output.str() == "Hello, World!\n");
    }
}

TEST_CASE("SUBARU IF and GOTO Statements", "[subaru]") {
    SECTION("Running test2.subaru") {
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            SUBARUU interpreter("tests/test2.subaru");
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);

        REQUIRE(output.str() == "The value of a is:  5\nDone!\n");
    }
}

TEST_CASE("SUBARU REM with IF and GOTO", "[subaru]") {
    SECTION("Running test3.subaru") {
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            SUBARUU interpreter("tests/test3.subaru");
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);

        REQUIRE(output.str() == "The value of a is:  5\nDone!\n");
    }
}

TEST_CASE("SUBARU Complex Arithmetic", "[subaru]") {
    SECTION("Running test4.subaru") {
        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            SUBARUU interpreter("tests/test4.subaru");
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);

        std::string expected_output = "a + b =  8\n"
                                      "a - b =  2\n"
                                      "a * b =  15\n"
                                      "a / b =  1\n"
                                      "(a + b) * 2 =  16\n"
                                      "a * b + c / d =  19\n";

        REQUIRE(output.str() == expected_output);
    }
}

TEST_CASE("SUBARUU GOTO Statement Tests", "[subaru]") {
    SECTION("Basic GOTO with IF") {
        std::string temp_filename = "temp_goto_test1.subaru";
        std::ofstream temp_file(temp_filename);
        temp_file << "REM Testing basic GOTO with IF\n"
                  << "10 LET a = 5\n"
                  << "20 IF a > 3 THEN 40\n"
                  << "30 PRINT \"This should not print\"\n"
                  << "40 PRINT \"Test passed\"\n";
        temp_file.close();

        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            SUBARUU interpreter(temp_filename);
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);
        REQUIRE(output.str() == "Test passed\n");

        std::filesystem::remove(temp_filename);
    }

    SECTION("GOTO with Multiple Conditions") {
        std::string temp_filename = "temp_goto_test2.subaru";
        std::ofstream temp_file(temp_filename);
        temp_file << "REM Testing multiple conditions\n"
                  << "10 LET a = 1\n"
                  << "20 IF a < 3 THEN 40\n"
                  << "30 PRINT \"Should not print\"\n"
                  << "40 LET a = a + 1\n"
                  << "50 IF a < 3 THEN 20\n"
                  << "60 PRINT \"Done\"\n";
        temp_file.close();

        std::stringstream output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());

        REQUIRE_NOTHROW([&]() {
            SUBARUU interpreter(temp_filename);
            interpreter.run();
        }());

        std::cout.rdbuf(old_cout);
        REQUIRE(output.str() == "Done\n");

        std::filesystem::remove(temp_filename);
    }
}
#define CATCH_CONFIG_MAIN
