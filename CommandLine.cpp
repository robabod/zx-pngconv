// CommandLine.cpp: implementation of the CCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pngconv.h"
#include "CommandLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandLine::CCommandLine()
: m_bDisplayHelp(false)
, m_bCommandLineError(false)
, m_nCommandLineError(0)
{

}

CCommandLine::~CCommandLine()
{

}

void CCommandLine::ParseParam(std::string param, bool bFlag, bool bLast)
{
	if (bFlag)
	{
		if (!param.compare("help") || !param.compare("h"))
		{
			m_bDisplayHelp = true;
		}
	}
}

int CCommandLine::ParseCommandLine(int argc, char *argv[])
{
	// first things first, set the application name.
	std::string appname = argv[0];
	// find the last slash
	std::string::size_type pos = appname.find_last_of("\\/");
	if (std::string::npos == pos)
		pos = 0;
	else
		pos ++;

	m_sAppName = appname.substr(pos);

	// now step through the command line options.
	for (int i = 1; i < argc; i++)
	{
		char * pszParam = argv[i];
		bool bFlag = false;
		bool bLast = ((i + 1) == __argc);
		if ('-' == pszParam[0] || '/' == pszParam[0])
		{
			// remove flag specifier
			bFlag = true;
			++pszParam;
		}

		// it's a string, so quick convert...
		std::string param(pszParam);

		this->ParseParam(param, bFlag, bLast);

		if (m_bCommandLineError)
		{
			m_nCommandLineError = i;
			break;
		}
	}

	return m_nCommandLineError;
}

std::string CCommandLine::Help()
{
	std::string helptext;
	helptext = m_sAppName + " [-help | -h] : Display Help";
	return helptext;
}
