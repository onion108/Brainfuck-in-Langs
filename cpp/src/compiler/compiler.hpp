#pragma once

#include "../vm/vm.hpp"
#include <exception>
#include <string>

namespace brainfuck {
    program compile_string(const std::string& code);

    class unmatched_bracket : public std::exception {
        const char * what() const noexcept override {
            return "Unmatched bracket found! ";
        }
    };

}

