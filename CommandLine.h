// CommandLine.h: interface for the CCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDLINE_H__68C36EE4_158B_4989_9B41_59496298363F__INCLUDED_)
#define AFX_COMMANDLINE_H__68C36EE4_158B_4989_9B41_59496298363F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

class CCommandLine  
{
public:
	int ParseCommandLine(int argc, char* argv[]);
	CCommandLine();
	virtual ~CCommandLine();
protected:
	virtual void ParseParam(std::string param, bool bFlag, bool bLast);
	bool m_bCommandLineError;

public:
	virtual std::string Help();
	std::string m_sAppName;
	bool m_bDisplayHelp;
	int m_nCommandLineError;
};

#endif // !defined(AFX_COMMANDLINE_H__68C36EE4_158B_4989_9B41_59496298363F__INCLUDED_)
