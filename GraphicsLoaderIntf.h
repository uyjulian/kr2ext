//---------------------------------------------------------------------------
/*
	TVP2 ( T Visual Presenter 2 )  A script authoring tool
	Copyright (C) 2000-2007 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// Graphics Loader ( loads graphic format from storage )
//---------------------------------------------------------------------------

#ifndef GraphicsLoaderIntfH
#define GraphicsLoaderIntfH
#include "tp_stub.h"
#include <vector>

class tTVPBaseBitmap;
namespace TJS {
    class tTJSBinaryStream;
}


//---------------------------------------------------------------------------
// Graphic Loading Handler Type
//---------------------------------------------------------------------------
typedef void (__fastcall *tTVPGraphicSizeCallback)
	(void *callbackdata, tjs_uint w, tjs_uint h);
typedef void (__fastcall *tTVPGraphicSizeCallback_r)
	(tjs_uint h, tjs_uint w, void *callbackdata);
/*
	callback type to inform the image's size.
	call this once before TVPGraphicScanLineCallback.
*/

typedef void * (__fastcall *tTVPGraphicScanLineCallback)
	(void *callbackdata, tjs_int y);
typedef void * (__fastcall *tTVPGraphicScanLineCallback_r)
	(tjs_int y, void *callbackdata);
/*
	callback type to ask the scanline buffer for the decoded image, per a line.
	returning null can stop the processing.

	passing of y=-1 notifies the scan line image had been written to the buffer that
	was given by previous calling of TVPGraphicScanLineCallback. in this time,
	this callback function must return NULL.
*/
typedef void(__fastcall *tTVPMetaInfoPushCallback)
	(void *callbackdata, const ttstr & name, const ttstr & value);
typedef void(__fastcall *tTVPMetaInfoPushCallback_r)
	(const ttstr & value, const ttstr & name, void *callbackdata);
/*
	callback type to push meta-information of the image.
	this can be null.
*/

enum tTVPGraphicLoadMode
{
	glmNormal, // normal, ie. 32bit ARGB graphic
	glmPalettized, // palettized 8bit mode
	glmGrayscale // grayscale 8bit mode
};

#if 0
typedef void(*tTVPGraphicLoadingHandler)(void* formatdata,
	void *callbackdata,
	tTVPGraphicSizeCallback sizecallback,
	tTVPGraphicScanLineCallback scanlinecallback,
	tTVPMetaInfoPushCallback metainfopushcallback,
	tTJSBinaryStream *src,
	tjs_int32 keyidx,
	tTVPGraphicLoadMode mode);
#else
typedef void(__fastcall *tTVPGraphicLoadingHandler)(
	tTVPGraphicSizeCallback_r sizecallback,
	void *callbackdata,
	tTVPGraphicLoadMode mode,
	tjs_int32 keyidx,
	tTJSBinaryStream *src,
	tTVPMetaInfoPushCallback_r metainfopushcallback,
	tTVPGraphicScanLineCallback_r scanlinecallback
	//,void* formatdata
	);
#endif
/*
	format = format specific data given at TVPRegisterGraphicLoadingHandler
	dest = destination callback function
	src = source stream
	keyidx = color key for less than or equal to 8 bit image
	mode = if glmPalettized, the output image must be an 8bit color (for province
	   image. so the color is not important. color index must be preserved).
	   if glmGrayscale, the output image must be an 8bit grayscale image.
	   otherwise the output image must be a 32bit full-color with opacity.

	color key does not overrides image's alpha channel ( if the image has )

	the function may throw an exception if error.
*/

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// Graphics Format Management
//---------------------------------------------------------------------------
void TVPRegisterGraphicLoadingHandler(const ttstr & name,
	tTVPGraphicLoadingHandler handler, void* formatdata);
void TVPUnregisterGraphicLoadingHandler(const ttstr & name,
	tTVPGraphicLoadingHandler handler, void * formatdata);
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
// default handlers
//---------------------------------------------------------------------------
extern void TVPLoadBMP(void* formatdata, void *callbackdata, tTVPGraphicSizeCallback sizecallback,
	tTVPGraphicScanLineCallback scanlinecallback, tTVPMetaInfoPushCallback metainfopushcallback,
	tTJSBinaryStream *src, tjs_int keyidx,  tTVPGraphicLoadMode mode);

extern void TVPLoadJPEG(void* formatdata, void *callbackdata, tTVPGraphicSizeCallback sizecallback,
	tTVPGraphicScanLineCallback scanlinecallback, tTVPMetaInfoPushCallback metainfopushcallback,
	tTJSBinaryStream *src, tjs_int keyidx,  tTVPGraphicLoadMode mode);

extern void TVPLoadPNG(void* formatdata, void *callbackdata, tTVPGraphicSizeCallback sizecallback,
	tTVPGraphicScanLineCallback scanlinecallback, tTVPMetaInfoPushCallback metainfopushcallback,
	tTJSBinaryStream *src, tjs_int keyidx,  tTVPGraphicLoadMode mode);

extern void TVPLoadERI(void* formatdata, void *callbackdata, tTVPGraphicSizeCallback sizecallback,
	tTVPGraphicScanLineCallback scanlinecallback, tTVPMetaInfoPushCallback metainfopushcallback,
	tTJSBinaryStream *src, tjs_int keyidx,  tTVPGraphicLoadMode mode);

