// subaruu.cc

#include "../include/subaruu.h"
#include "../include/common.h"
#include "../include/tokenizer.h"

#include <cctype>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <variant>

/**
 * Constructs a new SUBARUU object and initialize with the given source file.
 *
 * @param source The source code file.
 * @throws std::runtime_error if tokenizer initialization fails
 */
SUBARUU::SUBARUU(std::string_view source)
  : tokenizer_(std::make_unique<Tokenizer>(source))
  , execution_finished_(false) {
    if (!tokenizer_)
        throw std::runtime_error("Failed to initialize Tokenizer");
    variables_.fill(value_t(0));
}

/**
 * Runs the SUBARUU interpreter.
 */
void SUBARUU::run() {
    build_line_map();
    while (!finished()) {
        if (tokenizer_->finished()) {
            execution_finished_ = true;
            break;
        }
        line_statement();
    }
}

/**
 * Gets the string representation of a token.
 *
 * @param token The token type to convert to string
 * @return std::string The string representation of the token
 */
std::string SUBARUU::get_token_string(Tokenizer::TokenType token) const {
    return std::string(tokenizer_->token_to_string(token));
}

/**
 * Checks if the interpreter has finished execution.
 *
 * @return true if execution has finished
 * @return false if execution is still ongoing
 */
bool SUBARUU::finished() const { return execution_finished_; }

/**
 * Debug print function with error handling.
 * Prints message to stderr and throws for errors but not warnings.
 *
 * @param message The message to print
 * @param errorCode E_ERROR or E_WARNING
 * @throws std::runtime_error if errorCode is E_ERROR
 */
void SUBARUU::dprintf(const std::string& message, int errorCode) {
    if (errorCode == E_ERROR) {
        std::cerr << "ERROR: " << message << std::endl;
        throw std::runtime_error(message);
    } else {
        std::cerr << "WARNING: " << message << std::endl;
    }
}

/**
 * Accepts the expected token or reports an error.
 *
 * @param expectedToken The token type that should be next in the stream
 * @throws std::runtime_error if the current token doesn't match expected
 */
void SUBARUU::accept(Tokenizer::TokenType expectedToken) {
    if (tokenizer_->current_token() != expectedToken) {
        dprintf("*subaruu.cpp: unexpected `" +
                  get_token_string(tokenizer_->current_token()) +
                  "` expected `" + get_token_string(expectedToken) + "`",
                E_ERROR);
    }
    tokenizer_->next_token();
}

/**
 * Performs safe division with error handling.
 *
 * @param numerator The division numerator
 * @param denominator The division denominator
 * @return int The division result or 0 for division by zero
 */
SUBARUU::value_t SUBARUU::safe_divide(value_t numerator, value_t denominator) {
    if (denominator == 0) {
        dprintf("*warning: divide by zero", E_WARNING);
        if (SUBARUU_TERMINATE_ON_DIV_ZERO)
            dprintf("Division by zero", E_ERROR);
        return 0;
    }
    try {
        return numerator / denominator;
    } catch (const std::overflow_error&) {
        dprintf("*warning: divide by zero", E_WARNING);
        if (SUBARUU_TERMINATE_ON_DIV_ZERO)
            dprintf("Division by zero", E_ERROR);
        return 0;
    }
}

/**
 * Parses and evaluates a factor in the expression.
 * A factor can be:
 * - A number
 * - A variable
 * - A parenthesized expression
 *
 * @return int The evaluated value of the factor
 * @throws std::runtime_error on syntax errors
 */
