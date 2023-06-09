import std.stdio;
import std.file;
import vm.vm;
import compiler;

void showHelpLine(string helpLine, string progName)
{
    bool isEscaping = false;
    foreach (i; helpLine)
    {
        if (isEscaping)
        {
            switch (i)
            {
                case 'N':
                    write(progName);
                    break;
                case '&':
                    write('&');
                    break;
                default:
                    write("&", i);
                    break;
            }
            isEscaping = false;
        }
        else
        {
            if (i == '&')
            {
                isEscaping = true;
            }
            else
            {
                write(i);
            }
        }
    }
    writeln();
}

void showHelp(string progName) 
{
    const string[] helpMessage = [
        "Usage: ",
        "&N [source_file]",
        "&N [source_file] [output_file]",
        "Description: ",
        "If one argument is given, run the [source_file] as a Brainfuck source file. It will interprete the content in that file as a Brainfuck source. ",
        "If two arguments are given, it will compile the [source_file] and output the result to the [output_file]. ",
    ];
    foreach (i; helpMessage) {
        showHelpLine(i, progName);
    }
}

void main(string[] args)
{
    if (args.length <= 1)
    {
        showHelp(args[0]);
    }
    else
    {
        // TODO: Main logic here.
        try {
            Program p = compile(args[1]); 
            if (args.length == 2)
            {
                VM vm = new VM(p);
                vm.run();
            }
            else
            {
                File f = File(args[2], "w");
                f.writeln(p);
            }
        } catch (Exception e) {
            stderr.writeln("Runtime error ocurred: ", e);
        }
    }
}
