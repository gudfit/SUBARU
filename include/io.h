
#pragma once

#include <fstream>
#include <string>

class IO {
public:
  explicit IO(const std::string &filename);
  ~IO();

  // Basic operations
  char current() const { return current_char_; }
  void next();
  bool eof() const { return current_char_ == '\0' || file_stream_.eof(); }

  // Peek operations
  char peek() {
    int next = file_stream_.peek();
    return next != EOF ? static_cast<char>(next) : '\0';
  }

  // File operations
  void reset();
  void seek(long offset, std::ios_base::seekdir whence = std::ios::beg);
  void to_string(char *dest, std::size_t n);
  void set(const std::string &filename);
  void close();

  // State queries
  bool has_next_token() const { return !eof() && current_char_ != '\0'; }
  bool is_at_keyword() const {
    return std::isalpha(current_char_) && std::isupper(current_char_);
  }

  // Access to file info
  const std::string &file() const { return filename_; }

private:
  void load_next();

  std::string filename_;
  std::ifstream file_stream_;
  char current_char_;

  // Prevent copying
  IO(const IO &) = delete;
  IO &operator=(const IO &) = delete;
};
