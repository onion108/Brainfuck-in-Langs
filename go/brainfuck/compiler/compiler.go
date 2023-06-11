package compiler

import (
	"fmt"
	"gobfi/brainfuck/vm"
)

func checkIfValid(char byte) bool {
    return char == '+' || char == '-' || char == '<' || char == '>' || char == '.' || char == ',' || char == '[' || char == ']'
}

func eatSource(source string) string {
    result := source[:]
    if len(source) == 0 {
        return ""
    }
    for len(result) != 0 && !checkIfValid(result[0]) {
        result = result[1:]
    }
    return result
}

func foldRedundantIncDec(source string, matchState int) (vm.Instruction, string, int) {
    remain := source[:]
    foldCounter := 0
    for (remain[0] == '+' || remain[0] == '-') && len(remain) != 0 {
        if remain[0] == '+' {
            foldCounter += 1
        } else {
            foldCounter -= 1
        }
        remain = remain[1:]
    }
    if foldCounter == 0 {
        return produceAnInstruction(remain, matchState)
    }
    if foldCounter == 1 {
        return vm.IncrementInstruction{}, remain, matchState
    }
    if foldCounter == -1 {
        return vm.DecrementInstruction{}, remain, matchState
    }
    if foldCounter > 1 {
        return vm.AddInstruction{Value: foldCounter}, remain, matchState
    }
    return vm.SubstractInstruction{Value: -foldCounter}, remain, matchState
}

func produceAnInstruction(source string, matchState int) (vm.Instruction, string, int) {
    remain := eatSource(source)
    if len(remain) == 0 {
        return vm.UncompletedMatchInstruction{Label: -1}, "", matchState
    }
    switch remain[0] {
    case '<':
        return vm.PointerMoveLeftInstruction{}, remain[1:], matchState
    case '>':
        return vm.PointerMoveRightInstruction{}, remain[1:], matchState
    case '[':
        return vm.UncompletedMatchInstruction{Label: matchState}, remain[1:], matchState+1
    case ']':
        return vm.UncompletedMatchInstruction{Label: matchState-1}, remain[1:], matchState-1 
    case '.':
        return vm.OutputInstruction{}, remain[1:], matchState
    case ',':
        return vm.InputInstruction{}, remain[1:], matchState
    case '+', '-':
        return foldRedundantIncDec(remain, matchState)
    default:
        panic("Unreachable")
    }
}

func matchUncompleted(program *vm.Program) error {
    instructions := program.GetInstructionSequence()
    virtPc := 0
    for virtPc < len(instructions) {
        switch instructions[virtPc].(type) {
        case vm.UncompletedMatchInstruction:
            toMatch := instructions[virtPc].(vm.UncompletedMatchInstruction).Label
            matched := false
            for i := virtPc+1; i < len(instructions); i++ {
                if uncompleted, ok := instructions[i].(vm.UncompletedMatchInstruction); ok {
                    if uncompleted.Label == toMatch {
                        matched = true
                        instructions[i] = vm.JumpIfNonZeroInstruction{Dest: uint32(virtPc)}
                        instructions[virtPc] = vm.JumpIfZeroInstruction{Dest: uint32(i)}
                        break
                    }
                }
            }
            if !matched {
                return fmt.Errorf("Unmatched bracket at %08X", virtPc)
            }
        default:
        }
        virtPc += 1
    }
    program.SetInstructionSequence(instructions)
    return nil
}

func Compile(source string) (result *vm.Program, err error) {
    result = vm.MakeProgram()
    inst, remain, matchState := produceAnInstruction(source, 0)
    for len(remain) != 0 {
        result.AddInstruction(inst)
        inst, remain, matchState = produceAnInstruction(remain, matchState)
    }
    err = matchUncompleted(result)
    return
}

