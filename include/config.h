// config.h

#pragma once

#include <cstddef>

// SUBARUU file extension.
constexpr char SUBARUU_EXTENSION_LITERAL[] = "subaru";

// Max length of string/number literals.
constexpr std::size_t SUBARUU_STRING_LITERAL = 50;
constexpr std::size_t SUBARUU_NUMBER_LITERAL = 10000;

// Interpreter constants
constexpr std::size_t SUBARUU_MAX_VARIABLES = 26;
constexpr bool SUBARUU_TERMINATE_ON_DIV_ZERO = false;
