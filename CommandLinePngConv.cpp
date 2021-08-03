// CommandLinePngConv.cpp: implementation of the CommandLinePngConv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pngconv.h"
#include "CommandLinePngConv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandLinePngConv::CCommandLinePngConv()
: m_bSilent(false)
, m_bVersion(false)
, m_bInverseMask(false)
, m_nMaskFormat(ZXIMAGE_FORMAT_DEFAULT)
, m_bStdOut(false)
, m_bTxtOut(false)
, m_bZigZag(false)
, m_bReverse(false)
, m_bUpsideDown(false)
{
	m_pOrigin.x = m_pOrigin.y = 0;
	m_pSize.x = m_pSize.y = -1;
}

CCommandLinePngConv::~CCommandLinePngConv()
{

}

void CCommandLinePngConv::ParseParam(std::string param, bool bFlag, bool bLast)
{
	// now do the specialist stuff...
	if (bFlag)
	{
		// first we'll convert the parameter to lower case for our comparisons...
		// go through the string char by char and lower it...
		for (std::string::iterator its = param.begin(); its != param.end(); ++its)
		{
			*its = tolower(*its);
		}

		// parse the flags
		if (!param.compare("help") || !param.compare("h"))
			m_bDisplayHelp = true;
		else if (!param.compare("ss"))
			m_bSilent = true;
		else if (!param.compare("ver"))
			m_bVersion = true;
		else if (!param.compare(0, 4, "pos="))
			m_pOrigin = DecodeXY(param.substr(4));
		else if (!param.compare(0, 5, "size="))
			m_pSize = DecodeXY(param.substr(5));
		else if (!param.compare("imask"))
			m_bInverseMask = true;
		else if (!param.compare(0, 5, "mask="))
			m_nMaskFormat = DecodeMask(param.substr(5));
		else if (!param.compare("ostd"))
			m_bStdOut = m_bSilent = true;
		else if (!param.compare("otxt"))
			m_bTxtOut = true;
		else if (!param.compare("obin"))
			m_bTxtOut = false;
		else if (!param.compare("zz"))
			m_bZigZag = true;
		else if (!param.compare("rtl"))
			m_bReverse = true;
		else if (!param.compare("ltr"))
			m_bReverse = false;
		else if (!param.compare("usd"))
			m_bUpsideDown = true;
		else
			m_bCommandLineError = true;
	}
	else
	{
		// parse the non-flags
		if (m_sPngFileName.empty())
		{
			m_sPngFileName = param;
		}
		else if (m_sOutFileName.empty())
		{
			m_sOutFileName = param;
		}
		else
		{
			m_bCommandLineError = true;
		}
	}
}

std::string CCommandLinePngConv::Help()
{
	std::string help;
	// add our params to the list...
	help  = "Usage:\n  " + m_sAppName + " [-h|-help] [-ver] [-ss] [-pos=x,y] [-size=x,y] [-imask] [-mask=<mask>] [-ostd] [-otxt|-obin] [-lrt|rtl] [-zz] [-usd] input <output>";

	help += "\n\nFlags:";

	help += "\n  -h / -help      Display Help";
	help += "\n  -ver            Display Version information only";
	help += "\n  -ss             Silent operation";
	help += "\n  -pos=x,y        Origin of conversion image, may be negative 0,0 = top left";
	help += "\n  -size=x,y       Size of conversion image in pixels";
	help += "\n  -imask          Inverse mask. Default is Pixel On";
	help += "\n  -mask=<mask>    Mask format - one of:";
	help += "\n                    b    : Output only Image (default)";
	help += "\n                    m    : Output only Mask";
	help += "\n                    mb   : Alternate Mask and Image bytes";
	help += "\n                    bm   : Alternate Image and Mask bytes";
	help += "\n                    mmbb : Alternate Mask and Image lines";
	help += "\n                    bbmm : Alternate Image and Mask lines";
	help += "\n  -ostd           Send output to stdout rather than a file";
	help += "\n                  This option implies -ss to remove junk from output";
	help += "\n  -otxt           Output as text";
	help += "\n  -obin           Output as binary (default)";
	help += "\n  -ltr|rtl        Set output left-to-right (default) or right-to-left";
	help += "\n  -zz             ZigZag Output (alternate ltr, rtl)";
	help += "\n  -usd            Upside down (output bottom line first)";
	help += "\n  input           Input PNG File";
	help += "\n  output          Output File. Defaults to \"input.out\" if not given.";

	return help;
}

POINT CCommandLinePngConv::DecodeXY(std::string param)
{
	POINT pt = m_pOrigin;

	// ensure we only have digits and commas:
	// actually, allow negative numbers too, to allow a negative origin.
	if (param.find_first_not_of("0123456789,-") != std::string::npos)
	{
		m_bCommandLineError = true;
	}

	// check number of points
	int comma = param.find_first_of(",");
	if (std::string::npos == comma || (comma != param.find_last_of(",")))
	{
		m_bCommandLineError = true;
	}
	else
	{
		// get the two substrings
		std::string sx = param.substr(0, comma);
		std::string sy = param.substr(++comma);

		pt.x = atoi(sx.c_str());
		pt.y = atoi(sy.c_str());
	}

	return pt;
}

ZXIMAGEFORMAT CCommandLinePngConv::DecodeMask(std::string param)
{
	ZXIMAGEFORMAT retval = ZXIMAGE_FORMAT_DEFAULT;

	// check the various options...
	if (!param.compare("b"))
		retval = ZXIMAGE_FORMAT_B;
	else if (!param.compare("m"))
		retval = ZXIMAGE_FORMAT_M;
	else if (!param.compare("mb"))
		retval = ZXIMAGE_FORMAT_MB;
	else if (!param.compare("bm"))
		retval = ZXIMAGE_FORMAT_BM;
	else if (!param.compare("mmbb"))
		retval = ZXIMAGE_FORMAT_MMBB;
	else if (!param.compare("bbmm"))
		retval = ZXIMAGE_FORMAT_BBMM;
	else m_bCommandLineError = true;

	return retval;
}