#ifndef __PAL_GARPHICS_H__
#define __PAL_GARPHICS_H__

#include "Types/Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup GraphicsAPI API Graphics
/// 简单图形设备接口，获取FrameBuffer（帧缓冲）描述，设置Overlay（叠加）参数等等。
/// 渲染部分由上层实现，暂时不支持硬件加速。
/// \n 调用流程图:
/// \code
///    =========================================
///                   |                         
///             CreateGraphicsLayer 
///					  |
/// 				 Start
///         +---------|
///			|	UpdateSurface
///			|		  |
///         |     SetAlpha
///         |         |
///         |    SetKeycolor
///         |         |
///         +---------|
///					 Stop                  
///                   |                         
///    =========================================
/// \endcode
/// @{

/// 图形颜色格式类型
typedef enum GraphicsColorFormat{  
	graphicsColorRGB555,	///< 16位表示一个象素。颜色分量位掩码为XRRRRRGG GGGBBBBB。
	graphicsColorRGB565,	///< 16位表示一个象素。颜色分量位掩码为RRRRRGGG GGGBBBBB。
	graphicsColorRGB888,	///< 24位表示一个象素。颜色分量位掩码为RRRRRRRR GGGGGGGG BBBBBBBB。
} GraphicsColorFormat;

/// 图形颜色格式类型
typedef enum GraphicsLayerType {  
	graphicsPrimary,		///< 主图层。
	graphicsCursor,			///< 光标图层。
	graphicsButt,
} GraphicsLayerType;


/// 图形帧表面结构，128字节
typedef struct GraphicsSurface
{
	uchar *mem;			///< 主显存块指针。
	uchar *mem0;		///< 次显存块指针，显示时现在次显示快上显示，完成后Flip操作即可在主表面显示出来。
						///< 如果不支持翻转只需将mem0设置成mem,Filp函数为空，GUI会自动操作主表面。
	uchar *mem1;		///< 保留。
	int pitch;			///< 跨度，即显存一行扫描线的开始到下一行扫描线的开始存储空间的偏移，字节为单位。
	int width;			///< 面宽度，象素为单位。
	int height;			///< 面高度，象素为单位。
	int format;			///< 颜色格式，取GraphicsColorFormat类型值的值。
	int reserved[25];	///< 保留字节
} GraphicsSurface;

/// 图形层接口结构，512字节。
/// \detail 视频输出接口可能包含多个层，图形层是其中的一个，分辨率随视频输出分辨率变化。
typedef struct GraphicsLayer
{
	/// 底层私有数据
	void *priv;	


	/// 增加接口引用
	int (*addRef)(struct GraphicsLayer *thiz);


	/// 释放接口
	int (*release)(struct GraphicsLayer *thiz);


	/// 得到图形层全局表面。视频制式改变或者视频输出分辨率改变时可能会改变，GDI需要重新获取。
	/// 
	/// \param [in] thiz 图形层接口指针
	/// \param [out] surface 指向显示缓冲特征结构GraphicsSurface的指针。GDI直接通过
	///        显存块指针访问显存。图形设备只能选择GDI支持的颜色格式中的一种，如果GDI
	///        都不支持，就需要增加新的颜色格式，并扩充GDI功能。
	/// \retval 0  获取成功
	/// \retval <0  获取失败
	int (*getSurface)(struct GraphicsLayer *thiz, GraphicsSurface *surface);


	/// 设置图形的透明度。图形与图像在全屏范围内按此透明度叠加。
	/// 
	/// \param [in] thiz 图形层接口指针
	/// \param [in] alpha 要设置的当前透明度，值越低表示叠加时图像颜色的分量越低，也就
	///        越透明，取值0-255。
	/// \param [in] delta 透明度自动渐变值，每帧时间透明度加上此值，直到透明度变成最大
	///        或最小为止。值为0表示不渐变。
	/// \retval 0  设置成功
	/// \retval <0  设置失败
	int (*setAlpha)(struct GraphicsLayer *thiz, int alpha, int delta);


	/// 设置锁结颜色区域。图形与图像按锁结颜色叠加时，图像是源层，图形是目的层，图形的锁
	/// 结色区域将被视频替换。
	/// 
	/// \param [in] thiz 图形层接口指针
	/// \param [in] dwKeyLow 对应锁结色区域的颜色分量最小值，16进制表示为0x00BBGGRR，最高字节保留。
	/// \param [in] dwKeyHigh 对应锁结色区域的颜色分量最大值，16进制表示为0x00BBGGRR，最高字节保留。
	/// \retval 0  设置成功
	/// \retval <0  设置失败
	int (*setKeyColor)(struct GraphicsLayer *thiz, int keylow, int keyhigh);


	/// 设置硬件光标热点位置，但GDI一般使用软件光标，这个接口主要是供Windows模拟程序使用。
	///
	/// \param [in] thiz 图形层接口指针
	/// \param [in] x x坐标
	/// \param [in] y y坐标
	/// \retval 0  设置成功
	/// \retval <0  设置失败
	int (*setCursorPos)(struct GraphicsLayer *thiz, int x, int y);


	/// 显示、隐藏GUI图层
	///
	/// \param [in] thiz 图形层接口指针
	/// \param [in] nShow 是否显示图层
	/// \retval 0  设置成功
	/// \retval <0  设置失败
	int (*showLayer)(struct GraphicsLayer *thiz, int nShow);


	/// 图层翻转,实现GUI的双缓冲，上层将整个页面的数据写到后背缓冲以后调用该接口将后背缓冲中的数据显示出来
	/// \param [in] thiz 图形层接口指针
	/// \param [in] srcRect 要翻转的显示区域
	/// \retval 0  设置成功
	/// \retval <0  设置失败
	int (*flip)(struct GraphicsLayer *thiz, const Rect *srcRect, int dwRop);
	
	/// 图层填充操作
	/// \param [in] thiz 要填充的图层
	/// \param [in] rect 要填充的图层区域
	/// \param [in] ulFillData 要填充的数据
	/// \retval 0  设置成功
	/// \retval <0  设置失败
	int (*blt)(struct GraphicsLayer *thiz, const Rect *rect, ulong ulFillData, int dwRop);

	/// 保留
	void *reserved[119];
} GraphicsLayer;


/// 图形层接口描述，128字节
typedef struct GraphicsLayerDesc
{
	int layerType;		///< 图层类型
	int reserved[31];	///< 保留
} GraphicsLayerDesc;


/// 创建图形层接口
///
/// \param [in] desc 图形层接口描述结构GraphicsLayerDesc指针
/// \param [out] layer 图形层接口指针的指针
/// \retval <0 创建失败
/// \retval 0 创建成功
int createGraphicsLayer(GraphicsLayerDesc *desc, GraphicsLayer **layer);

/// @} end of group

#ifdef __cplusplus
}
#endif
#endif
