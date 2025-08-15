// config.h
#pragma once
#include <cstddef>

// SUBARUU file extension.
constexpr char SUBARUU_EXTENSION_LITERAL[] = "subaru";

// The maximum length of string literals / number literals.
constexpr std::size_t SUBARUU_STRING_LITERAL = 50;
constexpr std::size_t SUBARUU_NUMBER_LITERAL = 19;

// Interpreter constants
constexpr std::size_t SUBARUU_MAX_VARIABLES = 26;
constexpr long long SUBARUU_DIVIDE_BY_ZERO_RESULT = 0;
constexpr bool SUBARUU_TERMINATE_ON_DIV_ZERO = false;
