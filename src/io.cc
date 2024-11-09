#include "../include/io.h"
#include <sstream>
#include <stdexcept>

/******************************************************************************/

/**
 * IO Constructor
 *
 * @param filename The path to the file to be opened and loaded into memory
 * @throws std::runtime_error If the file cannot be opened
 */
IO::IO(std::string_view filename)
  : filename_(filename) {
    load_file();
}

/**
 * IO Destructor
 *
 * Ensures resources are properly cleaned up when the object is destroyed
 */
IO::~IO() noexcept { close(); }

/**
 * load_file
 *
 * @param void
 * @return void
 * Reads the entire file into memory and initializes the iterator position
 * to the beginning of the content.
 *
 * @throws std::runtime_error If the file cannot be opened
 */
void IO::load_file() {
    std::ifstream file_stream(filename_, std::ios::binary);
    if (!file_stream.is_open()) {
        throw std::runtime_error("Failed to open file: " +
                                 std::string(filename_));
    }

    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    content_ = buffer.str();
    current_pos_ = content_.begin();
}
/**
 * reset
 *
 * @param void
 * @return void
 * Resets the iterator position to the beginning of the content
 */
void IO::reset() noexcept { current_pos_ = content_.begin(); }

/**
 * close
 *
 * @param void
 * @return void
 * Clears the content and resets the iterator position
 */
void IO::close() noexcept {
    content_.clear();
    current_pos_ = content_.end();
}

/**
 * to_string
 *
 * Returns a string containing up to n characters from the current position,
 * advancing the iterator to after the last character read
 *
 * @param n The maximum number of characters to read
 * @return std::string containing the read characters
 */
std::string IO::to_string(std::size_t n) {
    if (eof())
        return {};

    auto end_pos = current_pos_;
    std::size_t chars_left = std::distance(current_pos_, content_.end());
    std::size_t chars_to_read = std::min(n, chars_left);

    std::advance(end_pos, chars_to_read);
    std::string result(current_pos_, end_pos);
    current_pos_ = end_pos;

    return result;
}

/**
 * set
 *
 * Clears current content and loads a new file into memory
 *
 * @param filename The path to the new file to load
 * @throws std::runtime_error If the new file cannot be opened
 */
void IO::set(std::string_view filename) {
    close();
    filename_ = filename;
    load_file();
}

/**
 * seek
 *
 * Repositions the iterator to the specified offset from the given position
 *
 * @param offset The number of positions to move the iterator
 * @param whence The reference position (beginning, current, or end)
 *
 * Iterator bounds are automatically enforced - attempting to seek before
 * begin() will position at begin(), and seeking past end() will position
 * at end()
 */
void IO::seek(long offset, std::ios_base::seekdir whence) {
    switch (whence) {
        case std::ios::beg:
            current_pos_ = content_.begin();
            std::advance(current_pos_, offset);
            break;
        case std::ios::cur:
            std::advance(current_pos_, offset);
            break;
        case std::ios::end:
            current_pos_ = content_.end();
            std::advance(current_pos_, -offset);
            break;
        default:
            throw std::invalid_argument("Invalid seek direction");
    }

    // Bounds checking
    if (current_pos_ < content_.begin()) {
        current_pos_ = content_.begin();
    } else if (current_pos_ > content_.end()) {
        current_pos_ = content_.end();
    }
}

/**
 * peek
 *
 * Returns the current character in the input stream without advancing the
 * position. If at the end of the stream, returns '\0'.
 *
 * @param void
 * @return char
 */

char IO::peek() {
    if (current_pos_ != content_.end()) {
        return *current_pos_;
    } else {
        return '\0'; // Return null character at EOF
    }
}
