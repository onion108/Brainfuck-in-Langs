#include "../compiler/compiler.hpp"
#include "../util/util.hpp"
#include <exception>
#include <utility>
#include <optional>
#include <string_view>

bool is_valid_operator(char op) {
    return 
        op == '+' ||
        op == '-' ||
        op == '[' ||
        op == ']' ||
        op == '<' ||
        op == '>' ||
        op == '.' ||
        op == ',';
}

std::string_view eat_invalid(const std::string_view& code) {
    // If the code is empty, just return without doing anything.
    if (code.empty()) {
        return code;
    }

    int pos = 0;
    while ((pos < code.length()) && !is_valid_operator(code[pos])) {
        pos += 1;
    }
    return code.substr(pos);
}

struct produced_instruction_indetermined_t {
    brainfuck::instruction instr;
    std::string_view remaining;
    int matcher;
};

std::optional<produced_instruction_indetermined_t> produce_instruction(const std::string_view& src, int match_state) {
    auto code = eat_invalid(src);
    if (code.empty()) {
        return {}; 
    }

    using itype = brainfuck::instruction::type;
    int pos = 1;
    switch (code[0]) {
        case '[':
            return produced_instruction_indetermined_t {
                brainfuck::instruction(itype::unc, match_state),
                code.substr(pos),
                match_state + 1
            };
        case ']':
            return produced_instruction_indetermined_t {
                brainfuck::instruction(itype::unc, match_state - 1),
                code.substr(pos),
                match_state - 1
            };
        case '.':
            return produced_instruction_indetermined_t {
                brainfuck::instruction(itype::out),
                code.substr(pos),
                match_state
            };
        case ',':
            return produced_instruction_indetermined_t {
                brainfuck::instruction(itype::in),
                code.substr(pos),
                match_state
            };
        case '<':
            return produced_instruction_indetermined_t {
                brainfuck::instruction(itype::ptr_l),
                code.substr(pos),
                match_state
            };
        case '>':
            return produced_instruction_indetermined_t {
                brainfuck::instruction(itype::ptr_r),
                code.substr(pos),
                match_state
            };
        case '+':
        case '-':
            pos = 0;
            int fold_counter = 0;
            while (pos < code.length() && (code[pos] == '+' || code[pos] == '-')) {
                if (code[pos] == '+') {
                    fold_counter += 1;
                }
                if (code[pos] == '-') {
                    fold_counter -= 1;
                }
                pos += 1;
            }
            auto remain = code.substr(pos);
            if (fold_counter == 0) {
                return produce_instruction(remain, match_state);
            }
            if (fold_counter == 1) {
                return produced_instruction_indetermined_t {
                    brainfuck::instruction(itype::inc),
                    remain,
                    match_state,
                };
            }
            if (fold_counter == -1) {
                return produced_instruction_indetermined_t {
                    brainfuck::instruction(itype::dec),
                    remain,
                    match_state,
                };
            }
            if (fold_counter > 1) {
                return produced_instruction_indetermined_t {
                    brainfuck::instruction(itype::add, fold_counter),
                    remain,
                    match_state,
                };
            }
            if (fold_counter < -1) {
                return produced_instruction_indetermined_t {
                    brainfuck::instruction(itype::sub, -fold_counter),
                    remain,
                    match_state,
                };
            }
            std::unreachable();
    }
    std::unreachable();
}

void match_brackets(brainfuck::program& prog, std::size_t start) {
    if (prog.get_instruction_at(start).instruction_type != brainfuck::instruction::type::unc) {
        return;
    }
    std::size_t pos = start + 1;
    int flag = prog.get_instruction_at(start).arg;
    bool found = false;
    while (pos < prog.get_size()) {
        if (prog.get_instruction_at(pos).instruction_type == brainfuck::instruction::type::unc && prog.get_instruction_at(pos).arg == flag) {
            prog.modify_instruction_at(start, brainfuck::instruction(brainfuck::instruction::type::jz, pos));
            prog.modify_instruction_at(pos, brainfuck::instruction(brainfuck::instruction::type::jnz, start));
            found = true;
            break;
        }
        pos += 1;
    }
    if (!found) {
        throw brainfuck::unmatched_bracket();
    }
}

auto brainfuck::compile_string(const std::string& code) -> program {
    std::string_view code_view = code;
    program p;

    // 1. Generate the uncompleted code.
    auto rem = produce_instruction(code_view, 0);
    while (rem.has_value()) {
        p.add_instruction(rem.value().instr);
        rem = produce_instruction(rem.value().remaining, rem.value().matcher);
    }

    // 2. Match the brackets
    for (std::size_t i = 0; i < p.get_size(); i++) {
        match_brackets(p, i);
    }

    return p;
}


