package vm

type Memory struct {
    positiveSpace []int
    negativeSpace []int
}

func MakeMemory() *Memory {
    return &Memory{
        positiveSpace: make([]int, 256),
        negativeSpace: make([]int, 256),
    }
}

func (memory *Memory) ensureIndexAvailable(idx int) {
    if idx >= 0 {
        for idx >= len(memory.positiveSpace) {
            memory.positiveSpace = append(memory.positiveSpace, 0)
        }
    } else {
        for (-idx-1) >= len(memory.negativeSpace) {
            memory.negativeSpace = append(memory.negativeSpace, 0)
        }
    }
}

func (memory *Memory) ReadSlot(addr int) int {
    memory.ensureIndexAvailable(addr)
    if addr >= 0 {
        return memory.positiveSpace[addr];
    } else {
        return memory.negativeSpace[-addr-1];
    }
}

func (memory *Memory) WriteSlot(addr int, data int) {
    memory.ensureIndexAvailable(addr)
    if addr >= 0 {
        memory.positiveSpace[addr] = data
    } else {
        memory.negativeSpace[-addr-1] = data
    }
}

