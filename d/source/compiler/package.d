module compiler;
import vm.vm;
import std.stdio;
import std.format;
import std.file;
import vm.instructions;

Program compile(string path) 
{
    
    File inputFile = File(path, "r");
    string content = "";
    while (!inputFile.eof())
    {
        content ~= inputFile.readln();
    }
    
    return compileString(content);

}

Program compileString(string content)
{
    Program result = new Program();
    int matchCounter = 0;
    bool isInFoldOptimization = false;
    int foldCount = 0;
    for (int idx = 0; idx < content.length; idx++)
    {
        auto i = content[idx];
        if (!isInFoldOptimization)
        {
            switch (i)
            {
                case '[':
                    result.addInstruction(new UncompletedMatchInstruction(matchCounter));
                    matchCounter++;
                    break;
                case ']':
                    matchCounter--;
                    result.addInstruction(new UncompletedMatchInstruction(matchCounter));
                    break;
                case '+':
                    foldCount = 1;
                    isInFoldOptimization = true;
                    break;
                case '-':
                    foldCount = -1;
                    isInFoldOptimization = true;
                    break;
                case '.':
                    result.addInstruction(new OutputInstruction());
                    break;
                case ',':
                    result.addInstruction(new InputInstruction());
                    break;
                case '<':
                    result.addInstruction(new PointerMoveLeftInstruction());
                    break;
                case '>':
                    result.addInstruction(new PointerMoveRightInstruction());
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch (i)
            {
                case '+':
                    foldCount += 1;
                    break;
                case '-':
                    foldCount -= 1;
                    break;
                default:
                    if (foldCount == 1)
                    {
                        result.addInstruction(new IncInstruction());
                    }
                    else if (foldCount == -1)
                    {
                        result.addInstruction(new DecInstruction());
                    }
                    else if (foldCount > 1)
                    {
                        result.addInstruction(new AddInstruction(foldCount));
                    }
                    else if (foldCount < -1)
                    {
                        result.addInstruction(new SubInstruction(-foldCount));
                    }
                    isInFoldOptimization = false;
                    idx--;
                    break;
            }
        }
    }

    for (int i = 0; i < result.instructions.length; i++)
    {
        if (result.instructions[i].isUncompleted())
        {
            int expected = (cast(UncompletedMatchInstruction)result.instructions[i]).getLabel();
            int found = -1;
            for (int j = i+1; j < result.instructions.length; j++)
            {
                if (
                        result.instructions[j].isUncompleted()
                        && (cast(UncompletedMatchInstruction)result.instructions[j]).getLabel() == expected
                )
                { 
                    found = j;
                    break;
                }
            }
            if (found == -1)
            {
                throw new Exception(format("Unmatched brackets at %d", i));
            }
            result.instructions[i] = new JumpIfZeroInstruction(found);
            result.instructions[found] = new JumpIfNonZeroInstruction(i);
        }
    }

    return result;
}

