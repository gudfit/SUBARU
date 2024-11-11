
#include <cstdlib> // For EXIT_SUCCESS, EXIT_FAILURE
#include <cstring> // For strcmp
#include <iostream>
#include <string>

#include "../include/subaruu.h"
#include "../include/tokenizer.h"

// SUBARU's version number.
constexpr const char* VERSION = "2.0";

// The message printed if no file is given.
const std::string NOARGS = "VERSION: " + std::string(VERSION) +
                           "\n"
                           "***************************************\n"
                           "  Howto: ./subaru [-debug] file." +
                           std::string("subaru") + "\n";

/**
 * @brief Check if the filename has the valid extension.
 *
 * @param filename File to check against.
 * @return true if filename ends with the expected extension.
 */
bool valid(const std::string& filename) {
    size_t pos = filename.rfind('.');
    if (pos == std::string::npos)
        return false;
    std::string ext = filename.substr(pos + 1);
    return ext == "subaru";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        // No arguments provided; print usage message.
        std::cout << NOARGS;
    }
    // Debug mode: run the tokenizer and print tokens.
    else if (std::strcmp(argv[1], "-debug") == 0) {
        if (argc < 3) {
            std::cout << NOARGS;
        } else {
            // Check if the file has a valid extension.
            if (!valid(argv[2])) {
                std::cerr
                  << "Invalid file extension. Expected a .subaru file.\n";
                return EXIT_FAILURE;
            }

            try {
                Tokenizer tokenizer(argv[2]);
                // Run the tokenizer until EOF, printing each token.
                do {
                    Tokenizer::TokenType token = tokenizer.current_token();
                    std::cout << tokenizer.token_to_string(token) << " ";
                    if (token == Tokenizer::TokenType::EOL)
                        std::cout << "\n";
                    tokenizer.next_token();
                } while (!tokenizer.finished());
            } catch (const std::exception& e) {
                std::cerr << "Tokenizer Error: " << e.what() << "\n";
                return EXIT_FAILURE;
            }
        }
    }
    // Run the SUBARU interpreter.
    else {
        // Check if the file has a valid extension.
        if (!valid(argv[1])) {
            std::cerr << "Invalid file extension. Expected a .subaru file.\n";
            return EXIT_FAILURE;
        }

        try {
            SUBARUU subaruu(argv[1]);
            subaruu.run();
        } catch (const std::exception& e) {
            std::cerr << "SUBARUU Error: " << e.what() << "\n";
            return EXIT_FAILURE;
        }
    }
    // Program completed successfully.
    return EXIT_SUCCESS;
}
