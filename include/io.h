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

        // Iterator operations
        [[nodiscard]] iterator begin() noexcept { return content_.begin(); }
        [[nodiscard]] iterator end() noexcept { return content_.end(); }
        [[nodiscard]] const_iterator begin() const noexcept {
            return content_.begin();
        }
        [[nodiscard]] const_iterator end() const noexcept {
            return content_.end();
        }

        // Get current position
        [[nodiscard]] iterator position() noexcept { return current_pos_; }
        [[nodiscard]] const_iterator position() const noexcept {
            return current_pos_;
        }

        // Basic operations
        char peek();
        [[nodiscard]] char current() const noexcept {
            return current_pos_ != content_.end() ? *current_pos_ : 0;
        }
        iterator next() noexcept {
            if (current_pos_ != content_.end()) {
                return ++current_pos_;
            }
            return current_pos_;
        }
        [[nodiscard]] bool eof() const noexcept {
            return current_pos_ == content_.end();
        }

        // Peek operations
        [[nodiscard]] char peek() const noexcept {
            auto next_pos = current_pos_;
            if (next_pos != content_.end() &&
                std::next(next_pos) != content_.end()) {
                return *std::next(next_pos);
            }
            return 0;
        }

        // File operations
        void reset() noexcept;
        void seek(long offset,
                  std::ios_base::seekdir whence = std::ios::beg); // Can throw
        [[nodiscard]] std::string to_string(std::size_t n);       // Can throw
        void set(std::string_view filename);                      // Can throw
        void close() noexcept;

        // State queries
        [[nodiscard]] bool has_next_token() const noexcept { return !eof(); }

        [[nodiscard]] bool is_at_keyword() const noexcept {
            return current() >= 'A' && current() <= 'Z';
        }

        // Access to file info
        [[nodiscard]] std::string_view file() const noexcept {
            return filename_;
        }

    private:
        // Loads entire file into content_
        void load_file();
        std::string filename_;
        std::string content_;
        iterator current_pos_;

        IO(const IO&) = delete;
        IO& operator=(const IO&) = delete;
};
