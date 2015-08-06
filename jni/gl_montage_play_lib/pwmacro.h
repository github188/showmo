/*----------------------------------------------------------------------------------------------
*
* This file is Puwell's property. It contains Puwell's trade secret, proprietary and 		
* confidential information. 
* 
* The information and code contained in this file is only for authorized Puwell employees 
* to design, create, modify, or review.
* 
* DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER AUTHORIZATION.
* 
* If you are not an intended recipient of this file, you must not copy, distribute, modify, 
* or take any action in reliance on it. 
* 
* If you have received this file in error, please immediately notify Puwell and 
* permanently delete the original and any copy of any file and any printout thereof.
*
*---------------------------------------------------------------------------------------------*/

#ifndef __PWMACRO_H__
#define __PWMACRO_H__

#define PW(fun)			lgy##fun
		
//	#define GrowMask		PW(GrowMask)

#ifndef	PI
#define PI				3.1415926535
#define PIF				3.14169265f
#endif
#ifndef	PI_20
#define PI_20			3294199
#endif

#define GNull		0
#define GFalse		0
#define GTrue		1

#ifndef PW_MAX_PATH
#define PW_MAX_PATH	256
#endif

#ifndef	PW_FABS
#define PW_FABS(x)		(((x) < 0) ? -(x) : (x))
#endif

#ifndef	PWABS8
#define PWABS8(x)		(((x)+((x)>>7))^((x)>>7))
#endif

#ifndef	PWABS16
#define PWABS16(x)		(((x)+((x)>>15))^((x)>>15))
#endif

#ifndef	PWABS32
#define PWABS32(x)		(((x)+((x)>>31))^((x)>>31))
#endif

#ifndef	PWABS64
#define PWABS64(x)		(((x)+((x)>>63))^((x)>>63))
#endif


#ifndef	PW_MAX
#define PW_MAX(x,y)		(((x)>=(y))?(x):(y))
#endif

#ifndef	PW_MIN
#define PW_MIN(x,y)		(((x)<=(y))?(x):(y))
#endif

#ifndef	PW_SWAP
#define PW_SWAP(x,y,t)	((t) = (x), (x) = (y), (y) = (t))
#endif

#ifndef	PW_FPI2D
#define PW_FPI2D(x)	((x)*57.29578f)  // x*180/PI
#endif

#ifndef	PW_FD2PI
#define PW_FD2PI(x)	((x)*0.01745329f)  // x*PI/180
#endif

#ifndef	PW_PI2D_10
#define PW_PI2D_10(x)	((x)*58671)  // x*180/PI
#endif

#ifndef	PW_D2PI_10
#define PW_D2PI_10(x)	(((x)*143)>>3)  // x*PI/180
#endif

#define PW_ROUND(x)		((GLong)((x)+0.5))
#define PW_ROUND_N(x, type, times)		(type)((x)*times+0.5)

#define PW_TRIMBYTE(x)	(GUInt8)((x)&(~255)?((-(x))>>31):(x))
#define PW_TRIM16(x)	(GUInt16)((x)&(~65535)?((-(x))>>31):(x))


#define	yuv_shift		14
#define	yuv_fix(x)		(int)((x) * (1 << (yuv_shift)) + 0.5f)
#define	yuv_descale(x)	(((x) + (1 << ((yuv_shift)-1))) >> (yuv_shift))
#define	yuv_prescale(x)	((x) << yuv_shift)

#define	yuvYr	yuv_fix(0.299f)
#define	yuvYg	yuv_fix(0.587f)
#define	yuvYb	yuv_fix(0.114f)
#define	yuvCr	yuv_fix(0.713f)
#define	yuvCb	yuv_fix(0.564f)

#define	yuvRCr	yuv_fix(1.403f)
#define	yuvGCr	(-yuv_fix(0.714f))
#define	yuvGCb	(-yuv_fix(0.344f))
#define	yuvBCb	yuv_fix(1.773f)

#define	ET_CAST_8U(t)		(GUInt8)(!((t) & ~255) ? (t) : (t) > 0 ? 255 : 0)

#define ET_YUV_TO_R(y,v)	(GUInt8)(ET_CAST_8U(yuv_descale((yuv_prescale(y)) + yuvRCr * (v))))
#define ET_YUV_TO_G(y,u,v)	(GUInt8)(ET_CAST_8U(yuv_descale((yuv_prescale(y)) + yuvGCr * (v) + yuvGCb * (u))))
#define ET_YUV_TO_B(y,u)	(GUInt8)(ET_CAST_8U(yuv_descale((yuv_prescale(y)) + yuvBCb * (u))))

// #define ET_RGB_TO_Y(r,g,b)	(GLong)(yuv_descale((b) * yuvYb + (g) * yuvYg + (r) * yuvYr))
// #define ET_RGB_TO_U(y,b)	(GLong)(yuv_descale(((b) - (y)) * yuvCb) + 128)
// #define ET_RGB_TO_V(y,r)	(GLong)(yuv_descale(((r) - (y)) * yuvCr) + 128)
#define ET_RGB_TO_Y(r,g,b)	(GUInt8)(ET_CAST_8U(yuv_descale((b) * yuvYb + (g) * yuvYg + (r) * yuvYr)))
#define ET_RGB_TO_U(y,b)	(GUInt8)(ET_CAST_8U(yuv_descale(((b) - (y)) * yuvCb) + 128))
#define ET_RGB_TO_V(y,r)	(GUInt8)(ET_CAST_8U(yuv_descale(((r) - (y)) * yuvCr) + 128))

#define ET_RGB_TO_US(y,b)	(GLong)(yuv_descale(((b) - (y)) * yuvCb))
#define ET_RGB_TO_VS(y,r)	(GLong)(yuv_descale(((r) - (y)) * yuvCr))

#define AILLINE_BYTES(w, b)	(((GLong)(w) * (b) + 31) / 32 * 4)

#define PW_SET1_4(t)	(t)
#define PW_SET2_4(t)	(t<<8)
#define PW_SET3_4(t)	(t<<16)
#define PW_SET4_4(t)	(t<<24)

#define PW_GET1_4(t)	(t & 0x0ff)
#define PW_GET2_4(t)	((t<<16)>>24)
#define PW_GET3_4(t)	((t<<8)>>24)
#define PW_GET4_4(t)	(t>>24)


#define PA_PROTOCOL_8	   		0x78	
#define PA_PROTOCOL_9      		0x79	
#define PA_PROTOCOL_PRODUCT		0x7a	
#endif