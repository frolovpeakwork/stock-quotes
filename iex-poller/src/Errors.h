#pragma once

#include <sstream>
#include <stdexcept>

#define ASSERT(cond, msg) \
    if (!(cond)) { \
        std::stringstream stream{}; \
        stream << "Assertion failed: " << msg; \
        throw std::runtime_error{stream.str()}; \
    }
