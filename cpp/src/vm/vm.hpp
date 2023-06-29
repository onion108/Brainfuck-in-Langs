#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <functional>

namespace brainfuck {

    class memory {
        private:
            std::vector<int> positive_space;
            std::vector<int> negative_space;
            void _assume_index_available(int idx);
        public:
            memory() : positive_space(256), negative_space(256) {}
            int& operator[](int idx);
    };          

    struct instruction {

        static const unsigned short MASK_SINGLE_ARG = 0x0100;
        static const unsigned short MASK_UNCOMPLETED = 0x0200;
        static const unsigned short MASK_DEBUG = 0x0400;
        static const unsigned short MASK_IO = 0x0800;
        static const unsigned short MASK_UNUSED_0 = 0x1000;
        static const unsigned short MASK_UNUSED_1 = 0x2000;
        static const unsigned short MASK_UNUSED_2 = 0x4000;
        static const unsigned short MASK_UNUSED_3 = 0x8000;

        enum class type : short {
            add = 0x0001 | MASK_SINGLE_ARG,
            sub = 0x0002 | MASK_SINGLE_ARG,
            inc = 0x0003,
            dec = 0x0004,
            ptr_l = 0x0005,
            ptr_r = 0x0006,
            jz = 0x0007 | MASK_SINGLE_ARG,
            jnz = 0x0008 | MASK_SINGLE_ARG,
            unc = 0x0009 | MASK_UNCOMPLETED | MASK_SINGLE_ARG,
            ud = 0x0010 | MASK_UNCOMPLETED,
            int3 = 0x0011 | MASK_DEBUG,
            in = 0x0012 | MASK_IO,
            out = 0x0013 | MASK_IO,
        };

        type instruction_type;
        int arg;

        instruction(type instruction_type, int arg) : instruction_type(instruction_type), arg(arg) {}
        instruction(type instruction_type) : instruction(instruction_type, -1) {}
        instruction() : instruction(type::ud) {}
        std::string to_string();
        inline bool has_arg() {
            return (unsigned short)(instruction_type) & MASK_SINGLE_ARG;
        }
    };

    class program {
        std::vector<instruction> instructions;
        public:
        program() : instructions() {}

        program& add_instruction(const instruction& inst); 
        instruction get_instruction_at(std::size_t idx);
        void modify_instruction_at(std::size_t idx, const instruction& new_instruction);
        std::string to_string();
        inline std::size_t get_size() { return instructions.size(); }
    };

    struct vm {
        using debug_handler_t = std::function<void(vm&)>;

        program prog;
        memory mem;
        std::size_t pc;
        int pointer;
        debug_handler_t debug_handler;
        
        vm(const program& prog, debug_handler_t custom_debug_handler) : prog(prog), mem(), pc(0), pointer(0), debug_handler(custom_debug_handler) {}
        vm(const program& prog) : vm(prog, [] (vm& instance) { instance.pc += 1; }) {}
        vm() : vm(program()) {}

        void run();
    };

}

