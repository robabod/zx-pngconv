// CommandLinePngConv.h: interface for the CommandLinePngConv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDLINEPNGCONV_H__99C5D50B_8E9B_4670_BD85_583F3142A3FD__INCLUDED_)
#define AFX_COMMANDLINEPNGCONV_H__99C5D50B_8E9B_4670_BD85_583F3142A3FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommandLine.h"
#include "typedefs.h"

class CCommandLinePngConv : public CCommandLine  
{
public:
	virtual void ParseParam(std::string param, bool bFlag, bool bLast);
	CCommandLinePngConv();
	virtual ~CCommandLinePngConv();

public:
	virtual std::string Help();
	std::string m_sPngFileName;
	std::string m_sOutFileName;
	std::string m_sLeadText;
	POINT m_pOrigin;
	POINT m_pSize;
	RGBA m_rgbaMaskColour;
	bool m_bUseRGBMask;
	bool m_bUpsideDown;
	bool m_bReverse;
	bool m_bZigZag;
	bool m_bStdOut;
	bool m_bTxtOut;
	bool m_bSilent;
	bool m_bVersion;
	bool m_bInverseByte;
	bool m_bInverseMask;
	ZXIMAGEFORMAT m_nMaskFormat;
protected:
	bool DecodeRGB3(std::string param);
	ZXIMAGEFORMAT DecodeMask(std::string param);
	POINT DecodeXY(std::string param);
};

#endif // !defined(AFX_COMMANDLINEPNGCONV_H__99C5D50B_8E9B_4670_BD85_583F3142A3FD__INCLUDED_)
