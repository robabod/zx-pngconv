// RGBAImage.h: interface for the CRGBAImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RGBAIMAGE_H__435DB1EA_11F2_4E3C_ADF3_7871583FBF64__INCLUDED_)
#define AFX_RGBAIMAGE_H__435DB1EA_11F2_4E3C_ADF3_7871583FBF64__INCLUDED_

#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PngImage.h"
#include "typedefs.h"

class CRGBAImage  
{
public:
	CRGBAImage(POINT size);
	virtual ~CRGBAImage();

	RGBA&       operator() (unsigned int i, unsigned int j);
	const RGBA& operator() (unsigned int i, unsigned int j) const;

	unsigned int nrows() const;
	unsigned int ncols() const;

	ExtractImageData(const CPngImage &png, POINT origin);

protected:
	POINT m_size;
	std::vector <std::vector<RGBA> > m_image;

public:
	class CExBoundsViolation : public std::runtime_error {public: CExBoundsViolation(const std::string &what_arg) : std::runtime_error(what_arg) {};};
	class CExBadSize : public std::runtime_error {public: CExBadSize(const std::string &what_arg) : std::runtime_error(what_arg) {};};
};

inline unsigned int CRGBAImage::nrows() const {return m_image.size();}
inline unsigned int CRGBAImage::ncols() const {return m_image[0].size();}

#endif // !defined(AFX_RGBAIMAGE_H__435DB1EA_11F2_4E3C_ADF3_7871583FBF64__INCLUDED_)
