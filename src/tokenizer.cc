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
  , token_data_(std::monostate()) {
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
Tokenizer::~Tokenizer() {
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
void Tokenizer::reset() {
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
void Tokenizer::reset(TokenType to) { current_token_ = to; }

/**
 * Looks at the next character in the input stream without advancing
 *
 * @param none
 * @return char The next character in the input stream
 */
char Tokenizer::peek_char() { return io_->peek(); }

/**
 * Advances the input stream by one character
 *
 * @param none
 * @return none
 */
void Tokenizer::skip_char() { io_->next(); }

/**
 * Converts a TokenType enum value to its corresponding string representation
 *
 * @param token The TokenType enum value to convert
 * @return std::string The string representation of the token type:
 *         - Returns the exact token name for known tokens (e.g., "ERROR",
 * "NUMBER", "LETTER")
 *         - Returns "UNKNOWN_TOKEN" for undefined token types
 */
std::string Tokenizer::token_to_string(TokenType token) const {
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
const Tokenizer::TokenData& Tokenizer::get_token_data() const {
    return token_data_;
}

/**
 * Checks if the tokenizer has reached the end of input
 *
 * @param none
 * @return true if the current token is EOF_TOKEN, false otherwise
 *
 * Used to determine when tokenization is complete and no more tokens
 * are available to process from the input
 */
bool Tokenizer::finished() const {
    return current_token_ == TokenType::EOF_TOKEN;
}

/**
 * Converts a letter token into its corresponding variable number
 *
 * @param none
 * @return Integer 0-25 corresponding to letters a-z, or 0 for invalid input
 *
 * Maps lowercase letters to array indices:
 * - 'a' = 0, 'b' = 1, ..., 'z' = 25
 * - Returns 0 for non-letter tokens or if token_data isn't a char
 * - Used for variable storage/lookup in the interpreter
 */
int Tokenizer::variable_num() const {
    if (std::holds_alternative<char>(token_data_)) {
        char letter = std::get<char>(token_data_);
        switch (letter) {
            case 'a':
                return 0;
            case 'b':
                return 1;
            case 'c':
                return 2;
            case 'd':
                return 3;
            case 'e':
                return 4;
            case 'f':
                return 5;
            case 'g':
                return 6;
            case 'h':
                return 7;
            case 'i':
                return 8;
            case 'j':
                return 9;
            case 'k':
                return 10;
            case 'l':
                return 11;
            case 'm':
                return 12;
            case 'n':
                return 13;
            case 'o':
                return 14;
            case 'p':
                return 15;
            case 'q':
                return 16;
            case 'r':
                return 17;
            case 's':
                return 18;
            case 't':
                return 19;
            case 'u':
                return 20;
            case 'v':
                return 21;
            case 'w':
                return 22;
            case 'x':
                return 23;
            case 'y':
                return 24;
            case 'z':
                return 25;
            default:
                return 0; // Should not reach here
        }
    }
    return 0; // Default return value
}

/**
 * Retrieves the string value of the current token
 *
 * @param none
 * @return Reference to the token's string value if it contains a string,
 *         otherwise returns reference to empty string
 *
 * Used to access string literals and keywords from the token_data
 * Returns empty string if token_data doesn't contain a string value
 */
const std::string& Tokenizer::get_string() const {
    static const std::string empty;
    if (std::holds_alternative<std::string>(token_data_)) {
        return std::get<std::string>(token_data_);
    }
    return empty;
}

/**
 * Retrieves the numeric value of the current token
 *
 * @param none
 * @return Integer value if token contains a number, otherwise returns 0
 *
 * Used to access numeric literals from the token_data
 * Returns 0 if token_data doesn't contain an integer value
 */
int Tokenizer::get_num() const {
    if (std::holds_alternative<int>(token_data_)) {
        return std::get<int>(token_data_);
    }
    return 0; // Default return value
}

/**
 * Advances to the next token in the input stream
 *
 * @param none
 * @return none
 *
 * Skips whitespace (spaces and tabs, but preserves newlines) and updates
 * current_token_ by calling get_next_token()
 * Does nothing if tokenizer has already reached EOF
 */
void Tokenizer::next_token() {
    if (finished()) {
        return;
    }

    // Skip spaces and tabs, but not newlines
    while (io_->current() == ' ' || io_->current() == '\t') {
        io_->next();
    }

    current_token_ = get_next_token();
}

/**
 * Skips all characters until the end of the current line
 * @param none
 * @return none
 */
void Tokenizer::skip_to_eol() {
    while (io_->current() != '\n' && io_->current() != '\r' &&
           io_->current() != EOF && io_->current() != '\0') {
        io_->next();
    }
    if (io_->current() == '\r') {
        io_->next();
        if (io_->current() == '\n') {
            io_->next();
        }
    } else if (io_->current() == '\n') {
        io_->next();
    }
    current_token_ = get_next_token();
}

/**
 * Reads and classifies the next token from the input stream
 *
 * @param none
 * @return The TokenType of the next token in the input
 *
 * Updates the input stream position (io_) and token_data_ as needed
 * Returns ERROR for unrecognized characters
 * Logs debug information about character processing if DEBUG_LOG is enabled
 */

Tokenizer::TokenType Tokenizer::get_next_token() {
    int c = io_->current();
    DEBUG_LOG("get_next_token processing char: '"
              << (std::isprint(c) ? static_cast<char>(c) : ' ')
              << "' (ASCII: " << c << ")");
    // Handle EOF
    if (c == EOF || c == '\0') {
        return TokenType::EOF_TOKEN;
    }

    // Skip spaces and tabs, but not newlines
    while (c == ' ' || c == '\t') {
        io_->next();
        c = io_->current();
    }

    // Handle EOL
    if (c == '\n' || c == '\r') {
        if (c == '\r') {
            io_->next();
            if (io_->current() == '\n') {
                io_->next();
            }
        } else {
            io_->next();
        }
        return TokenType::EOL;
    }

    // Handle quoted strings
    if (c == '"') {
        return token_string();
    }

    // Handle numbers
    if (std::isdigit(c)) {
        return token_number();
    }

    // Handle keywords and variables
    if (std::isalpha(c)) {
        if (std::isupper(c)) {
            return token_keyword();
        } else {
            char letter = static_cast<char>(c);
            token_data_ = letter;
            io_->next();
            return TokenType::LETTER;
        }
    }

    // Handle separators specially
    if (c == ',' || c == ';') {
        io_->next();
        DEBUG_LOG("Found separator");
        return TokenType::SEPARATOR;
    }

    // Handle other tokens
    TokenType token = TokenType::ERROR;
    switch (c) {
        case '=':
            io_->next();
            token = TokenType::EQUAL;
            break;
        case '<':
            io_->next();
            if (io_->current() == '=') {
                io_->next();
                token = TokenType::LT_EQ;
            } else if (io_->current() == '>') {
                io_->next();
                token = TokenType::NOT_EQUAL;
            } else {
                token = TokenType::LT;
            }
            break;
        case '>':
            io_->next();
            if (io_->current() == '=') {
                io_->next();
                token = TokenType::GT_EQ;
            } else {
                token = TokenType::GT;
            }
            break;
        case '+':
            io_->next();
            token = TokenType::PLUS;
            break;
        case '-':
            io_->next();
            token = TokenType::MINUS;
            break;
        case '*':
            io_->next();
            token = TokenType::ASTERISK;
            break;
        case '/':
            io_->next();
            token = TokenType::SLASH;
            break;
        case '(':
            io_->next();
            token = TokenType::LEFT_PAREN;
            break;
        case ')':
            io_->next();
            token = TokenType::RIGHT_PAREN;
            break;
        default:
            io_->next();
            token = TokenType::ERROR;
    }
    return token;
}

/**
 * Processes relational operators from the input stream
 *
 * @param none
 * @return TokenType representing the relational operator found
 */
Tokenizer::TokenType Tokenizer::token_relation() {
    int c = io_->current();
    TokenType token = TokenType::ERROR;
    switch (c) {
        case '=':
            io_->next();
            token = TokenType::EQUAL;
            break;
        case '<':
            io_->next();
            c = io_->peek();
            if (c == '>') {
                io_->next();
                token = TokenType::NOT_EQUAL;
            } else if (c == '=') {
                io_->next();
                token = TokenType::LT_EQ;
            } else {
                token = TokenType::LT;
            }
            break;
        case '>':
            io_->next();
            c = io_->peek();
            if (c == '=') {
                io_->next();
                token = TokenType::GT_EQ;
            } else {
                token = TokenType::GT;
            }
            break;
        default:
            break;
    }
    return token;
}

/**
 * Processes arithmetic operators and parentheses from the input stream
 *
 * @param none
 * @return TokenType representing the operator or parenthesis found
 */
Tokenizer::TokenType Tokenizer::token_operation() {
    int c = io_->current();
    TokenType token = TokenType::ERROR;

    switch (c) {
        case '(':
            token = TokenType::LEFT_PAREN;
            break;
        case ')':
            token = TokenType::RIGHT_PAREN;
            break;
        case '+':
            token = TokenType::PLUS;
            break;
        case '-':
            token = TokenType::MINUS;
            break;
        case '/':
            token = TokenType::SLASH;
            break;
        case '*':
            token = TokenType::ASTERISK;
            break;
        default:
            break;
    }

    if (token != TokenType::ERROR) {
        io_->next();
    }

    return token;
}

/**
 * Processes a quoted string literal from the input stream
 *
 * @param none
 * @return TokenType::STRING if successful, even if truncated
 *
 * Behavior:
 * - Assumes current character is opening quote (")
 * - Consumes characters until closing quote or EOF/null
 * - Stores resulting string in token_data_
 * - Skips trailing whitespace (spaces/tabs) after string
 * - Preserves separators and EOL for main tokenizer
 *
 * String processing:
 * - Maximum length enforced by MAX_STRING_LITERAL
 * - Strings exceeding max length are truncated
 * - No escape sequence processing
 * - Unclosed strings are preserved as-is
 */
Tokenizer::TokenType Tokenizer::token_string() {
    std::string str;
    io_->next(); // Skip initial quote

    while (io_->current() != EOF && io_->current() != '\0') {
        char c = io_->current();
        // If we hit the closing quote, we're done
        if (c == '"') {
            io_->next(); // Skip closing quote
            token_data_ = str;
            // Skip whitespace after string
            while (io_->current() == ' ' || io_->current() == '\t') {
                io_->next();
            }
            // Don't consume separators or EOL - let the main tokenizer handle
            // them
            return TokenType::STRING;
        }
        // Add character to string if we haven't hit max length
        if (str.size() >= VVTBI_STRING_LITERAL) {
            break;
        }
        str += c;
        io_->next();
    }
    token_data_ = str;
    return TokenType::STRING;
}
/**
 * Processes language keywords from the input stream
 *
 * @param none
 * @return TokenType corresponding to the recognized keyword, or ERROR if not
 * found
 */
Tokenizer::TokenType Tokenizer::token_keyword() {
    DEBUG_LOG("Processing possible keyword");
    std::string keyword;
    // Skip any leading whitespace
    while (io_->current() == ' ' || io_->current() == '\t') {
        io_->next();
    }
    // Collect the keyword
    while (std::isalpha(io_->current())) {
        keyword += static_cast<char>(std::toupper(io_->current()));
        io_->next();
    }
    DEBUG_LOG("Found possible keyword: " << keyword);
    // REM is special - don't skip trailing whitespace for it
    if (keyword == "REM") {
        return TokenType::REM;
    }
    // Skip trailing whitespace for other keywords
    while (io_->current() == ' ' || io_->current() == '\t') {
        io_->next();
    }
    if (keyword == "PRINT")
        return TokenType::PRINT;
    if (keyword == "LET")
        return TokenType::LET;
    if (keyword == "IF")
        return TokenType::IF;
    if (keyword == "THEN")
        return TokenType::THEN;
    if (keyword == "GOTO")
        return TokenType::GOTO;
    return TokenType::ERROR;
}

/**
 * Processes numeric literals from the input stream
 *
 * @param none
 * @return TokenType::NUMBER if a valid number is found, TokenType::ERROR
 * otherwise
 *
 * On successful parsing, stores the numeric value in token_data_
 * Updates input stream position as characters are consumed
 */

Tokenizer::TokenType Tokenizer::token_number() {
    std::string num_str;

    // Skip any leading whitespace
    while (io_->current() == ' ' || io_->current() == '\t') {
        io_->next();
    }

    // Collect all digits
    while (std::isdigit(io_->current()) &&
           num_str.size() < VVTBI_NUMBER_LITERAL) {
        num_str += static_cast<char>(io_->current());
        io_->next();
    }

    if (!num_str.empty()) {
        try {
            token_data_ = std::stoi(num_str);

            // Skip whitespace after number
            while (io_->current() == ' ' || io_->current() == '\t') {
                io_->next();
            }

            // Don't automatically consume EOL
            return TokenType::NUMBER;
        } catch (const std::out_of_range&) {
            return TokenType::ERROR;
        } catch (const std::invalid_argument&) {
            return TokenType::ERROR;
        }
    }

    return TokenType::ERROR;
}

/**
 * Processes end-of-line characters from the input stream
 *
 * @param c The current character being processed
 * @return TokenType::EOL if valid line ending found, TokenType::ERROR otherwise
 */
Tokenizer::TokenType Tokenizer::token_eol(int c) {
    if (c == '\n') {
        io_->next(); // consume the newline
        return TokenType::EOL;
    }
    if (c == '\r') {
        io_->next(); // consume the \r
        if (io_->current() == '\n') {
            io_->next(); // consume the \n if it exists
        }
        return TokenType::EOL;
    }
    return TokenType::ERROR;
}
