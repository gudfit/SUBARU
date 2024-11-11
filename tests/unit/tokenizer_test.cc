#include "../../include/tokenizer.h"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("Tokenizer Basic Operations", "[tokenizer]") {
    SECTION("Creating tokenizer with valid input") {
        REQUIRE_NOTHROW(Tokenizer("tests/test.subaru")); // Simple Hello World
        REQUIRE_NOTHROW(
          Tokenizer("tests/test1.subaru")); // Hello World with REM
        REQUIRE_NOTHROW(Tokenizer("tests/test2.subaru")); // Basic IF statement
        REQUIRE_NOTHROW(
          Tokenizer("tests/test3.subaru")); // IF statement with REM
        REQUIRE_NOTHROW(Tokenizer("tests/test4.subaru")); // Complex arithmetic
    }

    SECTION("Creating tokenizer with invalid file throws") {
        REQUIRE_THROWS_AS(Tokenizer("nonexistent.subaru"), std::runtime_error);
    }
}

TEST_CASE("Tokenizer Line Number Processing", "[tokenizer]") {
    Tokenizer tokenizer("tests/test.subaru");

    SECTION("Line number recognition") {
        REQUIRE(tokenizer.get_num() == 10);
        tokenizer.next_token(); // Move past line number
    }
}

TEST_CASE("Tokenizer String Handling", "[tokenizer]") {
    Tokenizer tokenizer("tests/test.subaru");

    SECTION("Hello World string") {
        // Skip to string: "10 PRINT" -> string
        tokenizer.next_token(); // Skip line number
        tokenizer.next_token(); // Skip PRINT
        REQUIRE(tokenizer.get_string() == "Hello, World!");
    }
}

TEST_CASE("Tokenizer REM Statement", "[tokenizer]") {
    Tokenizer tokenizer("tests/test1.subaru");

    SECTION("REM statement handling") {
        tokenizer.skip_to_eol(); // Should skip the entire REM line
        REQUIRE_FALSE(
          tokenizer.finished()); // Should have more tokens after REM
    }
}

TEST_CASE("Tokenizer Variable Recognition", "[tokenizer]") {
    Tokenizer tokenizer("tests/test2.subaru");

    SECTION("Variable assignment") {
        // Skip to 'a' in "LET a = 5"
        tokenizer.next_token();                 // Skip line number
        tokenizer.next_token();                 // Skip LET
        REQUIRE(tokenizer.variable_num() == 0); // 'a' should be 0
    }
}

TEST_CASE("Tokenizer Complex Expressions", "[tokenizer]") {
    Tokenizer tokenizer("tests/test4.subaru");

    SECTION("Multiple operations") {
        bool found_arithmetic = false;
        while (!tokenizer.finished()) {
            const auto& data = tokenizer.get_token_data();
            // Just checking if we can iterate through tokens
            if (std::holds_alternative<int>(data)) {
                found_arithmetic = true;
                break;
            }
            tokenizer.next_token();
        }
        REQUIRE(found_arithmetic);
    }
}

TEST_CASE("Tokenizer Reset Functionality", "[tokenizer]") {
    Tokenizer tokenizer("tests/test.subaru");

    SECTION("Reset after navigation") {
        auto first_token_data = tokenizer.get_token_data();
        tokenizer.next_token();
        tokenizer.next_token();
        tokenizer.reset();
        REQUIRE(tokenizer.get_token_data() == first_token_data);
    }
}

TEST_CASE("Tokenizer String Content", "[tokenizer]") {
    Tokenizer tokenizer("tests/test1.subaru");

    SECTION("Navigate and check string content") {
        tokenizer.skip_to_eol(); // Skip REM line
        tokenizer.next_token();  // Skip line number (10)
        tokenizer.next_token();  // Skip PRINT
        REQUIRE(tokenizer.get_string() == "Hello, World!");
    }
}

#define CATCH_CONFIG_MAIN
