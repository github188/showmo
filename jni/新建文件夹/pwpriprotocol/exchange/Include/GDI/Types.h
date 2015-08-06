
#ifndef __GDI_TYPES_H__
#define __GDI_TYPES_H__

#include "../Infra/Types.h"

typedef uint			PARAM;
typedef void *			HANDLE;
typedef uint 			RGBA32;		//0x00bbggrr
typedef uint 			COLORDEV;	//0x????????

#define MAKEUINT16(a, b)		((ushort)(((uchar)(a)) | ((ushort)((uchar)(b))) << 8))
#define MAKEINT32(a, b)			((int)(((ushort)(a)) | ((uint)((ushort)(b))) << 16))
#define MAKEUINT32(a, b, c, d)	((uint)(((uchar)(a)) | ((uchar)(b) << 8) | ((uchar)(c) << 16) | ((uchar)(d) << 24)))
#define LOWINT16(l)				((ushort)(l))
#define HIGHINT16(l)			((ushort)(((uint)(l) >> 16) & 0xFFFF))
#define LOINT8(w)				((uchar)(w))
#define HIINT8(w)				((uchar)(((ushort)(w) >> 8) & 0xFF))

#define MakeRGB(r,g,b)			((RGBA32)(((uchar)(r)|((ushort)((uchar)(g))<<8))|(((uint)(uchar)(b))<<16)))
#define MakeRGBA(r,g,b,a)		((RGBA32)(((uchar)(r)|((ushort)((uchar)(g))<<8))|(((uint)(uchar)(b))<<16)|(((uint)(uchar)(a))<<24)))
#define GetRed(c)				((uchar)(c))
#define GetGreen(c)				((uchar)(((ushort)(c)) >> 8))
#define GetBlue(c)				((uchar)((c)>>16))
#define GetAlpha(c)				((uchar)((c)>>24))

#define MakeRGB_16(r,g,b)       ((ushort)((((r) >> 3) << 10) | (((g) >> 3) << 5) | (((b) >> 3) << 0)))

typedef struct tagRect
{
	int left;
	int top;
	int right;
	int bottom;
}Rect;

typedef struct tagPoint
{
	int x;
	int y;
}Point;

typedef struct tagSize 
{
	int w;
	int h;
}Size;

typedef const char* PCSTR;

typedef const Rect *PCRECT;
typedef Rect *PRECT;

typedef const Point *PCPOINT;
typedef Point *PPOINT;

typedef const Size *PCSIZE;
typedef Size *PSIZE;

#endif
