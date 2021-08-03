// ZxImage.cpp: implementation of the CZxImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pngconv.h"
#include "ZxImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <sstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZxImage::CZxImage(POINT _size)
: m_nMaskFormat(ZXIMAGE_FORMAT_DEFAULT)
, m_bMaskInvert(false)
, m_ptSize(_size)
, m_nGreyThreshold(128)
, m_nMaskThreshold(128)
, m_imgByte(_size.y)
, m_imgMask(_size.y)
, m_bTextOutput(false)
, m_bReverse(false)
, m_bUpsideDown(false)
, m_bZigZag(false)
{
	// correct the X size so it's a multiple of 8
	if (m_ptSize.x % 8)
	{
		m_ptSize.x += 8 - (m_ptSize.x % 8);
	}

	// set the x size so it's actually a bitmap size rather than pixel...
	// basically, divide by 8
	m_ptSize.x /= 8;

	for (unsigned int i = 0; i < m_ptSize.y; ++i)
	{
		m_imgByte[i].resize(m_ptSize.x);
		m_imgMask[i].resize(m_ptSize.x);
	}
}

CZxImage::~CZxImage()
{

}

CZxImage::ProcessRGBAImage(CRGBAImage &_rgba)
{
	// for now we'll iterate through our image,
	// and get the associated pixels from the _rgba image.

	for (int y_idx = 0; y_idx < m_ptSize.y; ++y_idx)
	{
		for (int x_idx = 0; x_idx < m_ptSize.x; ++x_idx)
		{
			// need to set each bit...
			ucBYTE bitmap = 0x80;
			for (int b_idx = 0; b_idx < 8; ++b_idx)
			{
				bool byteon = false;
				bool maskon = true;
				try
				{
					byteon = _rgba(y_idx, (8*x_idx) + b_idx).PixelIsOn(m_nGreyThreshold);
					maskon = _rgba(y_idx, (8*x_idx) + b_idx).PixelIsMasked(m_nMaskThreshold);
				}
				catch (CRGBAImage::CExBoundsViolation)
				{
				}

				if (byteon)
					m_imgByte[y_idx][x_idx] |= bitmap;
				if (maskon)
					m_imgMask[y_idx][x_idx] |= bitmap;

				bitmap >>= 1;
			}

			if (m_bMaskInvert)
				m_imgMask[y_idx][x_idx] = 255 - m_imgMask[y_idx][x_idx];
		}
	}
}

