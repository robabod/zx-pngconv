// Some Types that we can used hither and thither.

#ifndef __PNGCONV__TYPEDEFS__H__INC
#define __PNGCONV__TYPEDEFS__H__INC

#define PNGCONV_VERSION_STRING	"0.0.1"
#define PNGCONV_BUILD_DATE		__DATE__
#define PNGCONV_BUILD_TIME		__TIME__
#define PNGCONV_SHORT_TITLE		"PngConv"
#define PNGCONV_LONG_TITLE		"Png To ZX Convertor"
#define PNGCONV_COPYRIGHT		"Copyright 2006 Robert Cooper - http://icabod.org/"

#ifdef _DEBUG
#define PNGCONV_RELEASE			"_debug"
#else
#define PNGCONV_RELEASE			""
#endif

typedef enum ZXIMAGEFORMAT
{
	ZXIMAGE_FORMAT_B	= 0,	// no mask
	ZXIMAGE_FORMAT_M	= 1,	// no byte
	ZXIMAGE_FORMAT_MB	= 2,	// alternate mask then byte
	ZXIMAGE_FORMAT_BM	= 3,	// alternate byte then mask
	ZXIMAGE_FORMAT_MMBB	= 4,	// alternate lines of mask then byte
	ZXIMAGE_FORMAT_BBMM	= 5		// alternate lines of byte then mask
};

#define ZXIMAGE_FORMAT_DEFAULT ZXIMAGE_FORMAT_B

typedef struct POINT
{
	int x, y;
} *LPPOINT;


typedef unsigned char rgbaval ;
typedef unsigned char ucBYTE ;

#pragma pack(push)
#pragma pack(1)

struct RGBA
{
public:
	RGBA() {btRed=btGrn=btBlu=btAlp=0;}
	RGBA(rgbaval red, rgbaval grn, rgbaval blu) {btRed=red;btGrn=grn;btBlu=blu;btAlp=0;}
	RGBA(rgbaval red, rgbaval grn, rgbaval blu, rgbaval alp) {btRed=red;btGrn=grn;btBlu=blu;btAlp=alp;}
	operator=(const RGBA _rgba) {btRed=_rgba.btRed;btGrn=_rgba.btGrn;btBlu=_rgba.btBlu;btAlp=_rgba.btAlp;}

	inline bool PixelIsOn(int threshold = 128) {
		return (((int)btBlu + (int)btGrn * 6 + (int)btRed * 3) / 10) >= threshold;
	}

	inline bool PixelIsMasked(int threshold = 128) {
		return (int)btAlp <= threshold;
	}

	rgbaval btRed;
	rgbaval btGrn;
	rgbaval btBlu;
	rgbaval btAlp;
};
#pragma pack(pop)

#endif /* __PNGCONV__TYPEDEFS__H__INC */