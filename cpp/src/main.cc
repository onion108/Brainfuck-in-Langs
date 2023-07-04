#include <cstring>
#include <fstream>
#include <iostream>
#include "compiler/compiler.hpp"
#include "util/to_string.hpp"
#include "util/util.hpp"
#include "vm/vm.hpp"
#include <optional>
#include <sstream>
#include <string>
#include <string_view>


void show_help(const char* program_name) {
    utils::println("Usage: ");
    utils::println(program_name, " [source_file]");
    utils::println(program_name, " [source_file] [output_file]");
    utils::println("Description: ");
    utils::println("If one argument is given, run the [source_file] as a Brainfuck source file. It will interprete the content in that file as a Brainfuck source. ");
    utils::println("If two arguments are given, it will compile the [source_file] and output the result to the [output_file]. ");
}

std::optional<brainfuck::program> compile_from_path(const std::string_view& path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        utils::eprintln("Cannot open file: ", path);
        return {};
    }

    std::string content = (std::stringstream() << ifs.rdbuf()).str();
    ifs.close();

    try {
        return brainfuck::compile_string(content);
    } catch (brainfuck::unmatched_bracket) {
        utils::eprintln("Unmatched brackets were found in the source code. Aborted. ");
        return {};
    } catch (...) {
        utils::eprintln("Unknown error happened while compiling the source code. Aborted. ");
        return {};
    }
}

void run(const std::string_view& path) {
    auto prog = compile_from_path(path);
    prog.and_then([](auto p) -> std::optional<int> {
        brainfuck::vm vm(p);
        vm.run();
        return 0;
    });
}

void compile_to(const std::string_view& src_path, const std::string_view& dest_path) {
    auto prog = compile_from_path(src_path);
    prog.and_then([&dest_path](auto p) -> std::optional<int> {
        std::ofstream ofs(dest_path);
        if (!ofs.is_open()) {
            utils::eprintln("Cannot open file ", dest_path, " , aborted. ");
            return 0;
        }
        utils::sprintln(ofs, p);
        ofs.close();
        return 0;
    });
}

int main(int argc, char **argv) {
    switch (argc) {
        case 0:
            show_help("cppbfi");
            break;
        case 2:
            run(argv[1]);
            break;
        case 3:
            compile_to(argv[1], argv[2]);
            break;
        case 1:
        default:
            show_help(argv[0]);
            break;
    }
}

