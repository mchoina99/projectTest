//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction( ) { };
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?  We should make this an enum class.  We will do this during a lecture.
    enum InstructionType {
        ST_MachineLanguage,     // A machine language instruction.
        ST_AssemblerInstr,      // Assembler Language instruction.
        ST_Comment,             // Comment or blank line
        ST_End,                 // End instruction.
        ST_Error,               // Error instruction.
        ST_Empty                // Empty instruction.
    };
    // Parse the Instruction.
    InstructionType ParseInstruction( string a_line );

    // Compute the location of the next instruction.
    int LocationNextInstruction( int a_loc );

    // Determine if Operand exists.
	inline bool isOperand() { return !m_Operand.empty(); };

    // To access the label
    inline string &GetLabel( ) {

        return m_Label;
    };

    // To access the OpCode
    inline string &GetOpCode( ) {
        return m_OpCode;
    }

    // Return Number of OpCode.
	inline int GetOpCodeNum() { return m_NumOpCode; };

    // To access the Operand
    inline string &GetOperand( ) {
        return m_Operand;
    }

    // To determine if a label is blank.
    inline bool isLabel( ) {

        return ! m_Label.empty();
    };

    // Return Operand Value
	inline int GetOperandValue() { return m_OperandValue; };


private:


    // The elemements of a instruction
    string m_Label;        // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand;      // The operand. 

    string m_instruction;    // The original instruction.

    // Derived values.
    int m_NumOpCode;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type; // The type of instruction.

    bool m_IsNumericOperand;// == true if the operand is numeric.
    int m_OperandValue;    // The value of the operand if it is numeric.
    
};