SUBARUU::value_t SUBARUU::factor() {
    value_t result = 0;
    const auto token = tokenizer_->current_token();
    switch (token) {
        case Tokenizer::TokenType::NUMBER:
            result = tokenizer_->get_num();
            tokenizer_->next_token();
            break;
        case Tokenizer::TokenType::LETTER: {
            char var_name =
              static_cast<char>(std::tolower(static_cast<unsigned char>(
                std::get<char>(tokenizer_->get_token_data()))));
            tokenizer_->next_token();
            if (tokenizer_->current_token() ==
                Tokenizer::TokenType::LEFT_BRACKET) {
                tokenizer_->next_token(); // skip '['
                value_t idx = expression();
                accept(Tokenizer::TokenType::RIGHT_BRACKET);
                auto it = memory_.find(idx);
                result = (it != memory_.end()) ? it->second : 0;
            } else {
                result = variables_[var_name - 'a'];
            }
            break;
        }
        case Tokenizer::TokenType::LEFT_PAREN:
            tokenizer_->next_token();
            result = expression();
            accept(Tokenizer::TokenType::RIGHT_PAREN);
            break;
        case Tokenizer::TokenType::MINUS: {
            tokenizer_->next_token();
            result = -factor();
            break;
        }
        default:
            dprintf("Syntax Error: Unexpected token in factor: " +
                      std::string(tokenizer_->token_to_string(token)),
                    E_ERROR);
    }
    return result;
}

/**
 * Parses and evaluates a term in the expression.
 * A term consists of factors connected by * or / operators.
 *
 * @return int The evaluated term value
 */
SUBARUU::value_t SUBARUU::term() {
    value_t result = factor();
    auto token = tokenizer_->current_token();
    while (token == Tokenizer::TokenType::ASTERISK ||
           token == Tokenizer::TokenType::SLASH) {
        auto op = token;
        tokenizer_->next_token();
        value_t rhs = factor();
        if (op == Tokenizer::TokenType::ASTERISK)
            result *= rhs;
        else
            result = safe_divide(result, rhs);
        token = tokenizer_->current_token();
    }
    return result;
}

/**
 * Parses and evaluates a complete expression.
 * An expression consists of terms connected by + or - operators.
 * Also handles special case of line numbers.
 *
 * @return int The evaluated expression value
 */
SUBARUU::value_t SUBARUU::expression() {
    value_t result = term();
    auto token = tokenizer_->current_token();
    // Special stop if the next token is a valid line-number; leave it for the
    // caller.
    if (token == Tokenizer::TokenType::NUMBER &&
        is_valid_line_number(tokenizer_->get_num())) {
        return result;
    }
    while (token == Tokenizer::TokenType::PLUS ||
           token == Tokenizer::TokenType::MINUS) {
        auto op = token;
        tokenizer_->next_token();
        value_t rhs = term();
        if (op == Tokenizer::TokenType::PLUS)
            result += rhs;
        else
            result -= rhs;
        token = tokenizer_->current_token();
    }
    return result;
}

/**
 * Parses and evaluates a relation (comparison expression).
 * Returns 1 for true conditions, 0 for false.
 * Non-zero values in simple expressions are treated as true.
 *
 * @return int 1 for true, 0 for false
 * @throws std::runtime_error on invalid comparison operator
 */
int SUBARUU::relation() {
    value_t left = expression();
    auto token = tokenizer_->current_token();
    switch (token) {
        case Tokenizer::TokenType::EQUAL:
            tokenizer_->next_token();
            return left == expression();
        case Tokenizer::TokenType::LT:
            tokenizer_->next_token();
            return left < expression();
        case Tokenizer::TokenType::GT:
            tokenizer_->next_token();
            return left > expression();
        case Tokenizer::TokenType::LT_EQ:
            tokenizer_->next_token();
            return left <= expression();
        case Tokenizer::TokenType::GT_EQ:
            tokenizer_->next_token();
            return left >= expression();
        case Tokenizer::TokenType::NOT_EQUAL:
            tokenizer_->next_token();
            return left != expression();
        default:
            return left != 0;
    }
}

/**
 * Executes a LET statement.
 * Format: LET variable = expression
 *
 * @throws std::runtime_error on syntax errors
 */
void SUBARUU::let_statement() {
    if (tokenizer_->current_token() != Tokenizer::TokenType::LETTER) {
        dprintf("Syntax Error: Expected variable name", E_ERROR);
        return;
    }
    char var_name = static_cast<char>(std::tolower(static_cast<unsigned char>(
      std::get<char>(tokenizer_->get_token_data()))));
    tokenizer_->next_token();
    bool indexed = false;
    value_t idx = 0;
    if (tokenizer_->current_token() == Tokenizer::TokenType::LEFT_BRACKET) {
        indexed = true;
        tokenizer_->next_token();
        idx = expression();
        accept(Tokenizer::TokenType::RIGHT_BRACKET);
    }
    accept(Tokenizer::TokenType::EQUAL);
    value_t value = expression();
    if (indexed)
        memory_[idx] = value;
    else
        variables_[var_name - 'a'] = value;
}

