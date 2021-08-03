// exceptions.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEEXCEPTION_H__EAE19611_5E3E_4BD3_AF6A_DE8CA8ABF0BB__INCLUDED_)
#define AFX_FILEEXCEPTION_H__EAE19611_5E3E_4BD3_AF6A_DE8CA8ABF0BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdexcept>

class CFileException : public std::runtime_error
{
public:
	CFileException(const std::string &what_arg) : std::runtime_error(what_arg) {};
};

#endif // !defined(AFX_FILEEXCEPTION_H__EAE19611_5E3E_4BD3_AF6A_DE8CA8ABF0BB__INCLUDED_)
