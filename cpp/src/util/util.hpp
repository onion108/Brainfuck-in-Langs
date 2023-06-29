#pragma once

#include "to_string.hpp"
#include <iostream>
#include <ostream>
namespace utils {

    template <typename ...Args>
    void sprint(std::ostream& os, Args ...args) {
        os << concat(args...);
    }

    template <typename ...Args>
    void sprintln(std::ostream& os, Args ...args) {
        sprint(os, args..., '\n');
    }

    template <typename ...Args>
    void print(Args ...args) {
        sprint(std::cout, args...);
    }

    template <typename ...Args>
    void println(Args ...args) {
        print(args..., '\n');
    }

    template <typename ...Args>
    void eprint(Args ...args) {
        sprint(std::cerr, args...);
    }

    template <typename ...Args>
    void eprintln(Args ...args) {
        sprintln(std::cerr, args...);
    }

}

