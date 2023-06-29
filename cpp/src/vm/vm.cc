#include "vm.hpp"
#include "../util/util.hpp"
#include <functional>
#include <ios>
#include <iostream>
#include <iomanip>
#include <sstream>

std::string hex(std::size_t number) {
    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << number;
    return ss.str();
}

void brainfuck::memory::_assume_index_available(int idx) {
    if (idx >= 0) {
        while (idx >= positive_space.size()) {
            positive_space.push_back(0);
        }
    } else {
        while ((-idx-1) >= negative_space.size()) {
            negative_space.push_back(0);
        }
    }
}

int& brainfuck::memory::operator[](int idx) {
    _assume_index_available(idx);
    return idx >= 0 ? positive_space[idx] : negative_space[-idx-1];
}

std::string brainfuck::instruction::to_string() {
    switch (instruction_type) {
        case type::add:
            return utils::concat("add ", arg);
        case type::sub:
            return utils::concat("sub ", arg);
        case type::inc:
            return "inc";
        case type::dec:
            return "dec";
        case type::ptr_l:
            return "ptr_l";
        case type::ptr_r:
            return "ptr_r";
        case type::jz:
            return utils::concat("jz ", "0x", hex(arg));
        case type::jnz:
            return utils::concat("jnz ", "0x", hex(arg));
        case type::unc:
            return utils::concat("?[", arg, "]");
        case type::ud:
            return "ud";
        case type::int3:
            return "int3";
        case type::in:
            return "in";
        case type::out:
            return "out";
    }
}

brainfuck::program& brainfuck::program::add_instruction(const instruction &inst) {
    instructions.push_back(inst);
    return *this;
}

brainfuck::instruction brainfuck::program::get_instruction_at(std::size_t idx) {
    return instructions[idx];
}

void brainfuck::program::modify_instruction_at(std::size_t idx, const instruction& new_instruction) {
    instructions[idx] = new_instruction;
}


std::string brainfuck::program::to_string() {
    std::stringstream result;
    utils::sprintln(result, "; PROGRAM with length ", instructions.size());
    std::size_t counter = 0;
    for (const auto& i : instructions) {
        utils::sprintln(result, i, "         ; addr=", hex(counter));
        counter += 1;
    }
    return result.str();
}

void brainfuck::vm::run() {
    while (pc < prog.get_size()) {
        auto inst = prog.get_instruction_at(pc);
        using itype = instruction::type;
        switch (inst.instruction_type) {
            case itype::add:
                mem[pointer] = mem[pointer] + inst.arg;
                pc += 1;
                break;
            case itype::sub:
                mem[pointer] = mem[pointer] - inst.arg;
                pc += 1;
                break;
            case itype::inc:
                mem[pointer] = mem[pointer] + 1;
                pc += 1;
                break;
            case itype::dec:
                mem[pointer] = mem[pointer] - 1;
                pc += 1;
                break;
            case itype::int3:
                debug_handler(*this);
            case itype::jz:
                if (mem[pointer] == 0) {
                    pc = inst.arg;
                } else {
                    pc += 1;
                }
                break;
            case itype::jnz:
                if (mem[pointer] != 0) {
                    pc = inst.arg;
                } else {
                    pc += 1;
                }
                break;
            case itype::in:
                mem[pointer] = std::cin.get();
                pc += 1;
                break;
            case itype::out:
                utils::print(char(mem[pointer]));
                pc += 1;
                break;
            case itype::ptr_l:
                pointer -= 1;
                pc += 1;
                break;
            case itype::ptr_r:
                pointer += 1;
                pc += 1;
                break;
            case itype::unc:
            case itype::ud:
                throw std::bad_function_call();
                break;
        }
    }
}

