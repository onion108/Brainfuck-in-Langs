package vm

import "fmt"

type Instruction interface {
	execute(vm *VM)
}

type Program struct {
	instructions []Instruction
}

func MakeProgram() *Program {
	return &Program{
		instructions: make([]Instruction, 0),
	}
}

func (program *Program) AddInstruction(instruction Instruction) {
	program.instructions = append(program.instructions, instruction)
}

func (program *Program) GetInstructionSequence() []Instruction {
	return program.instructions
}

func (program *Program) SetInstructionSequence(sequence []Instruction) {
	program.instructions = sequence
}

func (program *Program) String() string {
	result := fmt.Sprintf("; PROGRAM with length %d\n", len(program.instructions))
    for i, instr := range program.instructions {
        result += fmt.Sprintf("%v         ; addr=%08X\n", instr, i)
    }
    return result
}

type VM struct {
	program *Program
	pc      uint32
	ptr     int
	memory  *Memory
}

func MakeVM(program *Program) *VM {
	return &VM{
		program: program,
		pc:      0,
		ptr:     0,
		memory:  MakeMemory(),
	}
}

func (vm *VM) Run() {
	for vm.pc < uint32(len(vm.program.instructions)) {
		vm.program.instructions[vm.pc].execute(vm)
	}
}
