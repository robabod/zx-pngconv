// PngImage.h: interface for the CPngImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PNGIMAGE_H__55023B3B_17BD_450E_81A7_D8BFF02B6E9D__INCLUDED_)
#define AFX_PNGIMAGE_H__55023B3B_17BD_450E_81A7_D8BFF02B6E9D__INCLUDED_

#include "typedefs.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>
#include "png.h"

class CPngImage  
{
public:
	inline POINT GetImageSize() const {return m_size;};
	static ReadPngData(png_structp png_ptr, png_bytep data, png_size_t length);
	ReadImage();
	Open(std::string filename);
	CPngImage(std::string filename);
	CPngImage();
	virtual ~CPngImage();
	std::string Filename() {return m_sFilename;};
	RGBA GetRGBA(int x, int y) const;
	Close();

protected:
	png_bytep* m_rowpointers;
	std::ifstream m_ifs;
	std::string m_sFilename;

private:
	static const size_t PNG_BYTE_TO_CHECK;
	bool valid_png_image;
	png_structp png_ptr;
	png_infop png_info;
	int m_depth;
	int m_chans;
	int m_coltype;
	POINT m_size;

public:
	// define some exceptions from this class...
	class CPngFileException : public std::runtime_error {public: CPngFileException(const std::string &what_arg) : std::runtime_error(what_arg) {};};
	class CPngLibException : public std::runtime_error {public: CPngLibException(const std::string &what_arg) : std::runtime_error(what_arg) {};};

};

#endif // !defined(AFX_PNGIMAGE_H__55023B3B_17BD_450E_81A7_D8BFF02B6E9D__INCLUDED_)
