// subaruu.h

#pragma once

#include "config.h"
#include "tokenizer.h"

#include <array>
#include <boost/multiprecision/cpp_int.hpp>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

class SUBARUU {
    public:
        using value_t = boost::multiprecision::cpp_int;
        explicit SUBARUU(std::string_view source);
        ~SUBARUU() = default;
        void run();
        std::string get_token_string(Tokenizer::TokenType token) const;
        bool finished() const;
#ifdef DEBUG_MODE
        void log_found_line_numbers(
          const std::unordered_map<int, bool>& found_lines);
        void log_available_lines(int target_line);
#endif
    private:
        // Token processing
        void accept(Tokenizer::TokenType expectedToken);
        // Expression parsing
        value_t expression();
        value_t term();
        value_t factor();
        int relation();
        // Statements
        void statement();
        void line_statement();
        void let_statement();
        void if_statement();
        void goto_statement();
        void print_statement();
        // Line helpers
        void build_line_map();
        bool find_target_line(int line_number);
        // Aids
        bool is_valid_line_number(const value_t& num) const;
        bool is_line_number() const;
        bool is_statement_end(Tokenizer::TokenType token) const;
        // Errors
        enum ErrorCode { E_ERROR = 1, E_WARNING };
        void dprintf(const std::string& message, int errorCode);
        value_t safe_divide(value_t numerator, value_t denominator);
        // State
        std::unique_ptr<Tokenizer> tokenizer_;
        std::array<value_t, SUBARUU_MAX_VARIABLES> variables_;
        // indexed memory
        std::map<value_t, value_t> memory_;
        std::unordered_map<int, bool> line_positions_;
        bool execution_finished_;
};