/**
 * Executes an IF statement.
 * Format: IF condition THEN line_number
 *
 * @throws std::runtime_error on syntax errors
 */
void SUBARUU::if_statement() {
    accept(Tokenizer::TokenType::IF);
    int cond = relation();
    accept(Tokenizer::TokenType::THEN);
    if (tokenizer_->current_token() != Tokenizer::TokenType::NUMBER) {
        dprintf("Syntax Error: Expected line number after THEN", E_ERROR);
        return;
    }
    int line_number = static_cast<int>(tokenizer_->get_num());
    tokenizer_->next_token();
    if (cond) {
        if (line_positions_.find(line_number) == line_positions_.end()) {
            dprintf("Runtime Error: Line number " +
                      std::to_string(line_number) + " not found",
                    E_ERROR);
            return;
        }
        tokenizer_->reset();
        if (!find_target_line(line_number)) {
            dprintf("Internal Error: Failed to find valid line number " +
                      std::to_string(line_number),
                    E_ERROR);
        }
    } else {
        if (tokenizer_->current_token() == Tokenizer::TokenType::EOL)
            tokenizer_->next_token();
    }
}

/**
 * Executes a GOTO statement.
 * Format: GOTO line_number
 */
void SUBARUU::goto_statement() {
    accept(Tokenizer::TokenType::GOTO);
    int line_number = static_cast<int>(tokenizer_->get_num());
    accept(Tokenizer::TokenType::NUMBER);
    if (tokenizer_->current_token() == Tokenizer::TokenType::EOL)
        tokenizer_->next_token();
    if (line_positions_.find(line_number) == line_positions_.end()) {
        dprintf("Runtime Error: Line number " + std::to_string(line_number) +
                  " not found",
                E_ERROR);
        return;
    }
    tokenizer_->reset();
    if (!find_target_line(line_number)) {
        dprintf("Internal Error: Failed to find valid line number " +
                  std::to_string(line_number),
                E_ERROR);
    }
}

/**
 * Skips through the code until finding a specific line number.
 * Assumes tokenizer has been reset to beginning.
 *
 * @param line_number The target line number to find
 * @return bool True if line was found, false if reached end without finding
 */
bool SUBARUU::find_target_line(int line_number) {
    bool at_line_start = true;
    while (!tokenizer_->finished()) {
        auto tok = tokenizer_->current_token();
        if (at_line_start && tok == Tokenizer::TokenType::NUMBER &&
            static_cast<int>(tokenizer_->get_num()) == line_number) {
            tokenizer_->next_token();
            return true;
        }
        at_line_start = (tok == Tokenizer::TokenType::EOL);
        tokenizer_->next_token();
    }
    return false;
}

/**
 * Executes a PRINT statement.
 * Format: PRINT [expression|string|separator]...
 */
void SUBARUU::print_statement() {
    accept(Tokenizer::TokenType::PRINT);
    bool need_space = false;
    while (!tokenizer_->finished()) {
        auto token = tokenizer_->current_token();
        if (is_statement_end(token) || is_line_number())
            break;
        switch (token) {
            case Tokenizer::TokenType::STRING:
                if (need_space)
                    std::cout << " ";
                std::cout << tokenizer_->get_string();
                need_space = true;
                tokenizer_->next_token();
                break;
            case Tokenizer::TokenType::SEPARATOR:
                need_space = false;
                std::cout << " ";
                tokenizer_->next_token();
                break;
            case Tokenizer::TokenType::LETTER:
            case Tokenizer::TokenType::NUMBER:
            case Tokenizer::TokenType::LEFT_PAREN:
            case Tokenizer::TokenType::MINUS:
                if (need_space)
                    std::cout << " ";
                std::cout << expression();
                need_space = true;
                break;
            default:
                goto end_print;
        }
    }
end_print:
    std::cout << std::endl;
    auto final_token = tokenizer_->current_token();
    if (is_line_number())
        return;
    if (final_token == Tokenizer::TokenType::EOF_TOKEN)
        execution_finished_ = true;
    else if (final_token == Tokenizer::TokenType::EOL)
        tokenizer_->next_token();
}

