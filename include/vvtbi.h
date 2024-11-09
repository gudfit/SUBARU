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
        ~VVTBI();

        // Delete copy operations
        VVTBI(const VVTBI&) = delete;
        VVTBI& operator=(const VVTBI&) = delete;

        void run();
        [[nodiscard]] std::string_view get_token_string(
          Tokenizer::TokenType token) const;
        [[nodiscard]] bool finished() const noexcept {
            return execution_finished_;
        }

    private:
        enum class ErrorCode { Error = 1, Warning };

        // Parser methods
        void accept(Tokenizer::TokenType expectedToken);
        [[nodiscard]] int expression();
        [[nodiscard]] int term();
        [[nodiscard]] int factor();
        [[nodiscard]] int relation();

        // Statement handlers
        void statement();
        void line_statement();
        void let_statement();
        void if_statement();
        void goto_statement();
        void print_statement();

        // Line number and control flow
        void find_linenum(int linenum);
        void jump_linenum(int linenum);
        void build_line_map();

        // Utilities
        void dprintf(std::string_view message, ErrorCode errorCode);
        [[nodiscard]] int safe_divide(int numerator, int denominator) noexcept;

        // State
        std::unique_ptr<Tokenizer> tokenizer_;
        std::unordered_map<char, int> variables_;
        std::unordered_map<int, std::streampos> line_positions_;
        bool execution_finished_{ false };
        bool skip_to_line_{ false };
        int target_line_{ 0 };
};
