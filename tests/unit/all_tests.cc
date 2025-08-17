
#define CATCH_CONFIG_MAIN
#include "../../include/io.h"
#include "../../include/subaruu.h"
#include "../../include/tokenizer.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>

TEST_CASE("IO basics") {
    REQUIRE_NOTHROW(IO("tests/test.subaru"));
    IO io("tests/test.subaru");
    char first = io.peek();
    REQUIRE_FALSE(io.eof());
    io.next();
    REQUIRE(io.peek() != first);
    io.reset();
    REQUIRE(io.peek() == first);
    REQUIRE(io.to_string(5).size() <= 5);
}

TEST_CASE("Tokenizer string and line number") {
    Tokenizer t("tests/test.subaru");
    REQUIRE(t.current_token() == Tokenizer::TokenType::NUMBER);
    REQUIRE(t.get_num() == 10);
    t.next_token();
    t.next_token();
    REQUIRE(t.get_string() == "Hello, World!");
}

TEST_CASE("Interpreter: hello + control flow + arithmetic") {
    {
        std::stringstream out;
        auto* old = std::cout.rdbuf(out.rdbuf());
        SUBARUU interp("tests/test.subaru");
        interp.run();
        std::cout.rdbuf(old);
        REQUIRE(out.str() == "Hello, World!\n");
    }
    {
        std::stringstream out;
        auto* old = std::cout.rdbuf(out.rdbuf());
        SUBARUU interp("tests/test2.subaru");
        interp.run();
        std::cout.rdbuf(old);
        REQUIRE(out.str() == "The value of a is:  5\nDone!\n");
    }
    {
        std::stringstream out;
        auto* old = std::cout.rdbuf(out.rdbuf());
        SUBARUU interp("tests/test4.subaru");
        interp.run();
        std::cout.rdbuf(old);
        const std::string expected = "a + b =  8\n"
                                     "a - b =  2\n"
                                     "a * b =  15\n"
                                     "a / b =  1\n"
                                     "(a + b) * 2 =  16\n"
                                     "a * b + c / d =  19\n";
        REQUIRE(out.str() == expected);
    }
}