/**
 * Checks if the current token indicates end of statement
 */
bool SUBARUU::is_statement_end(Tokenizer::TokenType token) const {
    return token == Tokenizer::TokenType::EOL ||
           token == Tokenizer::TokenType::EOF_TOKEN;
}

/**
 * Checks if current token is a valid line number
 */
bool SUBARUU::is_line_number() const {
    if (tokenizer_->current_token() != Tokenizer::TokenType::NUMBER)
        return false;
    return is_valid_line_number(tokenizer_->get_num());
}

/**
 * Helper to determine if a number could be a valid line number.
 * Line numbers must be >= 10 and multiples of 10, followed by whitespace.
 *
 * @param num The number to check
 * @return bool True if this appears to be a line number
 */
bool SUBARUU::is_valid_line_number(const value_t& num) const {
    return num >= 10 && (num % 10 == 0);
}

/**
 * Executes a statement based on the current token.
 * Handles REM, PRINT, IF, GOTO, and LET statements.
 *
 * @throws std::runtime_error on syntax errors
 */
void SUBARUU::statement() {
    auto token = tokenizer_->current_token();
    switch (token) {
        case Tokenizer::TokenType::REM:
            tokenizer_->skip_to_eol();
            break;
        case Tokenizer::TokenType::PRINT:
            print_statement();
            break;
        case Tokenizer::TokenType::IF:
            if_statement();
            break;
        case Tokenizer::TokenType::GOTO:
            goto_statement();
            break;
        case Tokenizer::TokenType::LET:
            accept(Tokenizer::TokenType::LET);
            [[fallthrough]];
        case Tokenizer::TokenType::LETTER:
            let_statement();
            break;
        default:
            dprintf("Syntax Error: Unrecognized statement", E_ERROR);
            break;
    }
}

/**
 * Parses and executes a line statement.
 * Handles line numbers and statement execution.
 */
void SUBARUU::line_statement() {
    while (tokenizer_->current_token() == Tokenizer::TokenType::EOL)
        tokenizer_->next_token();
    if (tokenizer_->current_token() == Tokenizer::TokenType::EOF_TOKEN) {
        execution_finished_ = true;
        return;
    }
    if (tokenizer_->current_token() == Tokenizer::TokenType::NUMBER)
        tokenizer_->next_token();
    statement();
}

/**
 * Builds a map of line numbers in the program.
 * Maps each line number to its position in the source.
 */
void SUBARUU::build_line_map() {
    line_positions_.clear();
    tokenizer_->reset();
#ifdef DEBUG_MODE
    std::unordered_map<int, bool> found_lines;
#endif
    bool at_line_start = true;
    while (!tokenizer_->finished()) {
        auto tok = tokenizer_->current_token();
        if (at_line_start && tok == Tokenizer::TokenType::NUMBER) {
            value_t num_val = tokenizer_->get_num();
            if (is_valid_line_number(num_val)) {
                int value = static_cast<int>(num_val);
                line_positions_[value] = true;
#ifdef DEBUG_MODE
                found_lines[value] = true;
#endif
            }
        }
        at_line_start = (tok == Tokenizer::TokenType::EOL);
        tokenizer_->next_token();
    }
#ifdef DEBUG_MODE
    log_found_line_numbers(found_lines);
#endif
    tokenizer_->reset();
}

#ifdef DEBUG_MODE
/**
 * Helper to log all found line numbers during map building
 */
void SUBARUU::log_found_line_numbers(
  const std::unordered_map<int, bool>& found_lines) {
    std::string line_numbers;
    for (const auto& [line, _] : found_lines) {
        if (!line_numbers.empty())
            line_numbers += " ";
        line_numbers += std::to_string(line);
    }
    DEBUG_LOG("Found these line numbers: " << line_numbers);
}

void SUBARUU::log_available_lines(int target_line) {
    DEBUG_LOG("Line " << target_line << " not found in map. Available lines:");
    for (const auto& [line, _] : line_positions_)
        DEBUG_LOG(" " << line);
}
#endif
