///////////////////////////版权/////////////////////////////
/*---------------------------------------------------------------
* Copyright (c) 2008
* All rights reserved.
*
* 文件名称： datatype.h
* 文件标识：
* 摘 要：基本数据类型定义头文件. 字长不同的机器(short 为16为bit int基
*		 本都是32位bit),long型长度不同,指针范围也是以long为准.
* 注意:   尽量避免本文件与第三方的数据基本类型定义头文件 被同一个文件包含,若无法避免,应尽可能
*		  第三方头文件至于前面,以免污染第三方头文件接口;
*		  尽量避免使用MOCK_WINBASE_DATATYPE宏开关启用的数据类型(VOID,CHAR ...),尽可能用Gxxx数据
*		  类型,从而避免与第三方数据类型冲突;
* 当前版本：0.3
* 修改者： 汪文原
* 修改摘要: 鉴于sun机可能为little_endian,所以若是宏定义了_UNDFSUN(采用人工宏定义)则认为是little_endian
* 完成日期：2008年4月2日
* 修改by：刘光盐
* 完成日期：2013年3月28日
* 无关网络传输的类型定义，所有大小端转型所有的构造体改为普通类型；兼容掉之前算法所用的类型
* 修改者： 汪文原
* 修改摘要: 添加 MOCK_WINBASE_DATATYPE宏,及其包含的数据类型
* 日期：2013年4月8日
* 取代版本：
* 原作者 ：
* 完成日期：
---------------------------------------------------------------*/

#ifndef	_PW_DATATYPE_H_
#define	_PW_DATATYPE_H_

//64位整数--------
#ifdef	__unix
#ifdef	__alpha
typedef	long				GInt64;
typedef	unsigned long		GUInt64;
#elif	__sun
typedef	long long			GInt64;
typedef	unsigned long long	GUInt64;
#else
typedef	long long			GInt64;
typedef	unsigned long long	GUInt64;
#endif
#elif	WIN32
typedef	long long			GInt64;
typedef	unsigned long long	GUInt64;
#else
typedef	long long           GInt64;
typedef	unsigned long long	GUInt64;
#endif

//----------------

typedef	signed char			GInt8;
typedef	unsigned char		GUInt8;
typedef	short				GInt16;
typedef	unsigned short		GUInt16;

/*目前主流编译器(不论是否64位平台)int都是32位,只有老式编译器如turboC为16位, */
typedef int					GInt32;
typedef unsigned int		GUInt32;

typedef int					GInt;
typedef unsigned int		GUInt;

typedef float				GFloat;
typedef double				GDouble;
typedef GUInt8				GByte;
typedef GUInt16				GWord;
typedef GUInt32				GDWord;
typedef void*				GHandle;
typedef char				GChar;
typedef unsigned char		GUChar;//wwy add
typedef int 				GBool; //wwy edit 
typedef void				GVoid;
typedef void*				GPVoid;
typedef GInt8*				GPChar;
typedef GInt16				GShort;
typedef GUInt16				GUShort;//wwy add
typedef const GInt8*		GPCChar;
typedef unsigned long		GULong;
typedef long				GLong; 
typedef	GLong				GRESULT;//wwy remark
typedef GDWord				GCOLORREF; 

#if defined(__cplusplus)
//8位位类型
class GBit8
{
private:
	unsigned char out;
public:
	GBit8()	{}
	GBit8(unsigned char in)	{out = in;}
	void operator = (unsigned char in)	{out = in;}
	int operator [] (unsigned int n)
	{
		return GetBit(n);
	}
	int GetBit(unsigned int n)
	{
		if(n > 7)	return -1;
		unsigned char val,temp=1;
		val = out;
		temp <<= n;
		val &= temp;
		val >>= n;
		return val;
	}
	int SetBit(unsigned int n,int val=0)
	{
		if(n>7 || val<0 || val>1)	return 0;
		char temp = 1;
		temp <<= n;
		temp ^= 0xFF;
		out &= temp;
		val <<= n;
		out |= val;
		return 1;
	}
	operator unsigned char()
	{
		return out;
	}
};
#else 
/* Boolean definition */ 
#ifndef  bool 
#define bool GBool
#endif 

#endif 

/* 尽量避免使用MOCK_WINBASE_DATATYPE宏开关启用的数据类型,尽可能用GXXX数据类型,从而避免与第三方数据类型冲突*/
#ifdef  MOCK_WINBASE_DATATYPE

#ifndef VOID
#define VOID void
#endif

#ifndef CHAR
typedef char CHAR;
#define CHAR CHAR
#endif

#ifndef UCHAR
typedef unsigned char UCHAR;
#define UCHAR UCHAR
#endif

#ifndef SHORT
typedef short SHORT;
#define SHORT SHORT
#endif

#ifndef INT
typedef int INT;
#define INT INT
#endif

#ifndef UINT
typedef unsigned int UINT;
#define UINT UINT
#endif

#ifndef LONG
typedef long LONG;
#define LONG LONG
#endif

#ifndef ULONG
typedef unsigned long ULONG;
#define ULONG ULONG
#endif

#ifndef WORD
 typedef unsigned short WORD;
 #define WORD WORD
#endif

#ifndef DWORD
 typedef unsigned int DWORD;
 #define DWORD DWORD
#endif

 #ifndef BOOL
typedef int BOOL; //wwy edit 20131120
#define BOOL BOOL
#endif

#endif
/*--------------------------------*/
#ifndef BYTE
typedef unsigned char BYTE;
#define BYTE BYTE
#endif

#endif //_PW_DATATYPE_H_
