// io.h

#pragma once
#include <fstream>
#include <string>
#include <string_view>

class IO {
    public:
        using iterator = std::string::iterator;
        using const_iterator = std::string::const_iterator;

        explicit IO(std::string_view filename); // Can throw
        ~IO() noexcept;

        // Iterators
        [[nodiscard]] iterator begin() noexcept { return content_.begin(); }
        [[nodiscard]] iterator end() noexcept { return content_.end(); }
        [[nodiscard]] const_iterator begin() const noexcept {
            return content_.begin();
        }
        [[nodiscard]] const_iterator end() const noexcept {
            return content_.end();
        }

        // Position
        [[nodiscard]] iterator position() noexcept { return current_pos_; }
        [[nodiscard]] const_iterator position() const noexcept {
            return current_pos_;
        }

        // Char access
        [[nodiscard]] char current() const noexcept {
            return current_pos_ != content_.end() ? *current_pos_ : '\0';
        }
        iterator next() noexcept {
            if (current_pos_ != content_.end()) {
                ++current_pos_;
            }
            return current_pos_;
        }
        [[nodiscard]] bool eof() const noexcept {
            return current_pos_ == content_.end();
        }

        // Lookahead 1 character (or '\0')
        [[nodiscard]] char peek() const noexcept {
            auto it = current_pos_;
            if (it != content_.end()) {
                ++it;
                if (it != content_.end())
                    return *it;
            }
            return '\0';
        }

        // File ops
        void reset() noexcept;
        void seek(long offset,
                  std::ios_base::seekdir whence = std::ios::beg); // Can throw
        [[nodiscard]] std::string to_string(std::size_t n);       // Can throw
        void set(std::string_view filename);                      // Can throw
        void close() noexcept;

        // State
        [[nodiscard]] bool has_next_token() const noexcept { return !eof(); }
        [[nodiscard]] bool is_at_keyword() const noexcept {
            const auto c = current();
            return c >= 'A' && c <= 'Z';
        }

        // File info
        [[nodiscard]] std::string_view file() const noexcept {
            return filename_;
        }

    private:
        void load_file();
        std::string filename_;
        std::string content_;
        iterator current_pos_;

        IO(const IO&) = delete;
        IO& operator=(const IO&) = delete;
};
