#pragma once
#include <fstream>
#include <string>
#include <string_view>

class IO {
    public:
        explicit IO(std::string_view filename); // Can throw
        ~IO() noexcept;

        // Basic operations
        [[nodiscard]] constexpr char current() const noexcept {
            return current_char_;
        }
        void next() noexcept;
        [[nodiscard]] constexpr bool eof() const noexcept {
            return current_char_ == '\0' || file_stream_.eof();
        }

        // Peek operations
        [[nodiscard]] char peek() noexcept {
            int next = file_stream_.peek();
            return next != EOF ? static_cast<char>(next) : '\0';
        }

        // File operations
        void reset() noexcept;
        void seek(long offset,
                  std::ios_base::seekdir whence = std::ios::beg); // Can throw
        [[nodiscard]] std::string to_string(std::size_t n);       // Can throw
        void set(std::string_view filename);                      // Can throw
        void close() noexcept;

        // State queries
        [[nodiscard]] constexpr bool has_next_token() const noexcept {
            return !eof() && current_char_ != '\0';
        }

        [[nodiscard]] constexpr bool is_at_keyword() const noexcept {
            return (current_char_ >= 'A' && current_char_ <= 'Z');
        }

        // Access to file info
        [[nodiscard]] constexpr std::string_view file() const noexcept {
            return filename_;
        }

    private:
        void load_next() noexcept;
        std::string filename_;
        std::ifstream file_stream_;
        char current_char_;

        IO(const IO&) = delete;
        IO& operator=(const IO&) = delete;
};
