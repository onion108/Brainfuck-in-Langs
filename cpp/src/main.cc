#include <iostream>
#include "util/to_string.hpp"
#include "util/util.hpp"
#include "vm/vm.hpp"
#include <string>

struct A {};

struct B {
    std::string to_string() {
        return "B object";
    }
};

int main() {
    utils::println("hello, world");
    utils::println(utils::to_string("Hi"), ' ', utils::to_string(114514));
    utils::println("A: ", A {});
    utils::println("B: ", B {});
    brainfuck::program prog;
    using itype = brainfuck::instruction::type;
    using it = brainfuck::instruction;
    prog
        .add_instruction(it(itype::add, 'H'))
        .add_instruction(it(itype::out))
        .add_instruction(it(itype::ptr_r))
        .add_instruction(it(itype::add, 'e'))
        .add_instruction(it(itype::out))
        .add_instruction(it(itype::ptr_l))
        .add_instruction(it(itype::out));
    brainfuck::vm vm(prog);
    vm.run();
    utils::println();
}

