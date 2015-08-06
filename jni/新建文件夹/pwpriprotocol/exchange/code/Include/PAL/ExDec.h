#ifndef __EX_DECODE_20110812_H
#define __EX_DECODE_20110812_H
#ifdef __cplusplus
extern "C"
{
#endif
#define MAX_HVR_CHNCAP			32

typedef struct HVR_CHNCAP 
{
	int nD1Chn;		// 支持的D1路数
	int n960HChn;	// 支持的960H路数
	int n720PChn;	// 支持的720P路数
	int n1080PChn;	// 支持的1080P路数
	int nCIFChn;	//支持的CIF通道数
	int nHD1Chn;	//支持的HD1通道数
	int nRes[2];
}HVR_CHNCAP, *PHVR_CHNCAP;

typedef struct CAPTURE_HVRCAP 
{
	HVR_CHNCAP DigitalCap;		// 支持的数字通道信息
	HVR_CHNCAP AnalogCap;		// 支持的模拟通道信息
}CAPTURE_HVRCAP, *PCAPTURE_HVRCAP;

typedef struct CAPTURE_TOTAL_HVRCAP 
{
	int		nHVRCap;			// 实际支持的模式
	CAPTURE_HVRCAP	HVRCap[MAX_HVR_CHNCAP];		// 所有模式的汇总
}CAPTURE_TOTAL_HVRCAP, *PCAPTURE_TOTAL_HVRCAP;
 
/// 设置HVR的应用模式
/// 
/// \param [out] pHVRCap 指向HVR特性结构CAPTURE_HVRCAP的指针。
/// \retval 0  获取成功。
/// \retval -1  获取失败。
int CaptureSetHVRCap(const CAPTURE_HVRCAP *pHVRCap);
 
//! 获得当前HVR的应用模式
/// 
/// \param [out] pHVRCap 指向HVR特性结构CAPTURE_HVRCAP的指针。
/// \retval 0  获取成功。
/// \retval -1  获取失败。
int CaptureGetCurHVRCap(CAPTURE_HVRCAP *pHVRCap);
 
//! 获得HVR支持的应用模式
/// 
/// \param [out] pHVRCap 指向HVR特性结构CAPTURE_HVRCAP的指针。
/// \retval 0  获取成功。
/// \retval -1  获取失败。
int CaptureGetTotalHVRCap(CAPTURE_TOTAL_HVRCAP *pHVRCap);
 
 
// HVR的接口做了一些调整，3个接口，一个获取支持的所有工作模式，一个设置工作模式，一个获取当前工作模式

#ifdef __cplusplus
}
#endif

#endif

