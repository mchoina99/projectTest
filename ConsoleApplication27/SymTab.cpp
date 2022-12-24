//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "Errors.h"
#include "SymTab.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );
    	a_symbol	-> The name of the symbol to be added to the symbol table.
    	a_loc		-> the location to be associated with the symbol.

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void 
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

void SymbolTable::DisplaySymbolTable( )
{
    int numSymbols = 0;

    cout << "Symbol #" << "     " << "Symbol" << "     " << "Location" << endl;

    //  Iterate to display the Symbol Table
    map<string, int>::iterator it;
    for ( it = m_symbolTable.begin(); it != m_symbolTable.end(); it++ ) {
        cout << right << setw(8) << numSymbols
                      << setw(13) << it->first
                      << setw(13) << it->second << endl;
        numSymbols++;
    }
}

bool SymbolTable::LookupSymbol( const string &a_symbol, int &a_loc )
{
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if ( st != m_symbolTable.end() ) {
        //  Lookup the location in the symbol table.
        a_loc = m_symbolTable[a_symbol];
        return true;
    } else {
        Errors::RecordError(string("Undefined label."));
    }
}
