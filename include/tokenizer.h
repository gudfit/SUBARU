#pragma once

#include "config.h"
#include "io.h"

#include <memory>
#include <string>
#include <variant>
#include <vector>

class Tokenizer {
    public:
        // Constructor/Destructor
        explicit Tokenizer(const std::string& source);
        ~Tokenizer();

        enum class TokenType {
            ERROR = 1,
            EOF_TOKEN,
            NUMBER,
            LETTER,
            STRING,
            EQUAL,
            LT,
            GT,
            LT_EQ,
            GT_EQ,
            NOT_EQUAL,
            SEPARATOR,
            MINUS,
            PLUS,
            ASTERISK,
            SLASH,
            LET,
            IF,
            THEN,
            PRINT,
            REM,
            GOTO,
            LEFT_PAREN,
            RIGHT_PAREN,
            EOL
        };

        struct KeywordToken {
                std::string keyword;
                TokenType token;
        };

        using TokenData = std::variant<std::monostate, std::string, int, char>;

        // Token operations
        TokenType current_token() const { return current_token_; }
        void reset();
        void reset(TokenType to);
        bool finished() const;
        void next_token();

        // Line detection
        bool is_line_number();
        char peek_char();
        void skip_char();
        void skip_to_eol();

        // Token data access
        std::string token_to_string(TokenType token) const;
        const TokenData& get_token_data() const;
        int variable_num() const;
        const std::string& get_string() const;
        int get_num() const;

    private:
        // Token parsing methods
        TokenType get_next_token();
        TokenType token_relation();
        TokenType token_operation();
        TokenType token_string();
        TokenType token_keyword();
        TokenType token_number();
        TokenType token_eol(int c);

        // Member variables
        std::unique_ptr<IO> io_;
        TokenType current_token_;
        TokenData token_data_;
        std::vector<KeywordToken> keywords_;
};
