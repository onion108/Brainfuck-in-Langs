#pragma once

#include <cstddef>
#include <sstream>
#include <string>

namespace utils {

    template <typename T>
    concept string_convertible = requires(T t) {
        t.to_string();
    };

    template <typename T>
    std::string to_string(T value) {
        return "<UNKNOWN TYPE>";
    }

    template <string_convertible T>
    std::string to_string(T value) {
        return value.to_string();
    }

#define REGISTER_FOR(type) template <> inline std::string to_string<type>(type value) { \
    std::stringstream ss; \
    ss << value; \
    return ss.str(); \
}

    REGISTER_FOR(int);

    REGISTER_FOR(short);

    REGISTER_FOR(long long);

    REGISTER_FOR(unsigned int);

    REGISTER_FOR(unsigned short);

    REGISTER_FOR(unsigned long long);

    REGISTER_FOR(long);

    REGISTER_FOR(unsigned long);

    REGISTER_FOR(unsigned char);

    REGISTER_FOR(void*);

#undef REGISTER_FOR

    template <>
    inline std::string to_string<bool>(bool value) {
        return value ? "true" : "false";
    }

    template <>
    inline std::string to_string<const char *>(const char *value) {
        return value;
    }

    template <>
    inline std::string to_string<char>(char c) {
        return std::string {c};
    }

    template <>
    inline std::string to_string<std::string>(std::string s) {
        return s;
    }

    template <typename ...Args>
    inline std::string concat(Args ...args) {
        std::stringstream ss;
        (ss << ... << to_string(args)) << "";
        return ss.str();
    }

};

