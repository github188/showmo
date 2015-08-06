#ifndef __INFRA_TIME_H__
#define __INFRA_TIME_H__

#include "Types.h"
#include <string>

struct TimeSection
{
	int enable;
	int startHour;
	int	startMinute;
	int	startSecond;
	int	endHour;
	int	endMinute;
	int	endSecond;
};

/// 系统时间结构,需要和SYSTEM_TIME保持一致
struct SystemTime
{
	int  year;		///< 年。   
	int  month;		///< 月，January = 1, February = 2, and so on.   
	int  day;		///< 日。   
	int  wday;		///< 星期，Sunday = 0, Monday = 1, and so on   
	int  hour;		///< 时。   
	int  minute;	///< 分。   
	int  second;	///< 秒。   
	int  isdst;		///< 夏令时标识。   
};

#ifndef __SMALL_FILE_TIME_DEFINED__
#define __SMALL_FILE_TIME_DEFINED__
// 时间结构
typedef struct _DATETIME								
{
#ifdef __BIG_ENDIAN__
	uint year		:6;				//	年	2000-2063	
	uint month		:4;				//	月	1-12		
	uint day		:5;				//	日	1-31		
	uint hour		:5;				//	时	0-23		
	uint minute		:6;				//	分	0-59		
	uint second		:6;				//	秒	0-59	
#else
	uint second		:6;				//	秒	0-59		
	uint minute		:6;				//	分	0-59		
	uint hour		:5;				//	时	0-23		
	uint day		:5;				//	日	1-31		
	uint month		:4;				//	月	1-12		
	uint year		:6;				//	年	2000-2063	
#endif
} DateTime,*pDATETIME;
#endif

/// \brief 时间服务封装
/// 
class CTime : public SystemTime
{
public:

	/// 日期格式
	enum DateFormat
	{
		DF_YYMMDD = 0,		///< 年 月 日
		DF_MMDDYY,			///< 月 日 年
		DF_DDMMYY			///< 日 月 年
	};

	/// 时间格式
	enum TimeFormat
	{
		TF_24	= 0,		///< 24小时
		TF_12				///< 12小时
	};

	/// 日期分隔符
	enum DateSpliter
	{
		DS_DOT	= 0,		//点分格式
		DS_DASH,			//杠分格式
		DS_SLASH			//斜线格式
	};

	/// 时间格式化类型
	enum TimeFormatType
	{
		fmtNormal = 0,
		fmtHalfYear = 0x0001,
		fmtNoSecond = 0x0002,
		fmtNoSign  = 0x0004,
		fmtOnlyDate = 0x0008,
		fmtOnlyTime = 0x0010,
		fmtFullAMPM = 0x0020
	};

	/// 默认构造函数
	CTime();

	/// 普通时间构造函数
	CTime(const SystemTime &time);

	/// 普通时间构造函数
	CTime( int vyear, int vmonth, int vday, int vhour, int vmin, int vsec);

	/// 时间调节
	/// \param seconds 调节的秒数
	/// \return 调节后新的时间对象
	CTime operator +(int64 seconds ) const;

	/// 时间调节
	/// \param seconds 调节的秒数
	/// \return 调节后新的时间对象
	CTime operator -(int64 seconds ) const;

	/// 时间调节
	/// \param seconds 调节的秒数
	/// \return 对象本身
	CTime& operator +=(int64 seconds );

	/// 时间调节
	/// \param seconds 调节的秒数
	/// \return 对象本身
	CTime& operator -=(int64 seconds );
	
	/// 时间差运算
	/// \param time 相比较的时间
	/// \return 比较的结果，为(*this - time)得到的秒数
	int64 operator -(const CTime& time ) const;

	/// 时间比较
	/// \param time 相比较的时间
	/// \return 相等返回true，否则返回false
	bool operator == (const CTime& time) const;

	/// 时间比较
	/// \param time 相比较的时间
	/// \return 不等返回true，否则返回false
	bool operator != (const CTime& time) const;

