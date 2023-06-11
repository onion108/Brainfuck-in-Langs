package vm

import (
	"bufio"
	"fmt"
	"os"
)

type AddInstruction struct {
    Value int
}

func (inst AddInstruction) execute(vm *VM) {
    vm.memory.WriteSlot(vm.ptr, vm.memory.ReadSlot(vm.ptr) + inst.Value)
    vm.pc += 1
}

func (inst AddInstruction) String() string {
    return fmt.Sprintf("add %d", inst.Value)
}

type SubstractInstruction struct {
    Value int
}

func (inst SubstractInstruction) execute(vm *VM) {
    vm.memory.WriteSlot(vm.ptr, vm.memory.ReadSlot(vm.ptr) - inst.Value)
    vm.pc += 1
}

func (inst SubstractInstruction) String() string {
    return fmt.Sprintf("sub %d", inst.Value)
}

type PointerMoveLeftInstruction struct {}

func (inst PointerMoveLeftInstruction) execute(vm *VM) {
    vm.ptr -= 1
    vm.pc += 1
}

func (inst PointerMoveLeftInstruction) String() string {
    return "ptr_l"
}

type PointerMoveRightInstruction struct {}

func (inst PointerMoveRightInstruction) execute(vm *VM) {
    vm.ptr += 1
    vm.pc += 1
}

func (inst PointerMoveRightInstruction) String() string {
    return "ptr_r"
}

type IncrementInstruction struct {}

func (inst IncrementInstruction) execute(vm *VM) {
    vm.memory.WriteSlot(vm.ptr, vm.memory.ReadSlot(vm.ptr) + 1)
    vm.pc += 1
}

func (inst IncrementInstruction) String() string {
    return "inc"
}

type DecrementInstruction struct {}

func (inst DecrementInstruction) execute(vm *VM) {
    vm.memory.WriteSlot(vm.ptr, vm.memory.ReadSlot(vm.ptr) - 1)
    vm.pc += 1
}

func (inst DecrementInstruction) String() string {
    return "dec"
}

type JumpIfZeroInstruction struct {
    Dest uint32
}

func (inst JumpIfZeroInstruction) execute(vm *VM) {
    if vm.memory.ReadSlot(vm.ptr) == 0 {
        vm.pc = inst.Dest
    } else {
        vm.pc += 1
    }
}

func (inst JumpIfZeroInstruction) String() string {
    return fmt.Sprintf("jz 0x%08X", inst.Dest)
}

type JumpIfNonZeroInstruction struct {
    Dest uint32
}

func (inst JumpIfNonZeroInstruction) execute(vm *VM) {
    if vm.memory.ReadSlot(vm.ptr) != 0 {
        vm.pc = inst.Dest
    } else {
        vm.pc += 1
    }
}

func (inst JumpIfNonZeroInstruction) String() string {
    return fmt.Sprintf("jnz 0x%08X", inst.Dest)
}

type OutputInstruction struct {}

func (inst OutputInstruction) execute(vm *VM) {
    fmt.Printf("%c", vm.memory.ReadSlot(vm.ptr))
    vm.pc += 1
}

func (inst OutputInstruction) String() string {
    return "out"
}

type InputInstruction struct {}

func (inst InputInstruction) execute(vm *VM) {
    reader := bufio.NewReader(os.Stdin)
    b, _ := reader.ReadByte()
    vm.memory.WriteSlot(vm.ptr, int(b))
}

func (inst InputInstruction) String() string {
    return "in";
}

type UncompletedMatchInstruction struct {
    Label int
}

func (inst UncompletedMatchInstruction) execute(vm *VM) {
    panic("gg")
}

func (inst UncompletedMatchInstruction) String() string {
    return fmt.Sprintf("?[%d]", inst.Label)
}

