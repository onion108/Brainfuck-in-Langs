use std::env::{Args, args};

use rsbfi::{compile_and_run, compile_to_file};

fn show_help(program_name: &str) {
    println!("Usage: ");
    println!("{} [source_file]", program_name);
    println!("{} [source_file] [output_file]", program_name);
    println!("Description: ");
    println!("If one argument is given, run the [source_file] as a Brainfuck source file. It will interprete the content in that file as a Brainfuck source. ");
    println!("If two arguments are given, it will compile the [source_file] and output the result to the [output_file]. ");
}

fn main() {
    let args = args().collect::<Vec<String>>();
    if let Err(err) = match args.len() {
        0 => Ok(show_help("rsbfi")),
        2 => compile_and_run(&args[1]),
        3 => compile_to_file(&args[1], &args[2]),
        _ => Ok(show_help(&args[0])),
    } {
        eprintln!("ERROR: {}", err);
    }
}

