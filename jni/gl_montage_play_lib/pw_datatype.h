

#ifndef	_PW_DATATYPE_H_
#define	_PW_DATATYPE_H_

#define	__sun 1

//64位整数--------
#ifdef	__unix
#ifdef	__alpha
typedef	long				GInt64;
typedef	unsigned long		GUInt64;
#elif	__sun
typedef	long long			GInt64;
typedef	unsigned long long	GUInt64;
#endif
#elif	__APPLE__
typedef	long long			GInt64;
typedef	unsigned long long	GUInt64;
#elif	WIN32
typedef	__int64				GInt64;
typedef	unsigned __int64	GUInt64;
#endif
//----------------

typedef	signed char			GInt8;
typedef	unsigned char		GUInt8;
typedef	short				GInt16;
typedef	unsigned short		GUInt16;

/*目前主流编译器(不论是否64位平台)int都是32位,只有老式编译器如turboC为16位, */
typedef int					GInt32;
typedef unsigned int		GUInt32;


typedef float				GFloat;
typedef double				GDouble;
typedef GUInt8				GByte;
typedef GUInt16				GWord;
typedef GUInt32				GDWord;
typedef void*				GHandle;
typedef char				GChar;
typedef int 				GBool; //wwy edit 
typedef void				GVoid;
typedef void*				GPVoid;
typedef GInt8*				GPChar;
typedef GInt16				GShort;
typedef const GInt8*		GPCChar;
typedef GUInt32				GULong; 
typedef GInt32				GLong; 
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

#ifndef BYTE
typedef unsigned char BYTE;
#define BYTE BYTE
#endif

#endif
/*--------------------------------*/

#endif //_PW_DATATYPE_H_
