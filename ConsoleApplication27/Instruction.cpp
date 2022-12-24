//
//      Implementation of the Instruction class.
//

#include "stdafx.h"
#include "Instruction.h"
#include "Errors.h"

Instruction::InstructionType Instruction::ParseInstruction( string a_line )
{
    m_Label = "";
    m_OpCode = "";
    m_Operand = "";

    m_instruction = "";

    m_NumOpCode = 0;

    m_IsNumericOperand = true;
    m_OperandValue = 0;

    if ( a_line.empty() )
        return ST_Empty;

    if ( a_line.find(";") != string::npos )
    {
        if ( a_line.at(0) == ';' )
            return ST_Comment;

        a_line.erase( a_line.find(";"), a_line.length() );
    }

    m_instruction = a_line;
    istringstream line(m_instruction);
    string instr_buf;

    stack<string> instruction;

    while ( line )
    {
        if ( !instr_buf.empty() )
        {
            instruction.push( instr_buf );
        }
        line >> instr_buf;
    }

    if ( instruction.size() == 1 )
    {
        m_OpCode = instruction.top();
        instruction.pop();

        if ( m_OpCode == "end" )
            return ST_End;
    }
    else if ( instruction.size() == 2 )
    {
        m_Operand = instruction.top();

        try
        {
            m_OperandValue = stoi(instruction.top());
        }
        catch (invalid_argument&)
        {
            m_IsNumericOperand = false;
        }

        instruction.pop();

        m_OpCode = instruction.top();
        instruction.pop();
    }
    else if ( instruction.size() == 3 )
    {
        m_Operand = instruction.top();
        try
        {
            m_OperandValue = stoi(instruction.top());
        }
        catch (invalid_argument&)
        {
            m_IsNumericOperand = false;
        }

        instruction.pop();

        m_OpCode = instruction.top();
        instruction.pop();

        m_Label = instruction.top();
        instruction.pop();
    }
    else if ( instruction.size() > 3 )
    {
        Errors::RecordError(string("Syntax Error"));
        return ST_Error;
    }

    transform(m_OpCode.begin(), m_OpCode.end(), m_OpCode.begin(), ::tolower);
    transform(m_Label.begin(), m_Label.end(), m_Label.begin(), ::tolower);

    if ( m_OpCode == "add" ) {
        m_NumOpCode = 1;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "sub" ) {
        m_NumOpCode = 2;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "mult" ) {
        m_NumOpCode = 3;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "div" ) {
        m_NumOpCode = 4;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "load" ) {
        m_NumOpCode = 5;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "store" ) {
        m_NumOpCode = 6;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "read" ) {
        m_NumOpCode = 7;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "write" ) {
        m_NumOpCode = 8;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "b" ) {
        m_NumOpCode = 9;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "bm" ) {
        m_NumOpCode = 10;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "bz" ) {
        m_NumOpCode = 11;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "bp" ) {
        m_NumOpCode = 12;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "halt" ) {
        m_NumOpCode = 13;
        return ST_MachineLanguage;
    } else if ( m_OpCode == "dc" ) {
        return ST_AssemblerInstr;
    } else if ( m_OpCode == "ds" ) {
        return ST_AssemblerInstr;
    } else if ( m_OpCode == "org" ) {
        return ST_AssemblerInstr;
    } else if ( m_OpCode == "end" ) {
        return ST_AssemblerInstr;
    } else {
        Errors::RecordError(string("Illegal operation code."));
        return ST_Error;
    }
}

int Instruction::LocationNextInstruction( int a_loc )
{
    return ((m_OpCode == "org") || (m_OpCode == "ds")) ? a_loc + m_OperandValue : ++a_loc;
}