	/// 时间比较
	/// \param time 相比较的时间
	/// \return <返回true，否则返回false
	bool operator < (const CTime& time) const;

	/// 时间比较
	/// \param time 相比较的时间
	/// \return <=返回true，否则返回false
	bool operator <= (const CTime& time) const;

	/// 时间比较
	/// \param time 相比较的时间
	/// \return >返回true，否则返回false
	bool operator > (const CTime& time) const;

	/// 时间比较
	/// \param time 相比较的时间
	/// \return >=返回true，否则返回false
	bool operator >= (const CTime& time) const;

	/// 时间赋值
	CTime& operator = (const CTime& time);

	/// 得到相对时间，相对时间指从1900-1-1 00:00:00到某个时刻经过的秒数
	/// \return 相对时间
	time_t makeTime() const;

	/// 从相对时间得到绝对时间，和maketime相对应
	void breakTime(uint64 time);

	/// 时间格式化输出
	std::string format(uint fmt = fmtNormal) const;

	/// 获取当前系统时间
	static CTime getCurrentTime();

	/// 得到当前系统时间。这个函数可能调用的很频繁, 所有调用时应该是从内存中取得
	/// 时间,而不是直接从硬件, 在定时器里更新内存中的值.
	/// \retval true  成功
	/// \retval false 失败
	static bool getCurrentTime(SystemTime &time);

	/// 设置当前系统时间
	static void setCurrentTime(const CTime &time);

	/// 设置当前系统时间
	static void setCurrentTime(SystemTime &time);

	/// 得到系统的毫秒计数。两次调用之间的间隔时间就是得到的计数的差值。
	/// 
	/// \param 无
	/// \return 当前计数。
	/// \note 一般情况下，49.7天后得到计数会溢出，使用到的地方要做溢出后处理！
	static uint64 getCurrentMicroSecond();

	/// 获取当前系统时间的微妙数
	static uint64 getCurrentMillSecond();

	/// 休眠毫秒数
	static void sleep(int millsecond);

	//// 设置系统时间格式
	/// \param [in] dateFormat 日期格式
	/// \param [in] timeFormat 时间格式
	static void setTimeFormat(int dataFormat, int timeFormat, int timeSpliter);

	/// 获取系统时间格式
	static int getDateFormat();

	/// 获取当前时间格式
	static int getTimeFormat();

	/// 获取时间分割符
	static int getTimeSpliter();

	/// 得到指定月份的天数
	static int getDaysOfMonth(int year, int month);

	static int hour12To24(int hour12, int ampm);

	static int hour24To12(int hour24);

	static int hour24ToAMPM(int hour24);

	static bool isLeapYear(int year);

public:
	const static CTime minTime;		///< 系统最小时间
	const static CTime maxTime;		///< 系统最大时间

private:
	static int sm_dateFormat;
	static int sm_timeFormat;
	static int sm_timeSpliter;
};

void FormatTimeString(SystemTime *p,char* buf,int flag);
void FormatTimeStringDate(DateTime *p,char* buf,int flag);
void TimeDate2Sys(SystemTime * systime, DateTime * dateTime);
void TimeSys2Date(DateTime * dateTime, SystemTime * systime);
int TimeCompare(SystemTime * pSysTime1, SystemTime * pSysTime2);
void TimeAdd(SystemTime * pOldTime, SystemTime * pNewTime, int second);
int Time2Second(SystemTime * pSysTime1, SystemTime * pSysTime2);
int GetCurDateTime(DateTime* ptime);
void ClearDateTime(DateTime *ptime);
uint IsZeroDateTime(DateTime t);
int CompareDateTime(DateTime t1,DateTime t2);
int DateTimeToDay(DateTime t);
int DateTimeToSecond(DateTime t);
DateTime SecondToDateTime(int second);
int DateTimeChangeSecond(DateTime end_time, DateTime start_time);
int CompareDateTime(DateTime t, DateTime t1, DateTime t2);
DateTime AddDateTime(DateTime time, int second);

#endif
