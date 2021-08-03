// ZxImage.h: interface for the CZxImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZXIMAGE_H__577AAEE7_7EAF_4C4D_B664_9B1382F123D5__INCLUDED_)
#define AFX_ZXIMAGE_H__577AAEE7_7EAF_4C4D_B664_9B1382F123D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#include "typedefs.h"
#include "RGBAImage.h"

class CZxImage  
{
public:
	friend std::ostream& operator<< (std::ostream& ost, const CZxImage& zxi);
	ProcessRGBAImage(CRGBAImage &_rgba);
	void InvertMask(bool _invert = true) {m_bMaskInvert = _invert;};
	void SetMaskFormat(ZXIMAGEFORMAT _format) {m_nMaskFormat = _format;};
	void SetGreyThreshold(int _grey) {if (_grey >= 0 && _grey <= 255) m_nGreyThreshold = _grey;};
	void SetMaskThreshold(int _mask) {if (_mask >= 0 && _mask <= 255) m_nMaskThreshold = _mask;};
	void SetTextOutput(bool _text = true) {m_bTextOutput = _text;};
	void SetReverseOutput(bool _rev = true) {m_bReverse = _rev;};
	void SetUpsideDown(bool _usd = true) {m_bUpsideDown = _usd;};
	void SetZigZag(bool _zz = true) {m_bZigZag = _zz;};
	CZxImage(POINT _size);
	virtual ~CZxImage();

	class CExUnknownMask : public std::runtime_error {public: CExUnknownMask(const std::string &what_arg) : std::runtime_error(what_arg) {};};

protected:
	ZXIMAGEFORMAT m_nMaskFormat;
	bool m_bMaskInvert;
	int m_nGreyThreshold;
	int m_nMaskThreshold;
	bool m_bTextOutput;
	bool m_bReverse;
	bool m_bUpsideDown;
	bool m_bZigZag;
	POINT m_ptSize;

	std::vector <std::vector<ucBYTE> > m_imgByte;
	std::vector <std::vector<ucBYTE> > m_imgMask;
};

#endif // !defined(AFX_ZXIMAGE_H__577AAEE7_7EAF_4C4D_B664_9B1382F123D5__INCLUDED_)
