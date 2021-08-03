// RGBAImage.cpp: implementation of the CRGBAImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pngconv.h"
#include "RGBAImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRGBAImage::CRGBAImage(POINT size)
: m_size(size)
, m_image(size.y)
{
	// in which to store our image of size (x,y)

	if (m_size.x <= 0 || m_size.y <= 0)
	{
		std::ostringstream except;
		except << "(" << m_size.x << "," << m_size.y << ")";
		throw CExBadSize(except.str());
	}
	for (unsigned int i = 0; i < m_size.y; ++i)
		m_image[i].resize(m_size.x);
}

CRGBAImage::~CRGBAImage()
{
}

RGBA& CRGBAImage::operator () (unsigned int row, unsigned int col)
{
	if (row >= nrows() || col >= ncols())
	{
		std::ostringstream except;
		except << "(" << col << "," << row << ")";
		throw CExBoundsViolation(except.str());
	}
	return m_image[row][col];
}

const RGBA& CRGBAImage::operator () (unsigned int row, unsigned int col) const 
{
	if (row >= nrows() || col >= ncols())
	{
		std::ostringstream except;
		except << "(" << col << "," << row << ")";
		throw CExBoundsViolation(except.str());
	}
	return m_image[row][col];
}

CRGBAImage::ExtractImageData(const CPngImage &png, POINT origin)
{
	// here we want to extract the image data from the PNG
	// and put it into our vectors of RGBA.
	for (unsigned int yy = 0; yy < nrows(); ++yy)
	{
		// get a line of data from the PNG image
		for (unsigned int xx = 0; xx < ncols(); ++xx)
		{
			RGBA rgba = png.GetRGBA(xx + origin.x, yy + origin.y);
			m_image[yy][xx] = rgba;
		}
	}
}
