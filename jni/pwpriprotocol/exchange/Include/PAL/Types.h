
#ifndef __PAL_TYPES_H__
#define __PAL_TYPES_H__

/// 系统时间结构
typedef struct tagSYSTEM_TIME
{
	int  year;		///< 年。   
	int  month;		///< 月，January = 1, February = 2, and so on.   
	int  day;		///< 日。   
	int  wday;		///< 星期，Sunday = 0, Monday = 1, and so on   
	int  hour;		///< 时。   
	int  minute;	///< 分。   
	int  second;	///< 秒。   
	int  isdst;		///< 夏令时标识。   
} SYSTEM_TIME;

/// 视频区域结构
/// - 当表示整个视频区域的坐标时, 使用图形坐标体系, 也就是是指在图形坐标体系下视频边界
/// 的坐标, 结构成员的取值与图形分辨率, 图形和视频叠加时的偏移和缩放参数, 当前分割模
/// 式等都有关系;
/// - 当表示的是整个视频区域中的一个子区域, 则使用相对坐标, 相对于一个虚拟的分辨率
/// (0, 0, RELATIVE_MAX_X, RELATIVE_MAX_Y), 使用相对坐标可以在不知道实际视频分辨率的
/// 情况下设定视频的子区域, 可以保证一次转化误差在1象素以内. 转换公式如下(假设绝对分
/// 辨率为aw*ah):
/// 绝对坐标(ax, ay)转换成相对坐标(rx, ry):
/// \code
/// rx = ax * RELATIVE_MAX_X / aw;
/// ry = ay * RELATIVE_MAX_Y / ah;
/// \endcode
/// 相对坐标(rx, ry)转换成绝对坐标(ax, ay):
/// \code
/// ax = rx * aw / RELATIVE_MAX_X;
/// ay = ry * ah / RELATIVE_MAX_Y;
/// \endcode
/// 相对分辨率定义如下:
/// \code
/// #define RELATIVE_MAX_X 8192
/// #define RELATIVE_MAX_Y 8192
/// \endcode
///
typedef struct VIDEO_RECT
{
	unsigned short	left;			///< 左上角x坐标。
	unsigned short	top;			///< 左上角y坐标。
	unsigned short	right;			///< 右下角x坐标。
	unsigned short	bottom;			///< 右下角y坐标。
}VIDEO_RECT;

#endif
