// tokenizer.cc

#include "../include/tokenizer.h"
#include "../include/common.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string_view>

/******************************************************************************/

/**
 * Tokenizer Constructor
 *
 * Constructs a new Tokenizer to process source text
 *
 * @param source The input string to be tokenized
 * @throws std::runtime_error if source file cannot be opened
 */
Tokenizer::Tokenizer(std::string_view source)
  : io_(std::make_unique<IO>(std::string(source)))
  , current_token_(TokenType::ERROR)
  , token_data_(std::monostate()) {
    // Initialize the first token
    current_token_ = get_next_token();
}

/**
 * Tokenizer Constructor
 *
 * Cleans up tokenizer resources
 * Ensures IO stream is properly closed if it exists
 */
Tokenizer::~Tokenizer() {
    if (io_)
        io_->close();
}

/**
 * reset
 *
 * Resets the tokenizer to initial state
 * - Resets IO stream to beginning
 * - Clears token data
 * - Gets first token from input
 *
 * @param void
 * @return void
 */
void Tokenizer::reset() {
    DEBUG_LOG("Resetting tokenizer");
    io_->reset();
    token_data_ = std::monostate();
    current_token_ = get_next_token();
}

/**
 * reset
 *
 * Sets current token to specified type without reading input
 *
 * @param to The TokenType to set as current
 * @return void
 */
void Tokenizer::reset(TokenType to) { current_token_ = to; }

/**
 * peek_char
 *
 * Looks at next character in input without advancing position
 *
 * @param void
 * @return Next character in input stream
 */
char Tokenizer::peek_char() {
    auto it = io_->position();
    auto end = io_->end();
    if (it != end) {
        auto next_it = it;
        ++next_it;
        if (next_it != end)
            return *next_it;
    }
    return '\0';
}

/**
 * skip_char
 *
 * Advances input position by one character
 *
 * @param void
 * @return void
 */
void Tokenizer::skip_char() { io_->next(); }

/**
 * token_to_string
 *
 * Converts TokenType to string representation
 *
 * @param token The TokenType to convert
 * @return String name of the token type
 */
std::string_view Tokenizer::token_to_string(TokenType token) const {
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
        case TokenType::PRINT_DOLLAR:
            return "PRINT$";
        case TokenType::TAB:
            return "TAB";
        case TokenType::REM:
            return "REM";
        case TokenType::GOTO:
            return "GOTO";
        case TokenType::LEFT_PAREN:
            return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:
            return "RIGHT_PAREN";
        case TokenType::LEFT_BRACKET:
            return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET:
            return "RIGHT_BRACKET";
        case TokenType::EOL:
            return "EOL";
        default:
            return "UNKNOWN_TOKEN";
    }
}

/**
 * get_token_data
 *
 * Retrieves current token's associated data
 *
 * @param void
 * @return Reference to variant containing:
 * - Empty state (monostate)
 * - String value
 * - Integer value
 * - Character value
 */
const Tokenizer::TokenData& Tokenizer::get_token_data() const {
    return token_data_;
}

/**
 * finished
 *
 * Checks if tokenizer has reached end of input
 *
 * @param void
 * @return true if current token is EOF_TOKEN
 */
bool Tokenizer::finished() const {
    return current_token_ == TokenType::EOF_TOKEN;
}

/**
 * variable_num
 *
 * Converts current letter token to variable number (0-25)
 * 'a'=0, 'b'=1, etc.
 *
 * @param void
 * @return Variable index 0-25, or 0 if not a letter token
 */
int Tokenizer::variable_num() const {
    if (std::holds_alternative<char>(token_data_)) {
        unsigned char letter =
          static_cast<unsigned char>(std::get<char>(token_data_));
        if (letter >= 'a' && letter <= 'z')
            return letter - 'a';
    }
    return 0;
}

/**
 * get_string
 *
 * Gets string value from current token
 *
 * @param void
 * @return String value if token contains string,
 *         empty string otherwise
 */
std::string_view Tokenizer::get_string() const {
    static const std::string empty;
    if (std::holds_alternative<std::string>(token_data_))
        return std::get<std::string>(token_data_);
    return empty;
}

/**
 * get_num
 *
 * Gets numeric value from current token
 *
 * @param void
 * @return Integer value if token contains number,
 *         0 otherwise
 */
boost::multiprecision::cpp_int Tokenizer::get_num() const {
    if (std::holds_alternative<boost::multiprecision::cpp_int>(token_data_))
        return std::get<boost::multiprecision::cpp_int>(token_data_);
    return 0;
}

/**
 * next_token
 *
 * Advances to next token in input
 * - Skips whitespace (space/tab but not newline)
 * - Updates current token
 * - Does nothing if already at EOF
 *
 * @param void
 * @return void
 */
void Tokenizer::next_token() {
    if (finished())
        return;
    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t'))
        io_->next();
    current_token_ = get_next_token();
}

/**
 * skip_to_eol
 *
 * Skips all input until end of current line
 * - Handles both \n and \r\n line endings
 * - Updates current token after skipping
 *
 * @param void
 * @return void
 */
void Tokenizer::skip_to_eol() {
    while (!io_->eof() && io_->current() != '\n' && io_->current() != '\r')
        io_->next();
    if (!io_->eof()) {
        if (io_->current() == '\r') {
            io_->next();
            if (!io_->eof() && io_->current() == '\n')
                io_->next();
        } else if (io_->current() == '\n') {
            io_->next();
        }
    }
    current_token_ = get_next_token();
}

