use std::{fs::OpenOptions, io::Write};

use compiler::{error::CompileError, compile_from_file};
use vm::VM;

pub mod vm;

pub mod compiler;

pub fn compile_and_run(source: &str) -> Result<(), CompileError> {
    let program = compile_from_file(source)?;
    let mut vm = VM::new(program);
    vm.run();
    Ok(())
}

pub fn compile_to_file(source: &str, dest: &str) -> Result<(), CompileError> {
    let program = compile_from_file(source)?;
    let mut dest_file = OpenOptions::new().write(true).truncate(true).create(true).open(dest)?;
    writeln!(dest_file, "{}", program)?;
    Ok(())
}

