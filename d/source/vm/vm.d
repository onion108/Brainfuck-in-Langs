module vm.vm;
import std.stdio;
import std.format;
import vm.memory;

class Instruction {
    abstract void execute(VM vm);
    abstract string getStringRepresentation();
    bool isUncompleted()
    {
        return false;
    }
}

class Program {
    Instruction[] instructions;
    this() 
    {
        instructions = new Instruction[0];
    }

    void addInstruction(Instruction instruction)
    {
        instructions ~= instruction;
    }
    
    override string toString()
    {
        string result = format("; PROGRAM with length %d\n", instructions.length);
        int virtPc = 0;
        foreach(i; instructions)
        {
            result ~= format("%s ;%08X\n", i.getStringRepresentation(), virtPc);
            virtPc++;
        }
        return result;
    }
}

class VM {
    private Program program;
    public int pc;
    int ptr;
    private Memory memory;

    this(Program program) 
    {
        this.pc = 0;
        this.ptr = 0;
        this.program = program;
        this.memory = new Memory();
    }

    public void run() 
    {
        while (this.pc < this.program.instructions.length) 
        {
            //writeln(this.pc);
            //writeln(this.memory.positiveSpace);
            Instruction instruction = this.program.instructions[this.pc];
            instruction.execute(this);
        }
    }

    public int getCurrentMemorySlot()
    {
        int *addr = memory.addressOf(this.ptr);
        return *addr;
    }

    public void setCurrentMemorySlot(int value)
    {
        int *addr = memory.addressOf(this.ptr);
        *addr = value;
    }
}

