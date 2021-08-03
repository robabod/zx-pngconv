// PngImage.cpp: implementation of the CPngImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pngconv.h"
#include "PngImage.h"

#include "exceptions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

#include <sstream>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const size_t CPngImage::PNG_BYTE_TO_CHECK = 8;

CPngImage::CPngImage()
: valid_png_image(false)
, png_info(NULL)
, png_ptr(NULL)
{
}

CPngImage::~CPngImage()
{
	// now we have our data.
	if (png_info && png_ptr)
		png_destroy_read_struct(&png_ptr, &png_info, png_infopp_NULL);

	if (m_ifs.is_open())
		m_ifs.close();
}

CPngImage::CPngImage(std::string filename)
: m_sFilename(filename)
, valid_png_image(false)
, png_info(NULL)
, png_ptr(NULL)
{
	try
	{
		Open(filename);
	}
	catch (...)
	{
		throw;
	}
}

CPngImage::Open(std::string filename)
{
	if (!m_ifs.is_open())
	{
		m_sFilename = filename;
		m_ifs.open(m_sFilename.c_str(), std::ios::in | std::ios::binary);
		if (!m_ifs.is_open())
		{
			throw CFileException(filename);
		}
		else
		{
			// see if it's actually a png file...
			png_byte png_header[PNG_BYTE_TO_CHECK];
			m_ifs.read(reinterpret_cast<char*>(png_header), PNG_BYTE_TO_CHECK);
			if (m_ifs.fail())
			{
				throw CFileException(filename);
			}
			else
			{
				// check if what we've read in is a PNG file.
				if (png_sig_cmp(png_header, 0, PNG_BYTE_TO_CHECK))
				{
					std::ostringstream except;
					except << "\"" << filename << "\" is not a PNG file.";
					throw CPngFileException(except.str());
				}
				else
					valid_png_image = true;
			}
		}
	}
}

CPngImage::ReadImage()
{
	if (!valid_png_image)
	{
		throw CPngFileException("Invalid PNG File trying to read image");
	}

	// now read it in...
	// first set us up some structures
	// we'll use default error handling for now
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		throw CPngLibException("png_create_read_struct() failed");
	}

	png_info = png_create_info_struct(png_ptr);
	if (!png_info)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		throw CPngLibException("png_create_info_struct() failed");
	}

	// if we get here, all is well.
	// so set our own read method...
	png_set_read_fn (png_ptr, &m_ifs, reinterpret_cast<png_rw_ptr> (ReadPngData));

	// we've already checked some bytes, so let the library know...
	png_set_sig_bytes (png_ptr, PNG_BYTE_TO_CHECK);


	// now start to read the data.
	// we could do a high-level read - we kinda will in fact.
	// but we'll allow for simple expansion later on...
	//   read info, set transformations, read data, read end

	if (0)
	{
		// this is only for when we're doing full reading
		// set some transformations here if needed.

		png_read_info (png_ptr, png_info);

		// first convert to RGBA
		png_byte color_type = png_get_color_type (png_ptr, png_info);
		png_byte color_depth = png_get_bit_depth (png_ptr, png_info);

		if (PNG_COLOR_TYPE_PALETTE == color_type)
			png_set_expand (png_ptr);
		if (PNG_COLOR_TYPE_GRAY == color_type && color_depth < 8)
			png_set_expand (png_ptr);
		if (png_get_valid (png_ptr, png_info, PNG_INFO_tRNS))
			png_set_expand (png_ptr);
		if (16 == color_depth)
			png_set_strip_16 (png_ptr);
		if (PNG_COLOR_TYPE_GRAY == color_type || PNG_COLOR_TYPE_GRAY_ALPHA == color_type)
			png_set_gray_to_rgb (png_ptr);

		// now we'll set up a de-interlace, just in case the image is interlaced
		png_set_interlace_handling (png_ptr);
	}
	else
	{
		int transforms = PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND;
		png_read_png(png_ptr, png_info, transforms, NULL);

		m_depth = png_get_bit_depth(png_ptr, png_info);
		m_chans = png_get_channels(png_ptr, png_info);
		m_coltype = png_get_color_type(png_ptr, png_info);

		m_size.x = png_get_image_width(png_ptr, png_info);
		m_size.y = png_get_image_height(png_ptr, png_info);

		m_rowpointers = png_get_rows(png_ptr, png_info);
	}

}

CPngImage::ReadPngData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	std::ifstream *ifs = reinterpret_cast<std::ifstream *> (png_ptr->io_ptr);

	ifs->read(reinterpret_cast<char*>(data), length);

	if (ifs->gcount() != length)
		png_error(png_ptr, "Read Error");
}

RGBA CPngImage::GetRGBA(int x, int y) const
{
	// get the pixel at this point.
	// if requested pixel is outside the image, we'll return all off.
	RGBA _rgba;
	POINT pt = GetImageSize();

	// because of how we read in the image, it should be stored as:
	// 8-bit greyscale, 16-bit greyscale+alpha, 24-bit rgb, or 32-bit rgba.
	// i think.  hmm.
	if (x >= 0 && x < pt.x && y >= 0 && y < pt.y)
	{
		// get pointer to the row
		png_bytep row = m_rowpointers[y];
		int x_off = x * m_chans;

		switch (m_coltype)
		{
		case PNG_COLOR_TYPE_GRAY:
			_rgba.btBlu = _rgba.btGrn = _rgba.btRed = row[x_off];
			break;
		case PNG_COLOR_TYPE_GA:
			_rgba.btBlu = _rgba.btGrn = _rgba.btRed = row[x_off];
			_rgba.btAlp = row[x_off+1];
			break;
		case PNG_COLOR_TYPE_RGB:
			_rgba.btRed = row[x_off + 0];
			_rgba.btGrn = row[x_off + 1];
			_rgba.btBlu = row[x_off + 2];
			break;
		case PNG_COLOR_TYPE_RGBA:
			_rgba.btRed = row[x_off + 0];
			_rgba.btGrn = row[x_off + 1];
			_rgba.btBlu = row[x_off + 2];
			_rgba.btAlp = row[x_off + 3];
			break;
		default:
			break;
		}
	}
	else
	{
		// it's outside the image, so we'll set the alpha channel to 255
		_rgba.btAlp = 255;
	}

	return _rgba;
}

CPngImage::Close()
{
	if (png_info && png_ptr)
		png_destroy_read_struct(&png_ptr, &png_info, png_infopp_NULL);

	if (m_ifs.is_open())
		m_ifs.close();
}