std::ostream& operator<< (std::ostream& ost, const CZxImage& zxi)
{
	// this is the operator that actually streams the contents out according to various set options.
	// it's a friend of the CZxImage class so it can get to all the private/protected stuff.
	// it means that externally we can just do something like:
	//   std::cout << myZxImage;
	// which is nice.

	// for the moment we only need to care about the following options:
	// m_nMaskFormat;
	// 	ZXIMAGE_FORMAT_B	= 0,	// no mask
	// 	ZXIMAGE_FORMAT_M	= 1,	// no byte
	// 	ZXIMAGE_FORMAT_MB	= 2,	// alternate mask then byte
	// 	ZXIMAGE_FORMAT_BM	= 3,	// alternate byte then mask
	// 	ZXIMAGE_FORMAT_MMBB	= 4,	// alternate lines of mask then byte
	// 	ZXIMAGE_FORMAT_BBMM	= 5		// alternate lines of byte then mask
	// m_bTextOutput;


	ZXIMAGEFORMAT zxf = zxi.m_nMaskFormat;

	int y_init  = 0;
	int y_delta = 1;

	// correct y index for upside down output
	if (zxi.m_bUpsideDown)
	{
		y_init  = zxi.m_ptSize.y - 1;
		y_delta = -1;
	}

	int y_idx = y_init;

	int y_count = zxi.m_ptSize.y;
	int y_line  = 0;

	// we need to know which line we're outputting, in case we're in zigzag mode
	for (y_line = 0; y_line < y_count; ++y_line)
	{
		// so, we have a line, what do we do with it?

		// these are used in the for loops
		int x_count = zxi.m_ptSize.x;
		int x_line  = 0;

		bool breverse = false;
		// first, work out which direction we're moving in...
		if (zxi.m_bReverse)
		{
			breverse = !breverse;
		}
		if (zxi.m_bZigZag && (y_line % 2))
		{
			breverse = !breverse;
		}

		// set up the parameters for stepping through the X axis
		int x_init  = 0;
		int x_delta = 1;

		// correct for reversing the direction
		if (breverse)
		{
			x_init  = zxi.m_ptSize.x - 1;
			x_delta = -1;
		}

		int x_idx = x_init;

		switch (zxf)
		{
		case ZXIMAGE_FORMAT_B:
		case ZXIMAGE_FORMAT_BBMM:
			{
				// output a line of bytes
				x_idx = x_init;
				if (zxi.m_bTextOutput) ost << "db ";
				for (x_line = 0; x_line < x_count; ++x_line)
				{
					if (zxi.m_bTextOutput)
					{
						ost << static_cast<int>(zxi.m_imgByte[y_idx][x_idx]);
						if (x_line + 1 < x_count) ost << ",";
					}
					else
					{
						ost << zxi.m_imgByte[y_idx][x_idx];
					}
					x_idx += x_delta;
				}
				if (zxi.m_bTextOutput) ost << std::endl;
			}
			break;
		case ZXIMAGE_FORMAT_M:
		case ZXIMAGE_FORMAT_MMBB:
			{
				// output a line of mask
				x_idx = x_init;
				if (zxi.m_bTextOutput) ost << "db ";
				for (x_line = 0; x_line < x_count; ++x_line)
				{
					if (zxi.m_bTextOutput)
					{
						ost << static_cast<int>(zxi.m_imgMask[y_idx][x_idx]);
						if (x_line + 1 < x_count) ost << ",";
					}
					else
					{
						ost << zxi.m_imgMask[y_idx][x_idx];
					}
					x_idx += x_delta;
				}
				if (zxi.m_bTextOutput) ost << std::endl;
			}
			break;
		case ZXIMAGE_FORMAT_BM:
			{
				// output alternating byte/mask
				x_idx = x_init;
				if (zxi.m_bTextOutput) ost << "db ";
				for (x_line = 0; x_line < x_count; ++x_line)
				{
					if (zxi.m_bTextOutput)
					{
						ost << static_cast<int>(zxi.m_imgByte[y_idx][x_idx]) << "," << static_cast<int>(zxi.m_imgMask[y_idx][x_idx]);
						if (x_line + 1 < x_count) ost << ",";
					}
					else
					{
						ost << zxi.m_imgByte[y_idx][x_idx] << zxi.m_imgMask[y_idx][x_idx];
					}
					x_idx += x_delta;
				}
				if (zxi.m_bTextOutput) ost << std::endl;
			}
			break;
		case ZXIMAGE_FORMAT_MB:
			{
				// output alternating mask/byte
				x_idx = x_init;
				if (zxi.m_bTextOutput) ost << "db ";
				for (x_line = 0; x_line < x_count; ++x_line)
				{
					if (zxi.m_bTextOutput)
					{
						ost << static_cast<int>(zxi.m_imgMask[y_idx][x_idx]) << "," << static_cast<int>(zxi.m_imgByte[y_idx][x_idx]);
						if (x_line + 1 < x_count) ost << ",";
					}
					else
					{
						ost << zxi.m_imgMask[y_idx][x_idx] << zxi.m_imgByte[y_idx][x_idx];
					}
					x_idx += x_delta;
				}
				if (zxi.m_bTextOutput) ost << std::endl;
			}
			break;
		default:
			{
				// whoops!
				std::ostringstream except;
				except << zxf;
				throw CZxImage::CExUnknownMask(except.str());
			}
			break;
		}

		// now some clean-up in case we were doing a line-by-line thing...
		// we need to go through and print out the other line.
		switch (zxf)
		{
		case ZXIMAGE_FORMAT_BBMM:
			{
				// output a line of masks now
				x_idx = x_init;
				if (zxi.m_bTextOutput) ost << "db ";
				for (x_line = 0; x_line < x_count; ++x_line)
				{
					if (zxi.m_bTextOutput)
					{
						ost << static_cast<int>(zxi.m_imgMask[y_idx][x_idx]);
						if (x_line + 1 < x_count) ost << ",";
					}
					else
					{
						ost << zxi.m_imgByte[y_idx][x_idx];
					}
					x_idx += x_delta;
				}
				if (zxi.m_bTextOutput) ost << std::endl;
			}
			break;
		case ZXIMAGE_FORMAT_MMBB:
			{
				// output a line of bytes now
				x_idx = x_init;
				if (zxi.m_bTextOutput) ost << "db ";
				for (x_line = 0; x_line < x_count; ++x_line)
				{
					if (zxi.m_bTextOutput)
					{
						ost << static_cast<int>(zxi.m_imgByte[y_idx][x_idx]);
						if (x_line + 1 < x_count) ost << ",";
					}
					else
					{
						ost << zxi.m_imgByte[y_idx][x_idx];
					}
					x_idx += x_delta;
				}
				if (zxi.m_bTextOutput) ost << std::endl;
			}
			break;
		default:
			break;
		}

		y_idx += y_delta;
	}

	return ost;
}
