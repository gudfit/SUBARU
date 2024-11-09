// config.h
#pragma once

#include <cstddef>

// Vvtbi file extension.
constexpr char VVTBI_EXTENSION_LITERAL[] = "vvtb";

// The maximum length of string literals.
constexpr std::size_t VVTBI_STRING_LITERAL = 50;
constexpr std::size_t VVTBI_NUMBER_LITERAL = 8;

// Interpreter constants
constexpr std::size_t VVTBI_MAX_VARIABLES = 26;
constexpr int VVTBI_DIVIDE_BY_ZERO_RESULT = 0;
constexpr bool VVTBI_TERMINATE_ON_DIV_ZERO = false;
