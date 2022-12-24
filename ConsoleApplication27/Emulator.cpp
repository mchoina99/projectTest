//
//      Implementation of the Emulator class.
//
#include "stdafx.h"
#include "Emulator.h"

// Records instructions and data into simulated memory.
bool emulator::insertMemory( int a_location, long long a_contents )
{
    if ( a_location >= MEMSZ )
    {
        return false;
    }
    else
    {
        m_memory[a_location] = a_contents;
        return true;
    }
}

// Runs the program recorded in memory.
bool emulator::runProgram( )
{
    int op_code;    //  Operation Code
    int address;    //  Memory Address

    int dat_in;     //  Data Input

    //  The first instruction of a program is assumed to be at location 100.
    for (int loc = 100; loc < MEMSZ; loc++)
    {
        if (m_memory[loc])
        {
            op_code = m_memory[loc] / 10000;
            address = m_memory[loc] % 10000;

            //  Decode the Op-Code
            switch( op_code )
            {
                //  Add
                case 1:
                    //  ACC <-- c(ACC) + c(ADDR)
                    accumulator += m_memory[address];
                    break;
                //  Subtract
                case 2:
                    //  ACC <-- c(ACC) - c(ADDR)
                    accumulator -= m_memory[address];
                    break;
                //  Multiply
                case 3:
                    //  ACC <-- c(ACC) * c(ADDR)
                    accumulator *= m_memory[address];
                    break;
                //  Divide
                case 4:
                    //  ACC <-- c(ACC) / c(ADDR)
                    accumulator /= m_memory[address];
                    break;
                //  Load
                case 5:
                    //  ACC <-- c(ADDR)
                    accumulator = m_memory[address];
                    break;
                //  Store
                case 6:
                    //  c(ADDR) <-- c(ACC)
                    m_memory[address] = accumulator;
                    break;
                //  Read
                case 7:
                    //  A line is read and number on the line is placed in the specified address.
                    cout << "?";
                    cin >> dat_in;
                    m_memory[address] = dat_in;
                    break;
                //  Write
                case 8:
                    //  c(ADDR) is displayed
                    cout << m_memory[address] << endl;
                    break;
                //  Branch
                case 9:
                    //  go to ADDR for next instruction
                    loc = address;
                    break;
                //  Branch Minus
                case 10:
                    //  go to ADDR if c(ACC) < 0
                    if (accumulator < 0)
                    {
                        loc = address;
                    }
                    break;
                //  Branch Zero
                case 11:
                    //  go to ADDR if c(ACC) = 0
                    if (accumulator == 0)
                    {
                        loc = address;
                    }
                    break;
                //  Branch Positive
                case 12:
                    //  go to ADDR if c(ACC) > 0
                    if (accumulator > 0)
                    {
                        loc = address;
                    }
                    break;
                //  Halt
                case 13:
                    //  Terminate execution
                    return true;
                //  Error
                default:
                    cout << "..." << endl;
                    break;
            }
        }
        else
        {
            cout << "..." << endl;
        }
    }
    return true;
}