/**
 * get_next_token
 *
 * Reads and classifies next token from input
 *
 * @param void
 * @return TokenType of next token
 * Updates:
 * - Input stream position
 * - Token data as needed
 *
 * Handles:
 * - Whitespace skipping
 * - String literals
 * - Numbers
 * - Keywords
 * - Operators and symbols
 * - Line endings
 */
Tokenizer::TokenType Tokenizer::get_next_token() {
    if (io_->eof())
        return TokenType::EOF_TOKEN;
    char c = io_->current();
    while (!io_->eof() && (c == ' ' || c == '\t')) {
        io_->next();
        c = io_->current();
    }
    if (c == '\n' || c == '\r') {
        if (c == '\r') {
            io_->next();
            if (!io_->eof() && io_->current() == '\n')
                io_->next();
        } else {
            io_->next();
        }
        return TokenType::EOL;
    }
    if (c == '"')
        return token_string();
    if (std::isdigit(static_cast<unsigned char>(c)))
        return token_number();
    if (std::isalpha(static_cast<unsigned char>(c))) {
        if (std::isupper(static_cast<unsigned char>(c)))
            return token_keyword();
        token_data_ = c;
        io_->next();
        return TokenType::LETTER;
    }
    if (c == ',' || c == ';') {
        io_->next();
        return TokenType::SEPARATOR;
    }
    switch (c) {
        case '=':
            io_->next();
            return TokenType::EQUAL;
        case '<':
            io_->next();
            if (!io_->eof() && io_->current() == '=') {
                io_->next();
                return TokenType::LT_EQ;
            }
            if (!io_->eof() && io_->current() == '>') {
                io_->next();
                return TokenType::NOT_EQUAL;
            }
            return TokenType::LT;
        case '>':
            io_->next();
            if (!io_->eof() && io_->current() == '=') {
                io_->next();
                return TokenType::GT_EQ;
            }
            return TokenType::GT;
        case '+':
            io_->next();
            return TokenType::PLUS;
        case '-':
            io_->next();
            return TokenType::MINUS;
        case '*':
            io_->next();
            return TokenType::ASTERISK;
        case '/':
            io_->next();
            return TokenType::SLASH;
        case '(':
            io_->next();
            return TokenType::LEFT_PAREN;
        case ')':
            io_->next();
            return TokenType::RIGHT_PAREN;
        case '[':
            io_->next();
            return TokenType::LEFT_BRACKET;
        case ']':
            io_->next();
            return TokenType::RIGHT_BRACKET;
        default:
            io_->next();
            return TokenType::ERROR;
    }
}

/**
 * token_string
 *
 * Processes a string literal token
 *
 * @param void
 * @return TokenType::STRING
 *
 * Assumes current char is opening quote
 * Reads until closing quote or EOF
 * Handles:
 * - Max string length (SUBARUU_STRING_LITERAL)
 * - Whitespace after string
 * - Unclosed strings preserved as-is
 */
Tokenizer::TokenType Tokenizer::token_string() {
    std::string str;
    io_->next(); // skip "
    while (!io_->eof()) {
        char c = io_->current();
        if (c == '"') {
            io_->next();
            token_data_ = str;
            while (!io_->eof() &&
                   (io_->current() == ' ' || io_->current() == '\t'))
                io_->next();
            return TokenType::STRING;
        }
        if (str.size() >= SUBARUU_STRING_LITERAL)
            break;
        str += c;
        io_->next();
    }
    token_data_ = str;
    return TokenType::STRING;
}

/**
 * token_keyword
 *
 * Processes a language keyword token
 *
 * @param void
 * @return TokenType for matching keyword or ERROR
 *
 * Handles:
 * - Case-insensitive matching
 * - Leading/trailing whitespace
 * - Special REM keyword behavior
 */
Tokenizer::TokenType Tokenizer::token_keyword() {
    std::string keyword;
    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t'))
        io_->next();
    while (!io_->eof() &&
           std::isalpha(static_cast<unsigned char>(io_->current()))) {
        keyword += static_cast<char>(
          std::toupper(static_cast<unsigned char>(io_->current())));
        io_->next();
    }

    if (!io_->eof() && keyword == "PRINT" && io_->current() == '$') {
        io_->next(); // consume '$'
        while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t'))
            io_->next();
        return TokenType::PRINT_DOLLAR;
    }

    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t'))
        io_->next();

    if (keyword == "REM")
        return TokenType::REM;
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
    if (keyword == "TAB")
        return TokenType::TAB;

    return TokenType::ERROR;
}

/**
 * token_number
 *
 * Processes a numeric literal token
 *
 * @param void
 * @return TokenType::NUMBER or ERROR
 *
 * Handles:
 * - Leading/trailing whitespace
 * - Max number length (SUBARUU_NUMBER_LITERAL)
 * - Invalid number formats
 * Updates token_data with parsed value
 */
Tokenizer::TokenType Tokenizer::token_number() {
    std::string num_str;
    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t'))
        io_->next();
    while (!io_->eof() &&
           std::isdigit(static_cast<unsigned char>(io_->current()))) {
        if (num_str.size() >= SUBARUU_NUMBER_LITERAL) {
            return TokenType::ERROR;
        }
        num_str += io_->current();
        io_->next();
    }
    if (num_str.empty())
        return TokenType::ERROR;
    using boost::multiprecision::cpp_int;
    cpp_int value = 0;
    const cpp_int ten = 10;
    for (char ch : num_str) {
        value *= ten;
        const cpp_int d = static_cast<unsigned>(ch - '0');
        value += d;
    }

    token_data_ = value;
    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t'))
        io_->next();

    return TokenType::NUMBER;
}
