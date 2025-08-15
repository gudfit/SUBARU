
#include <cstdlib> // For EXIT_SUCCESS, EXIT_FAILURE
#include <cstring> // For strcmp
#include <iostream>
#include <string>

#include "../include/subaruu.h"
#include "../include/tokenizer.h"

// SUBARU's version number.
constexpr const char* VERSION = "2.1";

// The message printed if no file is given.
static std::string usage() {
    return std::string("VERSION: ") + VERSION +
           "\n"
           "***************************************\n"
           "  Howto: ./subaru [-debug] file." +
           std::string(SUBARUU_EXTENSION_LITERAL) + "\n";
}

/**
 * @brief Check if the filename has the valid extension.
 *
 * @param filename File to check against.
 * @return true if filename ends with the expected extension.
 */
static bool valid(const std::string& filename) {
    const auto pos = filename.rfind('.');
    if (pos == std::string::npos)
        return false;
    return filename.substr(pos + 1) == SUBARUU_EXTENSION_LITERAL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << usage();
        return EXIT_SUCCESS;
    }

    if (std::strcmp(argv[1], "-debug") == 0) {
        if (argc < 3) {
            std::cout << usage();
            return EXIT_SUCCESS;
        }
        if (!valid(argv[2])) {
            std::cerr << "Invalid file extension. Expected a .subaru file.\n";
            return EXIT_FAILURE;
        }
        try {
            Tokenizer tokenizer(argv[2]);
            do {
                auto token = tokenizer.current_token();
                std::cout << tokenizer.token_to_string(token) << " ";
                if (token == Tokenizer::TokenType::EOL)
                    std::cout << "\n";
                tokenizer.next_token();
            } while (!tokenizer.finished());
        } catch (const std::exception& e) {
            std::cerr << "Tokenizer Error: " << e.what() << "\n";
            return EXIT_FAILURE;
        }
    } else {
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
    return EXIT_SUCCESS;
}
