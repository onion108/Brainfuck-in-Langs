module vm.instructions;
import vm.vm;
import std.format;
import std.stdio;

class AddInstruction : Instruction
{
    private int delta;
    
    this(int value)
    {
        delta = value;
    }

    override void execute(VM vm) 
    {
        vm.setCurrentMemorySlot(vm.getCurrentMemorySlot() + delta);
        vm.pc += 1;
    }

    override string getStringRepresentation()
    {
        return format("add %d", delta);
    }
} 

class SubInstruction : Instruction
{
    private int delta;
    
    this(int value)
    {
        delta = value;
    }

    override void execute(VM vm) 
    {
        vm.setCurrentMemorySlot(vm.getCurrentMemorySlot() - delta);
        vm.pc += 1;
    }

    override string getStringRepresentation()
    {
        return format("sub %d", delta);
    }
}

class IncInstruction : AddInstruction
{
    this() 
    {
        super(1);
    }

    override string getStringRepresentation()
    {
        return "inc";
    }
}

class DecInstruction : SubInstruction
{
    this() 
    {
        super(1);
    }

    override string getStringRepresentation()
    {
        return "dec";
    }
}

class JumpIfZeroInstruction : Instruction
{
    int dest;

    this(int dest)
    {
        this.dest = dest;
    }

    override void execute(VM vm)
    {
        if (vm.getCurrentMemorySlot() == 0) 
        {
            vm.pc = dest;
        }
        else 
        {
            vm.pc += 1;
        }
    }

    override string getStringRepresentation()
    {
        return format("jz 0x%08X", dest);
    }
}

class JumpIfNonZeroInstruction : Instruction
{
    int dest;

    this(int dest)
    {
        this.dest = dest;
    }

    override void execute(VM vm)
    {
        if (vm.getCurrentMemorySlot() != 0) 
        {
            vm.pc = dest;
        }
        else 
        {
            vm.pc += 1;
        }
    }

    override string getStringRepresentation()
    {
        return format("jnz 0x%08X", dest);
    }
}

class PointerMoveLeftInstruction : Instruction
{
    override void execute(VM vm)
    {
        vm.ptr -= 1;
        vm.pc += 1;
    }

    override string getStringRepresentation()
    {
        return "ptr_l";
    }
}

class PointerMoveRightInstruction : Instruction
{
    override void execute(VM vm)
    {
        vm.ptr += 1;
        vm.pc += 1;
    }

    override string getStringRepresentation()
    {
        return "ptr_r";
    }
}

class UncompletedMatchInstruction : Instruction
{
    private int label;

    this(int label)
    {
        this.label = label;
    }

    int getLabel()
    {
        return this.label;
    }

    override void execute(VM vm)
    {
        throw new Exception("Uncompleted bracket match found in runtime");
    }

    override string getStringRepresentation() 
    {
        return format("?[%d]", label);
    }

    override bool isUncompleted()
    {
        return true;
    }
}

class OutputInstruction : Instruction
{
    override void execute(VM vm)
    {
        writef("%c", cast(char)vm.getCurrentMemorySlot());
        vm.pc += 1;
    }

    override string getStringRepresentation()
    {
        return "out";
    }
}

class InputInstruction : Instruction
{
    override void execute(VM vm)
    {
        vm.setCurrentMemorySlot(getchar());
        vm.pc += 1;
    }

    override string getStringRepresentation()
    {
        return "in";
    }
}

