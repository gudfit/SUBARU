#include "../include/io.h"
#include <stdexcept>

/**
 * IO Constructor
 *
 * @param filename The path to the file to be opened for reading
 * @throws std::runtime_error If the file cannot be opened
 */
IO::IO(std::string_view filename)
  : filename_(filename)
  , file_stream_(filename_, std::ios::binary)
  , current_char_('\0') {
    if (!file_stream_.is_open()) {
        throw std::runtime_error("Failed to open file: " +
                                 std::string(filename));
    }
    load_next();
}

/**
 * IO Destructor
 *
 * @param void
 * @return void
 * Ensures the file stream is properly closed when the object is destroyed
 */
IO::~IO() noexcept { close(); }

/**
 * load_next
 *
 * Internal helper function to load the next character from the file stream.
 * Sets current_char_ to '\0' if end of file is reached.
 *
 * @param void
 * @return void
 */
void IO::load_next() noexcept {
    if (file_stream_.get(current_char_)) {
        return;
    }
    current_char_ = '\0';
}

/**
 * next
 *
 * Advances to the next character in the stream
 *
 * @param void
 * @return void
 */
void IO::next() noexcept { load_next(); }

/**
 * reset
 *
 * Resets the file stream to the beginning of the file and loads the first
 * character
 *
 * @param void
 * @return void
 */
void IO::reset() noexcept {
    file_stream_.clear();
    file_stream_.seekg(0, std::ios::beg);
    load_next();
}

/**
 * close
 *
 * Closes the file stream if it is currently open
 *
 * @param void
 * @return void
 */
void IO::close() noexcept {
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

/**
 * to_string
 *
 * Reads up to n characters from the current stream position into a string.
 * Reading stops if either n characters have been read or end of file is
 * reached.
 *
 * @param n The maximum number of characters to read
 * @return std::string containing the read characters
 */
std::string IO::to_string(std::size_t n) {
    std::string result;
    result.reserve(n);
    while (n > 0 && !eof()) {
        result += current_char_;
        next();
        --n;
    }
    return result;
}

/**
 * set
 *
 * Closes the current file (if any) and opens a new file for reading
 *
 * @param filename The path to the new file to open
 * @throws std::runtime_error If the new file cannot be opened
 * @return void
 */
void IO::set(std::string_view filename) {
    close();
    filename_ = filename;
    file_stream_.open(filename_, std::ios::binary);
    if (!file_stream_.is_open()) {
        throw std::runtime_error("Failed to open file: " +
                                 std::string(filename));
    }
    load_next();
}

/**
 * seek
 *
 * Repositions the file stream pointer to the specified offset
 *
 * @param offset The number of bytes to offset from whence
 * @param whence The reference position for the offset (beginning, current, or
 * end)
 * @return void
 */
void IO::seek(long offset, std::ios_base::seekdir whence) {
    file_stream_.clear();
    file_stream_.seekg(offset, whence);
    load_next();
}
