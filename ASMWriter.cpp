#include "ASMWriter.hpp"

extern string moduleName;

extern string fnameIn;

bool flag=1;


ASMWritter::ASMWritter()
{
    ss_ASM.str(string());
    moduleName = "fnameIn";
    symbolCounter = 0;
    map_segments.clear();
    map_segments.insert(pair<string, VMsegments>("constant", seg_constant));
    map_segments.insert(pair<string, VMsegments>("static", seg_static));
    map_segments.insert(pair<string, VMsegments>("temp", seg_temp));
    map_segments.insert(pair<string, VMsegments>("pointer", seg_pointer));
    map_segments.insert(pair<string, VMsegments>("local", seg_local));
    map_segments.insert(pair<string, VMsegments>("argument", seg_argument));
    map_segments.insert(pair<string, VMsegments>("this", seg_this));
    map_segments.insert(pair<string, VMsegments>("that", seg_that));

      //ss_ASM << writeBootstrap();
}

ASMWritter::~ASMWritter()
{
    map_segments.clear();
}

/** Generate Hack Assembly code for a VM add operation assessed in Project 07 */
string ASMWritter::vm_add()
{
    ss_ASM.str(string());
    write("@SP // add");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D+M");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM sub operation assessed in Project 07 */
string ASMWritter::vm_sub()
{
    ss_ASM.str(string());
    write("@SP // sub");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=M-D");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM neg operation assessed in Project 07 */
string ASMWritter::vm_neg()
{
    ss_ASM.str(string());
    write("@SP // neg");
    write("A=M-1");
    write("M=-M");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM eq operation assessed in Project 07 */
string ASMWritter::vm_eq()
{
    ss_ASM.str(string());

    // Generate unique labels for true/false cases
    string labelTrue = moduleName + "_EQ_TRUE_" + to_string(symbolCounter);
    string labelFalse = moduleName + "_EQ_FALSE_" + to_string(symbolCounter);

    // Pop y (top of stack) into D
    write("@SP // eq");
    write("AM=M-1");       // SP--
    write("D=M");          // D = *SP

    // Pop x (next value on stack) into M
    write("@SP");
    write("AM=M-1");       // SP--
    write("D=M-D");        // D = x - y

    // Jump to true/false labels based on comparison result
    write("@" + labelTrue);
    write("D;JEQ");        // Jump if x == y (D == 0)
    
    // False case (push 0 onto the stack)
    write("@SP");
    write("A=M");
    write("M=0");          // *SP = 0 (false)
    write("@" + labelFalse);
    write("0;JMP");        // Jump to false label

    // True case (push -1 onto the stack)
    write("(" + labelTrue + ")");
    write("@SP");
    write("A=M");
    write("M=-1");         // *SP = -1 (true)

    // False label
    write("(" + labelFalse + ")");
    
    // Increment SP
    write("@SP");
    write("M=M+1");

    symbolCounter++;       // Increment symbol counter for unique labels
    return ss_ASM.str() + "\n";
}


/** Generate Hack Assembly code for a VM gt operation assessed in Project 07 */
string ASMWritter::vm_gt()
{
    ss_ASM.str(string());

    // Generate unique labels for true/false cases
    string labelTrue = moduleName + "_GT_TRUE_" + to_string(symbolCounter);
    string labelFalse = moduleName + "_GT_FALSE_" + to_string(symbolCounter);

    // Pop y (top of stack) into D
    write("@SP // gt");
    write("AM=M-1");       // SP--
    write("D=M");          // D = *SP

    // Pop x (next value on stack) into M
    write("@SP");
    write("AM=M-1");       // SP--
    write("D=M-D");        // D = x - y

    // Jump to true/false labels based on comparison result
    write("@" + labelTrue);
    write("D;JGT");        // Jump if x > y (D > 0)

    // False case (push 0 onto the stack)
    write("@SP");
    write("A=M");
    write("M=0");          // *SP = 0 (false)
    write("@" + labelFalse);
    write("0;JMP");        // Jump to false label

    // True case (push -1 onto the stack)
    write("(" + labelTrue + ")");
    write("@SP");
    write("A=M");
    write("M=-1");         // *SP = -1 (true)

    // False label
    write("(" + labelFalse + ")");
    
    // Increment SP
    write("@SP");
    write("M=M+1");

    symbolCounter++;       // Increment symbol counter for unique labels
    return ss_ASM.str() + "\n";
}


/** Generate Hack Assembly code for a VM lt operation assessed in Project 07 */
string ASMWritter::vm_lt()
{
    ss_ASM.str(string());

    // Generate unique labels for true/false cases
    string labelTrue = moduleName + "_LT_TRUE_" + to_string(symbolCounter);
    string labelFalse = moduleName + "_LT_FALSE_" + to_string(symbolCounter);

    // Pop y (top of stack) into D
    write("@SP // lt");
    write("AM=M-1");       // SP--
    write("D=M");          // D = *SP

    // Pop x (next value on stack) into M
    write("@SP");
    write("AM=M-1");       // SP--
    write("D=M-D");        // D = x - y

    // Jump to true/false labels based on comparison result
    write("@" + labelTrue);
    write("D;JLT");        // Jump if x < y (D < 0)

    // False case (push 0 onto the stack)
    write("@SP");
    write("A=M");
    write("M=0");          // *SP = 0 (false)
    write("@" + labelFalse);
    write("0;JMP");        // Jump to false label

    // True case (push -1 onto the stack)
    write("(" + labelTrue + ")");
    write("@SP");
    write("A=M");
    write("M=-1");         // *SP = -1 (true)

    // False label
    write("(" + labelFalse + ")");
    
    // Increment SP
    write("@SP");
    write("M=M+1");

    symbolCounter++;       // Increment symbol counter for unique labels
    return ss_ASM.str() + "\n";
}



/** Generate Hack Assembly code for a VM and operation assessed in Project 07 */
string ASMWritter::vm_and()
{
    ss_ASM.str(string());
    write("@SP // and");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D&M");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM or operation assessed in Project 07 */
string ASMWritter::vm_or()
{
    ss_ASM.str(string());
    write("@SP // or");
    write("AM=M-1");
    write("D=M");
    write("A=A-1");
    write("M=D|M");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM not operation assessed in Project 07 */
string ASMWritter::vm_not()
{
    ss_ASM.str(string());
    write("@SP // not");
    write("A=M-1");
    write("M=!M");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM push operation assessed in Project 07 */
string ASMWritter::vm_push(string segment, int offset)
{
    ss_ASM.str(string());
    string indexStr = to_string(offset); // Convert offset to string
    string registerStr = registerName(segment, offset);

    // Map VM segments to Hack assembly segments
    map<string, string> mapping = {
        {"local", "@LCL"},
        {"argument", "@ARG"},
        {"this", "@THIS"},
        {"that", "@THAT"}
    };

    if (segment == "constant")
    {
        // Push constant value onto the stack
        write("@" + indexStr + " // push constant " + indexStr);
        write("D=A");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
    }
    else if (segment == "static")
    {
        // Push static variable value onto the stack
        write("@" + moduleName + "." + indexStr + " // push static " + segment + " " + indexStr);
        write("D=M");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
    }
    else if (segment == "temp" || segment == "pointer")
    {
        // Validate offset for temp/pointer segments
        if (offset > 7)
            throw runtime_error("Invalid location for segment: " + segment);

        int baseAddress = (segment == "temp") ? 5 : 3; // Temp starts at R5, Pointer starts at R3
        write("@R" + to_string(baseAddress + offset) + " // push " + segment + " " + indexStr);
        write("D=M");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
    }
    else if (mapping.find(segment) != mapping.end())
    {
        // Push value from local/argument/this/that segments onto the stack
        write(mapping[segment] + " // push " + segment + " " + indexStr);
        write("D=M");         // D = base address of the segment
        write("@" + indexStr);
        write("A=D+A");       // A = base_address + offset
        write("D=M");         // D = *(base_address + offset)
        write("@SP");
        write("A=M");         // Access top of stack
        write("M=D");         // *SP = D
        write("@SP");
        write("M=M+1");       // Increment SP
    }
    else
    {
        throw runtime_error(segment + ": Invalid memory segment in vm_push()");
    }

    return ss_ASM.str() + "\n";
}

string ASMWritter::vm_pop(string segment, int offset)
{
    ss_ASM.str(string());
    string indexStr = to_string(offset);

    // Map VM segments to Hack assembly segments
    map<string, string> mapping = {
        {"local", "@LCL"},
        {"argument", "@ARG"},
        {"this", "@THIS"},
        {"that", "@THAT"}
    };

    if (segment == "constant")
    {
        throw runtime_error("vm_pop(): Cannot pop to constant segment.");
    }
    else if (segment == "static")
    {
        // Pop value into static variable
        write("@" + moduleName + "." + indexStr + " // pop static " + indexStr);
        write("D=A");           // D = static variable address
        write("@R13");          // Store address in R13
        write("M=D");
        write("@SP");           // SP--
        write("AM=M-1");
        write("D=M");           // D = *SP
        write("@R13");
        write("A=M");
        write("M=D");           // *addr = D = *SP
    }
    else if (segment == "temp" || segment == "pointer")
    {
        // Validate offset for temp/pointer segments
        if (offset > 7)
            throw runtime_error("Invalid offset for " + segment + " segment: " + indexStr);

        int baseAddress = (segment == "temp") ? 5 : 3; // Temp starts at R5, Pointer at R3
        string targetAddress = to_string(baseAddress + offset);

        write("@SP               // pop " + segment + " " + indexStr);
        write("AM=M-1");         // SP--
        write("D=M");            // D = *SP
        write("@R" + targetAddress);
        write("M=D");            // R(base+i) = *SP
    }
    else if (mapping.find(segment) != mapping.end())
    {
        // Pop value into local/argument/this/that segments
        write(mapping[segment] + " // pop " + segment + " " + indexStr);
        write("D=M");            // D = base address
        write("@" + indexStr);   // Load offset
        write("D=D+A");          // D = base + offset (target address)
        write("@R13");           // Store target address in R13
        write("M=D");
        write("@SP");            // SP--
        write("AM=M-1");
        write("D=M");            // D = *SP
        write("@R13");
        write("A=M");
        write("M=D");            // *addr = D = *SP
    }
    else
    {
        throw runtime_error("Invalid segment: " + segment);
    }

    return ss_ASM.str() + "\n";
}


/** Generate Hack Assembly code for a VM label operation assessed in Project 08 */
string ASMWritter::vm_label(string label)
{    static int cnt=0;
    ss_ASM.str(string());
    if(label=="END") flag=0;
    write("(" + moduleName+ label+")"+" // label " + label);
    return ss_ASM.str()+"\n";
}

/** Generate Hack Assembly code for a VM goto operation assessed in Project 08 */
string ASMWritter::vm_goto(string label)
{
    ss_ASM.str(string());
    write("@" + moduleName + label +"  // goto " + label);
    write("0;JMP");
    return ss_ASM.str() + "\n";
}

/** Generate Hack Assembly code for a VM if-goto operation assessed in Project 08 */
string ASMWritter::vm_if_goto(string label)
{
    ss_ASM.str(string());
    write("@SP // if-goto " + label);
    write("M=M-1");
    write("A=M");
    write("D=M");
   write("@" + moduleName + label);
    write("D;JNE");
    return ss_ASM.str() + "\n";
}

string ASMWritter::vm_function(string function_name, int n_vars)
{
    // Clear the stringstream to prepare for new assembly code
    ss_ASM.str(string());

    // Define the function label
    write("(" +function_name + ")");

    // If n_vars > 0, initialize local variables by pushing zeros onto the stack
    if (n_vars > 0)
    {
        write("// Initialize " + to_string(n_vars) + " local variables to 0");
        for (int i = 0; i < n_vars; i++)
        {
            write("@0");          // Load constant 0
            write("D=A");         // D = 0
            write("@SP");         // Access stack pointer
            write("A=M");         // Point to top of stack
            write("M=D");         // *SP = 0
            write("@SP");         // Increment stack pointer
            write("M=M+1");
        }
    }

    // Return the generated assembly code as a string
    return ss_ASM.str() + "\n";
}




/** Generate Hack Assembly code for a VM call operation assessed in Project 08 */
string ASMWritter::vm_call(string function_name, int n_args)
{
    static int call_counter = 0; // Unique counter for return addresses
    ss_ASM.str(string());
    
    // Generate a unique return label
    string return_label = function_name + ".RET_" + to_string(call_counter++);

    // Push return address onto the stack
    write("@" + return_label + " // call " + function_name + " " + to_string(n_args));
    write("D=A");
    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");

    // Save caller's frame (LCL, ARG, THIS, THAT)
    for (const string& segment : {"LCL", "ARG", "THIS", "THAT"})
    {
        write("@" + segment);
        write("D=M");
        write("@SP");
        write("A=M");
        write("M=D");
        write("@SP");
        write("M=M+1");
    }

    // Reposition LCL (LCL = SP)
    write("@SP");
    write("D=M");
    write("@LCL");
    write("M=D");

    // Reposition ARG (ARG = SP - 5 - n_args)
    write("@SP");            // Load SP into D
    write("D=M");            
    write("@5");             // Subtract 5
    write("D=D-A");
    write("@" + to_string(n_args)); // Subtract n_args
    write("D=D-A");
    write("@ARG");           // Store in ARG
    write("M=D");

    // Jump to the function
    write("@" + function_name);
    write("0;JMP");

    // Declare the return label
    write("(" + return_label + ")");

    return ss_ASM.str() + "\n";
}




/** Generate Hack Assembly code for a VM return operation assessed in Project 08 */
string ASMWritter::vm_return()
{
    ss_ASM.str(string());

    // FRAME = LCL
    write("@LCL // return");
    write("D=M");
    write("@R14");          // Use R14 to store FRAME
    write("M=D");

    // RET = *(FRAME - 5)
    write("@5");
    write("A=D-A");         // A = FRAME - 5
    write("D=M");           // D = *(FRAME - 5)
    write("@R15");          // Use R15 to store RET
    write("M=D");

    // *ARG = pop()
    write("@SP");
    write("AM=M-1");        // SP--
    write("D=M");           // D = *SP (value popped from stack)
    write("@ARG");
    write("A=M");           // A = ARG (address to store return value)
    write("M=D");

    // SP = ARG + 1
    write("@ARG");
    write("D=M");
    write("@SP");
    write("M=D+1");

    // Restore THAT, THIS, ARG, LCL from FRAME
    for (const string& segment : {"THAT", "THIS", "ARG", "LCL"})
    {
        write("@R14");
        write("AMD=M-1");   // FRAME--
        write("D=M");       // D = *(FRAME - (i+1))
        write("@" + segment);
        write("M=D");       // Restore segment
    }

    // Jump to RET
    write("@R15");
    write("A=M");           // A = RET
    write("0;JMP");

    return ss_ASM.str() + "\n";
}



/** Generate Hack Assembly code for a VM not operation assessed in Project 07 */
string ASMWritter::vm_end()
{   
    if(flag==1){
    ss_ASM.str(string());
    write("(END) // END loop");
    write("@END");
    write("0;JMP");
    return ss_ASM.str() + "\n";
    }
    return "";
}

void ASMWritter::write(string vmCode)
{
    if (vmCode.find("(") == string::npos)
        ss_ASM << "\t"; // put TAB if the ASM command is not a label
    ss_ASM << vmCode << endl;
}

string ASMWritter::registerName(string segment, int index)
{
    if (segment == "static")
        return "16";
    if (segment == "local")
        return "LCL";
    if (segment == "argument")
        return "ARG";
    if (segment == "this")
        return "THIS";
    if (segment == "that")
        return "THAT";
    if (segment == "pointer")
        return "R" + to_string( 3 + index);
    if (segment == "temp")
        return "R" + to_string(5 + index);

    return moduleName + "." + to_string(index); // else it is static
}