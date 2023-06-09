pub mod error;

use std::io::Read;
use std::{fs::OpenOptions, path::Path};
use crate::compiler::error::CompileError;

use crate::vm::{Program, Instruction};

fn clean_code(source_code_slice: &[char]) -> &[char] {
    if source_code_slice.is_empty() {
        return &[];
    }
    let mut idx = 0;
    while !['+', '-', '<', '>', '.', ',', '[', ']'].contains(&source_code_slice[idx]) {
        idx += 1;
        if idx >= source_code_slice.len() {
            return &[];
        }
    }
    &source_code_slice[idx..]
}

fn produce_new_instruction(source_code_slice: &[char], match_state: i32) -> Option<(&[char], Instruction, i32)> {
    let source_code_slice = clean_code(source_code_slice);
    if source_code_slice.is_empty() {
        None
    } else {
        match source_code_slice[0] {
            '<' => {
                Some((&source_code_slice[1..], Instruction::MovePointerLeft, match_state))
            }
            '>' => {
                Some((&source_code_slice[1..], Instruction::MovePointerRight, match_state))
            }
            '[' => {
                Some((&source_code_slice[1..], Instruction::UncompletedMatch(match_state), match_state+1))
            }
            ']' => {
                Some((&source_code_slice[1..], Instruction::UncompletedMatch(match_state-1), match_state-1))
            }
            '.' => {
                Some((&source_code_slice[1..], Instruction::Output, match_state))
            }
            ',' => {
                Some((&source_code_slice[1..], Instruction::Input, match_state))
            }
            ch if ['+', '-'].contains(&ch) => {
                let mut idx = 0;
                let mut fold_counter = 0;
                loop {
                    match source_code_slice[idx] {
                        '+' => {
                            fold_counter += 1;
                        }
                        '-' => {
                            fold_counter -= 1;
                        }
                        _ => {
                            let instr = match fold_counter {
                                0 => break produce_new_instruction(&source_code_slice[idx..], match_state),
                                1 => Instruction::Increase,
                                -1 => Instruction::Decrease,
                                delta if delta < -1 => Instruction::Substract(-delta),
                                delta if delta > 1 => Instruction::Add(delta),
                                _ => unreachable!(),
                            };
                            break Some((&source_code_slice[idx..], instr, match_state));
                        }
                    }
                    idx += 1;
                }
            }
            _ => {
                unreachable!()
            }
        }
    }
}

fn match_uncompleted(program: &mut Program, addr: usize) -> Result<(), CompileError> {
    if let Instruction::UncompletedMatch(label) = program.instruction_at(addr) {
        let mut addr_counter = addr+1;
        let mut completed = false;
        while addr_counter < program.len() {
            match program.instruction_at(addr_counter) {
                Instruction::UncompletedMatch(l) if l == label => {
                    program.modify_instruction_at(addr,  Instruction::JumpIfZero(addr_counter));
                    program.modify_instruction_at(addr_counter, Instruction::JumpIfNotZero(addr));
                    completed = true;
                    break;
                }
                _ => {
                    addr_counter += 1;
                }
            }
        }
        if completed {
            Ok(())
        } else {
            Err(CompileError::UnmatchedBrackets)
        }
    } else {
        Ok(())
    }
}

pub fn compile(source_code: &str) -> Result<Program, CompileError> {
    let source_code: Vec<char> = source_code.chars().collect();
    let mut source_code_slice = &source_code[..];
    let mut program = Program::new();
    let mut match_state = 0;
    while let Some((remain, instr, new_match_state)) = produce_new_instruction(source_code_slice, match_state) {
        match_state = new_match_state;
        program.add_instruction(instr);
        source_code_slice = remain;
    }
    for i in 0..program.len() {
        match_uncompleted(&mut program, i)?;
    }
    Ok(program)
}

pub fn compile_from_file<P>(path: P) -> Result<Program, CompileError> where P: AsRef<Path> {
    let mut f = OpenOptions::new().read(true).write(false).truncate(false).open(path)?;
    let mut buffer = String::new();
    f.read_to_string(&mut buffer)?;
    compile(&buffer)
}

#[allow(unused)]
mod test {
    use super::compile;

    #[test]
    fn test_compile() {
        println!("{}", compile("+++++++++--->><<++>>.[...[],],").unwrap());
    }
}

