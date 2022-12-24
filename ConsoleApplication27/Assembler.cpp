//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}  
// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler( )
{
}
// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::ST_Comment )  
        {
            continue;
        }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}
// Pass II generates a translation
void Assembler::PassII( )
{
    //  Initializes error reports.
    Errors::InitErrorReporting();

    //  Rewind the file access object to the beginning fo the file.
    m_facc.rewind();

    //  Print Header
    cout << "Location" << "    " << "Contents" << "    " << "Original Statement" << endl;

    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {
            Errors::RecordError(string("Missing end statement or the end statement is not the last one in the program."));
            Errors::DisplayErrors();

            break;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction( line );

        //  ================
        // If the instruction is a ST_Comment or ST_End, then only print out the Original Statement.
        if ( st == Instruction::ST_Comment || st == Instruction::ST_End )
        {
            cout << setw(55) << right << line << endl;

            if (st == Instruction::ST_End)
                break;

            Errors::DisplayErrors();
            continue;
        }

        // Length of label and operand must be between 1 and 10. first is a letter, remainder can be letters/numbers.
        if ( m_inst.isLabel() && m_inst.GetLabel().length() > 10 && !isalpha(m_inst.GetLabel()[0]) )
        {
            Errors::RecordError(string("Syntax error."));
        }
        if ( m_inst.isOperand() && m_inst.GetOperand().length() > 10 && !isalpha(m_inst.GetOperand()[0]) )
        {
            Errors::RecordError(string("Syntax error."));
        }

        // If the instruction is org or ds, print out only the location and original statement.
        if (m_inst.GetOpCode() == "org" || m_inst.GetOpCode() == "ds")
        {
            cout << setw(15) << right << loc << setw(40) << right << line << endl;
            loc = m_inst.LocationNextInstruction(loc);

            Errors::DisplayErrors();
            continue;
        }

        // If ST_Error, the instruction is illegal.
        if (st == Instruction::ST_Error)
        {
            Errors::RecordError(string("Illegal operation code."));
        }

        // Build the contents by having the first 2 digits be the OpCode and last 4 digits be the address portion.
        int content = m_inst.GetOpCodeNum() * 10000;

        if ( st == Instruction::ST_MachineLanguage )
        {
            if (m_inst.GetOpCode() == "halt")
            {
                content += 0;
            }
            else
            {
                int sym_loc;
                m_symtab.LookupSymbol(m_inst.GetOperand(), sym_loc);
                content += sym_loc;
            }
        }
        else if ( st == Instruction::ST_AssemblerInstr )
        {
            content += m_inst.GetOperandValue();
            if(m_inst.GetOperandValue()>9999)
                Errors::RecordError(string("Constant too large for VC407 memory."));
        }

        string cont = to_string(content);
        while (cont.length() != 6) cont = '0' + cont;

        // Output the location, contents, and original statement as well as any errors.
        cout << setw(15) << right << loc << setw(15) << right << cont << setw(25) << right << line << endl;
        Errors::DisplayErrors();
        loc = m_inst.LocationNextInstruction(loc);

        if ( !m_emul.insertMemory(loc, content) )
            Errors::RecordError(string("Insufficient memory."));
    }
}
// Run emulator on the translation
void Assembler::RunProgramInEmulator( )
{
    m_emul.runProgram();
}
