module vm.memory;
import std.stdio;

class Memory
{
    int[] positiveSpace;
    int[] negativeSpace;

    this()
    {
        positiveSpace = new int[256];
        foreach (ref i; positiveSpace)
        {
            i = 0;
        }
        negativeSpace = new int[256];
        foreach (ref i; positiveSpace)
        {
            i = 0;
        }
    }

    int *addressOf(int index)
    {
        if (index >= 0)
        {
            if (index < positiveSpace.length) 
            {
                return &positiveSpace[index];
            }
            else
            {
                while (index >= positiveSpace.length)
                {
                    positiveSpace ~= 0;
                }
                return &positiveSpace[index];
            }
        }
        else
        {
            int transformed_index = -index-1;
            if (transformed_index < negativeSpace.length)
            {
                return &negativeSpace[transformed_index];
            }
            else
            {
                while (transformed_index >= negativeSpace.length)
                {
                    negativeSpace ~= 0;
                }
                return &negativeSpace[index];
            }
        }
    }

    int opIndex(int idx) 
    {
        return *addressOf(idx);
    }

    void opIndexAssign(int rhs, int idx) 
    {
        *addressOf(idx) = rhs;
    }
}

