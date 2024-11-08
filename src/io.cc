#include "../include/io.h"
#include <iostream>
#include <stdexcept>

/******************************************************************************/

/**
 * IO Constructor
 *
 * @param filename The path to the file to be opened for reading
 * @throws std::runtime_error If the file cannot be opened
 */
IO::IO(const std::string& filename)
  : filename_(filename)
  , file_stream_(filename, std::ios::binary)
  , current_char_('\0')
{
    if (!file_stream_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename_);
    }
    load_next();
}

/**
 * IO Destructor
 *
 * Ensures the file stream is properly closed when the object is destroyed
 */
IO::~IO() { close(); }

/**
 * load_next
 *
 * Internal helper function to load the next character from the file stream.
 * Sets current_char_ to '\0' if end of file is reached.
 *
 * @param void
 * @return void
 */
void IO::load_next()
{
    if (file_stream_.get(current_char_)) {
        // Successfully read character
        ;
    } else {
        current_char_ = '\0';
    }
}

/**
 * next
 *
 * Advances to the next character in the stream
 *
 * @param void
 * @return void
 */
void IO::next() { load_next(); }

/**
 * reset
 *
 * Resets the file stream to the beginning of the file and loads the first
 * character
 *
 * @param void
 * @return void
 */
void IO::reset()
{
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
void IO::close()
{
    if (file_stream_.is_open()) {
        file_stream_.close();
    }
}

/**
 * to_string
 *
 * Copies up to n characters from the current stream position into the
 * destination buffer. Adds null terminator at the end of the copied string.
 *
 * @param dest The destination buffer to copy characters into
 * @param n The maximum number of characters to copy
 * @return void
 */
void IO::to_string(char* dest, std::size_t n)
{
    std::size_t i = 0;
    while (n > 0 && !eof()) {
        dest[i++] = current_char_;
        next();
        --n;
    }
    dest[i] = '\0';
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
void IO::set(const std::string& filename)
{
    close();
    filename_ = filename;
    file_stream_.open(filename_, std::ios::binary);
    if (!file_stream_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename_);
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
void IO::seek(long offset, std::ios_base::seekdir whence)
{
    file_stream_.clear();
    file_stream_.seekg(offset, whence);
    load_next();
}
