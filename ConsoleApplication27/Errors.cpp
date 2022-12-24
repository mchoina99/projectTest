//
//      Implementation of the Error class.
//

#include "stdafx.h"
#include "Errors.h"

//  Initializes error reports.
void Errors::InitErrorReporting( )
{
    while (!m_ErrorMsgs.empty())
    {
        m_ErrorMsgs.pop_back();
    }
}

//  Records an error message.
void Errors::RecordError( string a_emsg )
{
    m_ErrorMsgs.push_back( a_emsg );
}

//  Displays the collected error message.
void Errors::DisplayErrors( )
{
    // Get details about the console window.
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(Console, &Info);
	WORD Attributes = Info.wAttributes;

	// Set the text color to red.
	SetConsoleTextAttribute(Console, FOREGROUND_RED | FOREGROUND_INTENSITY);
	
	// While there are any error messages, print them to the screen.
	while (!m_ErrorMsgs.empty())
	{
		cout << setw(15) << right << m_ErrorMsgs.front() << endl;
		m_ErrorMsgs.pop_back();
	}

	// Return the console window to normal settings.
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}
