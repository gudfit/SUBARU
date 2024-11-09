#include "../include/tokenizer.h"
#include "../include/common.h"

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
    // Initialize keywords with their corresponding token types
    keywords_ = { { "let", TokenType::LET },   { "if", TokenType::IF },
                  { "then", TokenType::THEN }, { "print", TokenType::PRINT },
                  { "rem", TokenType::REM },   { "goto", TokenType::GOTO } };

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
    if (io_) {
        io_->close();
    }
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
char Tokenizer::peek_char() { return io_->peek(); }

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
        char letter = std::get<char>(token_data_);
        if (letter >= 'a' && letter <= 'z') {
            return letter - 'a';
        }
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
    if (std::holds_alternative<std::string>(token_data_)) {
        return std::get<std::string>(token_data_);
    }
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
int Tokenizer::get_num() const {
    if (std::holds_alternative<int>(token_data_)) {
        return std::get<int>(token_data_);
    }
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
    if (finished()) {
        return;
    }
    // Skip spaces and tabs, but not newlines
    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t')) {
        io_->next();
    }
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
    while (!io_->eof() && io_->current() != '\n' && io_->current() != '\r') {
        io_->next();
    }
    if (!io_->eof()) {
        if (io_->current() == '\r') {
            io_->next();
            if (!io_->eof() && io_->current() == '\n') {
                io_->next();
            }
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
    if (io_->eof()) {
        return TokenType::EOF_TOKEN;
    }
    char c = io_->current();
    DEBUG_LOG("get_next_token processing char: '"
              << (std::isprint(c) ? c : ' ')
              << "' (ASCII: " << static_cast<int>(c) << ")");
    // Skip spaces and tabs, but not newlines
    while (!io_->eof() && (c == ' ' || c == '\t')) {
        io_->next();
        c = io_->current();
    }
    // Handle EOL
    if (c == '\n' || c == '\r') {
        if (c == '\r') {
            io_->next();
            if (!io_->eof() && io_->current() == '\n') {
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
            token_data_ = c;
            io_->next();
            return TokenType::LETTER;
        }
    }
    // Handle separators
    if (c == ',' || c == ';') {
        io_->next();
        DEBUG_LOG("Found separator");
        return TokenType::SEPARATOR;
    }
    // Handle operators and other tokens
    TokenType token = TokenType::ERROR;
    switch (c) {
        case '=':
            io_->next();
            token = TokenType::EQUAL;
            break;
        case '<':
            io_->next();
            if (!io_->eof()) {
                if (io_->current() == '=') {
                    io_->next();
                    token = TokenType::LT_EQ;
                } else if (io_->current() == '>') {
                    io_->next();
                    token = TokenType::NOT_EQUAL;
                } else {
                    token = TokenType::LT;
                }
            }
            break;
        case '>':
            io_->next();
            if (!io_->eof() && io_->current() == '=') {
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
 * - Max string length (VVTBI_STRING_LITERAL)
 * - Whitespace after string
 * - Unclosed strings preserved as-is
 */
Tokenizer::TokenType Tokenizer::token_string() {
    std::string str;
    io_->next(); // Skip initial quote
    while (!io_->eof()) {
        char c = io_->current();
        if (c == '"') {
            io_->next();
            token_data_ = str;
            while (!io_->eof() &&
                   (io_->current() == ' ' || io_->current() == '\t')) {
                io_->next();
            }
            return TokenType::STRING;
        }
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
    DEBUG_LOG("Processing possible keyword");
    std::string keyword;

    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t')) {
        io_->next();
    }
    while (!io_->eof() && std::isalpha(io_->current())) {
        keyword += static_cast<char>(std::toupper(io_->current()));
        io_->next();
    }
    DEBUG_LOG("Found possible keyword: " << keyword);
    std::string_view keyword_view(keyword);
    if (keyword_view == "REM") {
        return TokenType::REM;
    }
    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t')) {
        io_->next();
    }
    if (keyword_view == "PRINT")
        return TokenType::PRINT;
    if (keyword_view == "LET")
        return TokenType::LET;
    if (keyword_view == "IF")
        return TokenType::IF;
    if (keyword_view == "THEN")
        return TokenType::THEN;
    if (keyword_view == "GOTO")
        return TokenType::GOTO;
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
 * - Max number length (VVTBI_NUMBER_LITERAL)
 * - Invalid number formats
 * Updates token_data with parsed value
 */
Tokenizer::TokenType Tokenizer::token_number() {
    std::string num_str;
    while (!io_->eof() && (io_->current() == ' ' || io_->current() == '\t')) {
        io_->next();
    }
    while (!io_->eof() && std::isdigit(io_->current()) &&
           num_str.size() < VVTBI_NUMBER_LITERAL) {
        num_str += io_->current();
        io_->next();
    }
    if (!num_str.empty()) {
        try {
            token_data_ = std::stoi(num_str);

            while (!io_->eof() &&
                   (io_->current() == ' ' || io_->current() == '\t')) {
                io_->next();
            }
            return TokenType::NUMBER;
        } catch (const std::exception&) {
            return TokenType::ERROR;
        }
    }
    return TokenType::ERROR;
}
