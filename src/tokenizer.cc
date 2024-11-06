#ifndef DEBUG_MODE
#define DEBUG_LOG(x) std::cerr << "DEBUG: " << x << std::endl
#else
#define DEBUG_LOG(x)
#endif

/******************************************************************************/
#include "../include/tokenizer.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

/******************************************************************************/

/**
 * Constructs a new Tokenizer object from a source string
 *
 * @param source The input string to be tokenized
 *
 * Initializes:
 * - IO stream with the source
 * - Current token to ERROR
 * - Token data to empty state
 * - Keywords map with language keywords
 * - Gets the first token from input
 */
Tokenizer::Tokenizer(const std::string& source)
  : io_(std::make_unique<IO>(source))
  , current_token_(TokenType::ERROR)
  , token_data_(std::monostate())
{
    // Initialize keywords
    keywords_ = { { "let", TokenType::LET },   { "if", TokenType::IF },
                  { "then", TokenType::THEN }, { "print", TokenType::PRINT },
                  { "rem", TokenType::REM },   { "goto", TokenType::GOTO } };

    // Initialize the first token
    current_token_ = get_next_token();
}

/**
 * Destructor for Tokenizer class
 *
 * Ensures proper cleanup by:
 * - Checking if IO stream exists
 * - Closing IO stream if it exists
 */
Tokenizer::~Tokenizer()
{
    if (io_) {
        io_->close();
    }
}

/**
 * Resets the tokenizer to its initial state by:
 * 1. Resetting the input/output stream
 * 2. Clearing any stored token data
 * 3. Getting the first token from the input
 *
 * @param none
 * @return none
 */
void
Tokenizer::reset()
{
    DEBUG_LOG("Resetting tokenizer");
    io_->reset();
    token_data_ = std::monostate();
    current_token_ = get_next_token();
}

/**
 * Sets the current token to a specific type without processing input
 *
 * @param to The TokenType to set as the current token
 * @return none
 */
void
Tokenizer::reset(TokenType to)
{
    current_token_ = to;
}

/**
 * Looks at the next character in the input stream without advancing
 *
 * @param none
 * @return char The next character in the input stream
 */
char
Tokenizer::peek_char()
{
    return io_->peek();
}

/**
 * Advances the input stream by one character
 *
 * @param none
 * @return none
 */
void
Tokenizer::skip_char()
{
    io_->next();
}

/**
 * Converts a TokenType enum value to its corresponding string representation
 *
 * @param token The TokenType enum value to convert
 * @return std::string The string representation of the token type:
 *         - Returns the exact token name for known tokens (e.g., "ERROR",
 * "NUMBER", "LETTER")
 *         - Returns "UNKNOWN_TOKEN" for undefined token types
 */
std::string
Tokenizer::token_to_string(TokenType token) const
{
    switch (token) {
        case TokenType::ERROR:
            return "ERROR";
        case TokenType::EOF_TOKEN:
            return "EOF_TOKEN";
        case TokenType::NUMBER:
            return "NUMBER";
        case TokenType::LETTER:
            return "LETTER";
        case TokenType::STRING:
            return "STRING";
        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::LT:
            return "LT";
        case TokenType::GT:
            return "GT";
        case TokenType::LT_EQ:
            return "LT_EQ";
        case TokenType::GT_EQ:
            return "GT_EQ";
        case TokenType::NOT_EQUAL:
            return "NOT_EQUAL";
        case TokenType::SEPARATOR:
            return "SEPARATOR";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::ASTERISK:
            return "ASTERISK";
        case TokenType::SLASH:
            return "SLASH";
        case TokenType::LET:
            return "LET";
        case TokenType::IF:
            return "IF";
        case TokenType::THEN:
            return "THEN";
        case TokenType::PRINT:
            return "PRINT";
        case TokenType::REM:
            return "REM";
        case TokenType::GOTO:
            return "GOTO";
        case TokenType::LEFT_PAREN:
            return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";
        case TokenType::EOL:
            return "EOL";
        default:
            return "UNKNOWN_TOKEN";
    }
}

/**
 * Retrieves the data associated with the current token
 *
 * @param none
 * @return const TokenData& A reference to the current token's data:
 *         - Could be a number (int)
 *         - Could be a letter (char)
 *         - Could be a string (std::string)
 *         - Could be empty (std::monostate)
 */
const Tokenizer::TokenData&
Tokenizer::get_token_data() const
{
    return token_data_;
}
