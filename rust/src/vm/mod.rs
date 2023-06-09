use std::{fmt::Display, io::{stdin, Read, stdout, Write}};

use self::memory::Memory;

pub mod memory;

#[derive(Debug, Copy, Clone)]
pub enum Instruction {
    Add(i32),
    Substract(i32),
    Increase,
    Decrease,
    MovePointerLeft,
    MovePointerRight,
    Input,
    Output,
    JumpIfZero(usize),
    JumpIfNotZero(usize),
    UncompletedMatch(i32),
}

impl Display for Instruction {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            &Self::Add(delta) => write!(f, "add {}", delta)?,
            &Self::Substract(delta) => write!(f, "sub {}", delta)?,
            &Self::Increase => write!(f, "inc")?,
            &Self::Decrease => write!(f, "dec")?,
            &Self::MovePointerLeft => write!(f, "ptr_l")?,
            &Self::MovePointerRight => write!(f, "ptr_r")?,
            &Self::Input => write!(f, "in")?,
            &Self::Output => write!(f, "out")?,
            &Self::JumpIfZero(dest) => write!(f, "jz 0x{:08X}", dest)?,
            &Self::JumpIfNotZero(dest) => write!(f, "jnz 0x{:08X}", dest)?,
            &Self::UncompletedMatch(label) => write!(f, "?[{}]", label)?,
        }
        Ok(())
    }
}

#[derive(Clone, Debug)]
pub struct Program {
    instructions: Vec<Instruction>,
}

impl Program {
    pub fn new() -> Self {
        Self {
            instructions: Vec::new(),
        }
    }
    pub fn add_instruction(&mut self, instruction: Instruction) {
        self.instructions.push(instruction);
    }
    pub fn len(&self) -> usize {
        self.instructions.len()
    }
    pub fn instruction_at(&self, addr: usize) -> Instruction {
        return self.instructions[addr];
    }
    pub fn modify_instruction_at(&mut self, addr: usize, new_instr: Instruction) {
        self.instructions[addr] = new_instr;
    }
}

impl Display for Program {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "; PROGRAM with length {}\n", self.instructions.len())?;
        let mut virt_pc = 0;
        for i in &self.instructions {
            write!(f, "{}         ; addr={:08X}\n", i, virt_pc)?;
            virt_pc += 1;
        }
        Ok(())
    }
}

#[derive(Clone, Debug)]
pub struct VM {
    program: Program,
    pc: usize,
    ptr: i32,
    memory: Memory,
}

impl VM {
    pub fn new(program: Program) -> VM {
        VM {
            program,
            pc: 0,
            ptr: 0,
            memory: Memory::new(),
        }
    }
    pub fn run(&mut self) {
        while self.pc < self.program.instructions.len() {
            let i = &self.program.instructions[self.pc];
            match i {

                &Instruction::Add(delta) => {
                    let value = self.memory.get(self.ptr);
                    self.memory.set(self.ptr, value + delta);
                    self.pc += 1;
                }

                &Instruction::Substract(delta) => {
                    let value = self.memory.get(self.ptr);
                    self.memory.set(self.ptr, value - delta);
                    self.pc += 1;
                }

                &Instruction::Increase => {
                    let value = self.memory.get(self.ptr);
                    self.memory.set(self.ptr, value + 1);
                    self.pc += 1;
                }

                &Instruction::Decrease => {
                    let value = self.memory.get(self.ptr);
                    self.memory.set(self.ptr, value - 1);
                    self.pc += 1; 
                }

                &Instruction::MovePointerLeft => {
                    self.ptr -= 1;
                    self.pc += 1;
                }

                &Instruction::MovePointerRight => {
                    self.ptr += 1;
                    self.pc += 1;
                }

                &Instruction::Output => {
                    unsafe {
                        print!("{}", char::from_u32_unchecked(self.memory.get(self.ptr) as u32));
                    }
                    self.pc += 1;
                }

                &Instruction::Input => {
                    stdout().flush().expect("Failed to flush stdout. ");
                    let buffer = &mut [0u8];
                    stdin().read_exact(buffer).expect("Unable to read input. ");
                    self.pc += 1;
                }

                &Instruction::JumpIfZero(dest) => {
                    if self.memory.get(self.ptr) == 0 {
                        self.pc = dest;
                    } else {
                        self.pc += 1;
                    }
                }

                &Instruction::JumpIfNotZero(dest) => {
                    if self.memory.get(self.ptr) != 0 {
                        self.pc = dest;
                    } else {
                        self.pc += 1;
                    }
                }

                &Instruction::UncompletedMatch(label) => {
                    panic!("Uncompleted match: {}", label);
                }

            }
        }            
        stdout().flush().expect("Failed to flush stdout. ");
    }
}

#[allow(unused)]
mod test {
    use super::*;

    #[test]
    fn test_format() {
        let mut p = Program::new();
        p.add_instruction(Instruction::Add(114514));
        p.add_instruction(Instruction::Output);
        p.add_instruction(Instruction::Substract(135));
        p.add_instruction(Instruction::Output);
        p.add_instruction(Instruction::JumpIfZero(2));
        println!("{}", p);
    }

    #[test]
    fn test_vm_execution() {
        let mut p = Program::new();
        p.add_instruction(Instruction::MovePointerLeft);
        p.add_instruction(Instruction::MovePointerLeft);
        p.add_instruction(Instruction::Add('H' as i32));
        p.add_instruction(Instruction::Output);
        p.add_instruction(Instruction::Decrease);
        p.add_instruction(Instruction::JumpIfNotZero(4));
        p.add_instruction(Instruction::Add('e' as i32));
        p.add_instruction(Instruction::Output);
        p.add_instruction(Instruction::Decrease);
        p.add_instruction(Instruction::JumpIfNotZero(8));
        p.add_instruction(Instruction::MovePointerRight);
        p.add_instruction(Instruction::MovePointerRight);
        p.add_instruction(Instruction::MovePointerRight);
        p.add_instruction(Instruction::Add('\n' as i32));
        p.add_instruction(Instruction::Output);

        println!("{}", p);
        let mut vm = VM::new(p);
        vm.run();
    }
}

