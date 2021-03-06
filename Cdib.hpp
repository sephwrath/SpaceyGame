///////////////////////////////////////
//
// Developer: Stephen Moran
// Date: 4-9-1999
// Comments: Tools for manipulating bitmaps stored in memory
// Dib Class
//      
///////////////////////////////////////
#ifndef CDIB_HPP_
#define CDIB_HPP_

///////////////////////////////////////
// include Files
///////////////////////////////////////
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>


///////////////////////////////////////
// definitions
///////////////////////////////////////
typedef enum enum_CDIB_States {
                CDIB_OK,
				FAIL_DIB_OPEN,
				FAIL_FHDR_OPEN,
				FAIL_FHDR_READ,
				FAIL_BFTYPE_INVALID,
				FAIL_IHDR_READ,
				FAIL_FORMAT_INVALID,
				FAIL_CORE_READ,
				FAIL_LPBI_ALLOC,
				FAIL_CLR_READ,
				FAIL_RGBQD_READ,
				FAIL_BITS_ALLOC,
				FAIL_BITS_READ
		} CDibState;

///////////////////////////////////////
// class declaration CDib
///////////////////////////////////////
class CDib 
{
protected:
    LPBITMAPINFO lpbinfo;       // Pointer to BITMAPINFO struct
    BYTE*        lpBits;        // Pointer to the bits
	LPRGBQUAD    lprgb;			// Pointer to RGBQUAD
	//RECT         bounds;        // Rectangle bounding image
	CDibState    state ;        // current state of DIB

public:
    RECT         bounds;
    CDib();
    ~CDib();

    BITMAPINFO* GetBitmapInfoPtr()
        {return lpbinfo;}                      // Pointer to bitmap info
    BYTE* GetBitsPtr()
        {return lpBits;}                       // Pointer to the bits
    LPRGBQUAD GetLPRGBPtr()
        {return (LPRGBQUAD)(((BYTE *)(lpbinfo)) 
            + sizeof(BITMAPINFOHEADER));}      // Pointer to color table

    BOOL NewDIBObject(int width, int height); 
    void DrawBits(HDC hDC, int x, int y);
	void DrawBits(HDC hDC, RECT *r);
	BOOL SetBitmapHeader(int biWidth, int biHeight);
	BOOL SetDefaultPalette();
	void FreeCDib();
	BOOL ReadFile(LPSTR lpszFilename);
	void SetCDibState( CDibState estate );

	HPALETTE CreateLogicalPalette();
	BOOL CopyDIBBits( const CDib& srcDIB, RECT srcRect, RECT dstRect);
	BOOL CopyLRGBQUAD( const CDib& srcDIB);
	UINT GetBITMAPINFOSize()
	   {return((UINT)(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)));}
    BOOL CreateBITMAPINFO();
	BOOL CopyBITMAPINFO( const CDib& srcDIB );
	void SetBounds(int left, int top, int right, int bottom);
	void SetBounds();
	UINT GetCDibBiHeight();
	UINT GetCDibBiWidth();
	BOOL CopyTransDIBBits( const CDib& srcDIB, RECT srcRect, RECT dstRect, BYTE transByte);
	BYTE GetPixelByte( UINT x, UINT y);
	COLORREF GetPixelRGB( UINT x, UINT y);
};

#endif

