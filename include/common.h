#pragma once

#include <iostream>

#ifndef DEBUG_MODE
#define DEBUG_LOG(x) std::cerr << "DEBUG: " << x << std::endl
#else
#define DEBUG_LOG(x)
#endif
