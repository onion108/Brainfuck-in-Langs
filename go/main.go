package main

import (
	"fmt"
	"gobfi/brainfuck/compiler"
	"gobfi/brainfuck/vm"
	"os"
)

func showHelp(progName string) {
    fmt.Printf("Usage: \n")
    fmt.Printf("%s [source_file]\n", progName)
    fmt.Printf("%s [source_file] [output_file]\n", progName)
    fmt.Printf("Description: \n")
    fmt.Printf("If one argument is given, run the [source_file] as a Brainfuck source file. It will interprete the content in that file as a Brainfuck source. \n")
    fmt.Printf("If two arguments are given, it will compile the [source_file] and output the result to the [output_file]\n")
}

func compileSourceFile(path string) (*vm.Program, error) {
    src, err := os.ReadFile(path)
    if err != nil {
        return nil, err
    }
    return compiler.Compile(string(src))
}

func compileSourceFileAndRun(path string) {
    prog, err := compileSourceFile(path)
    if err != nil {
        fmt.Fprintf(os.Stderr, "Cannot compile the source due to: %v", err)
    }
    
    machine := vm.MakeVM(prog)
    machine.Run()
}

func compileSourceFileAndDump(path string, dest string) {
    prog, err := compileSourceFile(path)
    if err != nil {
        fmt.Fprint(os.Stderr, "Cannot compile the source due to:", err, "\n")
    }

    f, err := os.Create(dest)
    if err != nil {
        fmt.Fprint(os.Stderr, "Cannot dump compiled result due to:", err, "\n")
    }
    _, err = f.WriteString(prog.String())
    if err != nil {
        fmt.Fprint(os.Stderr, "Cannot dump compiled result due to:", err, "\n")
    }
}

func main() { 
    args := os.Args
    switch {
    case len(args) == 0:
        showHelp("gobfi")
    case len(args) == 2:
        compileSourceFileAndRun(args[1])
    case len(args) == 3:
        compileSourceFileAndDump(args[1], args[2])
    default:
        showHelp(args[0])
    }
}

