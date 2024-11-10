#pragma once

#include "config.h"
#include "tokenizer.h"
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

class VVTBI {
    public:
        explicit VVTBI(std::string_view source);
        ~VVTBI() = default;

        void run();
        std::string get_token_string(Tokenizer::TokenType token) const;
        bool finished() const;

        void log_found_line_numbers(
          const std::unordered_map<int, bool>& found_lines);
        void log_available_lines(int target_line);

    private:
        // Token processing
        void accept(Tokenizer::TokenType expectedToken);

        // Expression parsing
        int expression();
        int term();
        int factor();
        int relation();

        // Statement handling
        void statement();
        void line_statement();
        void let_statement();
        void if_statement();
        void goto_statement();
        void print_statement();

        // Line number management
        void find_linenum(int linenum);
        void jump_linenum(int linenum);
        void build_line_map();

        // Aids
        bool is_valid_line_number(int num) const;
        bool is_line_number() const;
        bool is_statement_end(Tokenizer::TokenType token) const;

        // Error handling
        enum ErrorCode { E_ERROR = 1, E_WARNING };
        void dprintf(const std::string& message, int errorCode);
        int safe_divide(int numerator, int denominator);

        // Member variables
        std::unique_ptr<Tokenizer> tokenizer_;
        std::unordered_map<char, int> variables_;
        std::unordered_map<int, bool> line_positions_;
        bool execution_finished_;
        bool skip_to_line_;
        int target_line_;
};