void TVPLoadWEBP(void* formatdata, void *callbackdata, tTVPGraphicSizeCallback sizecallback,
	tTVPGraphicScanLineCallback scanlinecallback, tTVPMetaInfoPushCallback metainfopushcallback,
	tTJSBinaryStream *src, tjs_int keyidx,  tTVPGraphicLoadMode mode);
//---------------------------------------------------------------------------



class iTVPBaseBitmap;
//---------------------------------------------------------------------------
// BMP saving handler
//---------------------------------------------------------------------------
void TVPSaveAsBMP(const ttstr & storagename, const ttstr & mode,
    iTVPBaseBitmap *bmp);

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// JPEG loading handler
//---------------------------------------------------------------------------
enum tTVPJPEGLoadPrecision
{
	jlpLow,
	jlpMedium,
	jlpHigh
};

extern tTVPJPEGLoadPrecision TVPJPEGLoadPrecision;
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// Graphics cache management
//---------------------------------------------------------------------------
extern bool TVPAllocGraphicCacheOnHeap;
	// this allocates graphic cache's store memory on heap, rather than
	// shareing bitmap object. ( since sucking win9x cannot have so many bitmap
	// object at once, WinNT/2000 is ok. )
	// this will take more time for memory copying.
extern void TVPSetGraphicCacheLimit(tjs_uint limit);
	// set graphic cache size limit by bytes.
	// limit == 0 disables the cache system.
	// limit == -1 sets the limit to TVPGraphicCacheSystemLimit
extern tjs_uint TVPGetGraphicCacheLimit();

extern tjs_uint TVPGraphicCacheSystemLimit;
	// maximum possible value of Graphic Cache Limit

extern void TVPTouchImages(const std::vector<ttstr> & storages, tjs_int limit,
	tjs_uint64 timeout);

//---------------------------------------------------------------------------

struct tTVPGraphicHandlerType
{
	ttstr Extension;
	tTVPGraphicLoadingHandler Handler;
	void * FormatData;

	tTVPGraphicHandlerType(const ttstr &ext,
						   tTVPGraphicLoadingHandler handler, void * data)
	{
		Extension = ext, Handler = handler, FormatData = data;
	}

	tTVPGraphicHandlerType(const tTVPGraphicHandlerType & ref)
	{
		Extension = ref.Extension;
		Handler = ref.Handler;
		FormatData = ref.FormatData;
	}

	bool operator == (const tTVPGraphicHandlerType & ref) const
	{
		return FormatData == ref.FormatData &&
			Handler == ref.Handler &&
			Extension == ref.Extension;
	}
};



//---------------------------------------------------------------------------
// TVPLoadGraphic
//---------------------------------------------------------------------------
// extern int TVPLoadGraphic(iTVPBaseBitmap *dest, const ttstr &name, tjs_int keyidx,
// 	tjs_uint desw, tjs_uint desh,
// 	tTVPGraphicLoadMode mode, ttstr *provincename = NULL, iTJSDispatch2 ** metainfo = NULL);
	// throws exception when this function can not handle the file
//---------------------------------------------------------------------------
// extern void TVPLoadGraphicProvince(tTVPBaseBitmap *dest, const ttstr &name, tjs_int keyidx,
//     tjs_uint desw, tjs_uint desh);



//---------------------------------------------------------------------------
// BMP loading interface
//---------------------------------------------------------------------------

#ifndef BI_RGB // avoid re-define error on Win32
	#define BI_RGB			0
	#define BI_RLE8			1
	#define BI_RLE4			2
	#define BI_BITFIELDS	3
#endif

#pragma pack(push, 1)
struct TVP_WIN_BITMAPFILEHEADER
{
	tjs_uint16	bfType;
	tjs_uint32	bfSize;
	tjs_uint16	bfReserved1;
	tjs_uint16	bfReserved2;
	tjs_uint32	bfOffBits;
};
struct TVP_WIN_BITMAPINFOHEADER
{
	tjs_uint32	biSize;
	tjs_int		biWidth;
	tjs_int		biHeight;
	tjs_uint16	biPlanes;
	tjs_uint16	biBitCount;
	tjs_uint32	biCompression;
	tjs_uint32	biSizeImage;
	tjs_int		biXPelsPerMeter;
	tjs_int		biYPelsPerMeter;
	tjs_uint32	biClrUsed;
	tjs_uint32	biClrImportant;
};
#pragma pack(pop)

enum tTVPBMPAlphaType
{
	// this specifies alpha channel treatment if the bitmap is 32bpp.
	// note that TVP currently does not support new (V4 or V5) bitmap header
	batNone, // plugin does not return alpha channel.
	batMulAlpha, // returns alpha channel, d = d * alpha + s * (1-alpha)
	batAddAlpha // returns alpha channel, d = d * alpha + s
};


extern void TVPInternalLoadBMP(void *callbackdata,
	tTVPGraphicSizeCallback sizecallback,
	tTVPGraphicScanLineCallback scanlinecallback,
	TVP_WIN_BITMAPINFOHEADER &bi,
	const tjs_uint8 *palsrc,
	tTJSBinaryStream * src,
	tjs_int keyidx,
	tTVPBMPAlphaType alphatype,
	tTVPGraphicLoadMode mode);

//---------------------------------------------------------------------------


#endif
