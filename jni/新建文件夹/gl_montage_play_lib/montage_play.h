#ifndef _MONTAGE_PLAY_SDK_
#define _MONTAGE_PLAY_SDK_

#include "pw_datatype.h"
#include "pw_typedef.h"
#include "montage_pattern.h"

#ifdef VMDLL_EXPORTS
#define PMP_API __declspec(dllexport)
#else
#define PMP_API
#endif

/*----------------------------------------------------------------------------------------------
*	Macro & Enum definition 
*---------------------------------------------------------------------------------------------*/

#define PMP_IN
#define PMP_OUT
#define PMP_ERR_ID	(-1)

enum	MP_ERROR_TYPE{
	E_MP_ERR_OK				=0,
	E_MP_ERR_UNDEFINED		,
};

enum	MP_EVENT_TYPE{
	E_MP_EVN_NULL			=0,
	E_MP_EVN_CONTEXT_DEBUG	,
	E_MP_EVN_UNDEFINED		,
};

enum	MP_TEXTURE_TYPE{
	E_MP_EVN_LUMINACE	=0,
	E_MP_EVN_RGBA		,
	E_MP_EVN_YUV420		,
};

/*----------------------------------------------------------------------------------------------
*	Strut & Unit definition 
*---------------------------------------------------------------------------------------------*/

typedef struct _pw_mp_gles2_texid_
{
	MP_TEXTURE_TYPE	eTextureType;
	GInt32		lTextureID[4];
	GInt32		lTextureUnit[4];

}PMP_GLES2_TEXID;

typedef struct _pw_mp_cb_info_
{
	GInt32 lEngineID;
	GInt32 lSourceID;
	const GChar* pstrDescrib;
	GUInt16	by2Width;
	GUInt16	by2Height;
	GUInt16	by2FPS;
	GUInt8	byExtern[2];
	GUInt32	dwSampleRate;
}PMP_CB_INFO;

/*----------------------------------------------------------------------------------------------
*	API definition 
*---------------------------------------------------------------------------------------------*/
/*-- pcBuff is standard video or audio data  
	pstInfo->pstrDescrib ==
	"h264_SPS"
	"h264_PPS"
	"h264_I"
	"h264_P"
	"g711a"
	"PCM"
--*/
typedef GBool (*pmpfStreamCBFun)( const GInt8* pcBuff, GInt32 lBuffSize, PMP_CB_INFO* pstInfo, GUInt64 dw2UserData );

/*-- pcBuff is standard video or audio data  
	pstInfo->pstrDescrib:
	"LPGVM_SCREEN"		--	pcBuff = LPGVM_SCREEN
	"SurfaceTexture"	--	pcBuff = SurfaceTexture
--*/
typedef GBool (*pmpfVideoDecCBFun)( const GInt8* pcBuff, GInt32 lBuffSize, PMP_CB_INFO* pstInfo, GUInt64 dw2UserData );
/*-- pcBuff = PCM  --*/
typedef GBool (*pmpfAudioDecCBFun)( const GInt8* pcBuff, GInt32 lBuffSize, PMP_CB_INFO* pstInfo, GUInt64 dw2UserData );
/*-- EVENT
	Need a authorization
--*/
typedef GVoid (*pmpfEventCBFun)( GInt32 lEngineID, MP_EVENT_TYPE eEvnType, GUInt64 dw2Data, const GChar *pstrEventDescrib, GUInt64 dw2UserData );

typedef GVoid (*pmpfErrorCBFun)( GInt32 lEngineID, MP_ERROR_TYPE eErrType, const GChar *pstrErrorDescrib, GUInt64 dw2UserData );

PMP_API	GBool	pw_mp_QuickInit();
PMP_API	GBool	pw_mp_Uninit();
PMP_API GBool	pw_mp_GetVersion ( PMP_OUT const GChar* pstrVersion );
PMP_API GBool	pw_mp_ConfigComPath ( PMP_IN const GChar* pstrComPath );
PMP_API GBool	pw_mp_ConfigLibPath ( PMP_IN const GChar* pstrLibPath );

/*---------------------------------------------------
*	Component control
*--------------------------------------------------*/
	/*-- 
	* Load opengl pattern component, only one component as this type could be loaded
	pstrComponentType == 
		"gl_pattern"
		pstrComponentContext ==
		context string to config a output pattern include:
			data-source
			show model
			lib of pattern control component
			opengles 2.0 context ...

	* Load net component, only one component as this type could be loaded
	pstrComponentType == 
		"device_connect"
		pstrComponentContext ==
		context string to config a stream get component
			lib of net component

	* Load file ctrl component, only one component as this type could be loaded
	pstrComponentType == 
		"file_play"
		pstrComponentContext ==
		context string to config a file ctrl component
			lib of file component	

	* Load DPTZ component, only one component as this type could be loaded
	pstrComponentType == 
		"dptz"
		pstrComponentContext ==
		context string to config a file ctrl component
			lib of file component	
	--*/
	PMP_API GBool	pw_mp_LoadComponentContext(
		PMP_IN	const GChar*		pstrComponentType,
		PMP_IN	const GChar*		pstrComponentContext
		);

// 	/*-- Load a context string of pattern control component ?? --*/
// 	PMP_API GBool	pw_mp_LoadPatternContext(
// 		PMP_IN	const GChar*		pstrComponentName,
// 		PMP_IN	const GChar*		pstrPatternContext
// 		);

	/*-- Unload the component loaded currently ?? --*/
	PMP_API GBool	pw_mp_UnloadCurrentComponent(
		PMP_IN	const GChar*		pstrComponentType
		);

typedef struct PMP_MONTAGE_PLAY
{
	PMP_API PMP_MONTAGE_PLAY();
	PMP_API ~PMP_MONTAGE_PLAY();

	/*-- Call this API following the struct created.  --*/
	PMP_API GBool	pw_mp_Create();

	/*-- Destory the engine.  --*/
	PMP_API GBool	pw_mp_Destory();

	/*-- Get the only ID of the play engine.  --*/
	PMP_API GInt32	pw_mp_GetEngineID();

	/*-- Get a new application need to be sent to the authorization sever ?? --*/
	PMP_API GBool	pw_mp_GetApplication(
		PMP_IN	GInt32				lFunctionLevel,
		PMP_OUT	GChar*				pstrApplication
		);

	/*-- Import the authorization key received from the authorization sever ?? --*/
	PMP_API GBool	pw_mp_ImportAuthorization(
		PMP_IN	GChar*				pstrAuthorization
		);
	
	/*-- Add a new data source for some function.
		pstrTypeName ==
			"h264&g711a"
			"h264/avc"
			"g711a"
			"jpeg"
			"gvm_screen"
			"camera_01"
			"camera_02"	
			"mutiscale_stream_1"
		return SourceID
	--*/
	PMP_API GInt32	pw_mp_AddDataSource(
		PMP_IN	const GChar*		pstrSourceName,
		PMP_IN	const GChar*		pstrTypeName
		);
	PMP_API const GChar*	pw_mp_GetDataSourceName(
		PMP_IN	GInt32				lSourceID
		);
	PMP_API GInt32	pw_mp_GetDataSourceID(
		PMP_IN	const GChar*		pstrSourceName
		);

/*---------------------------------------------------
*	Decode API 
*--------------------------------------------------*/

	/*-- Set the parameters. 
	*Force the parameters of video.
		pstrInfoName ==
			"pano_type"		GUInt32
			"pano_typeex"	GUInt64
			"pano_tilt"		GFloat
			"pano_centerx"	GFloat
			"pano_centery"	GFloat
			"pano_radius"	GFloat
		 Engine will achieve these parameters from video stream if not call this API 

	*Stream parse info.
		pstrInfoName ==
		"queue_length"	
		pstrValue == unsinged int number such as "50"	

	*Type of dtream call back.
		 pstrInfoName ==
			"steamcb_type"	
			pstrValue ==
				"block"		----call stream call back function during the 'pw_mp_InputData' function running. 
							'pw_mp_InputData' function will be blocked. stream platform not work.
				"unblock"	----call stream call back function during the 'pw_mp_RunStep' function running or the play thread running.

	*Buff cache or not.
		pstrInfoName ==
			"buff_cache_size"	
			pstrValue ==
				"0"---- Close buff cache
				"n"---- >0:Open buff cache, real cache size will >= 1024*1024 

	*Method for queue is full.
		pstrInfoName ==
			"queue_full_method"	
			pstrValue ==
				"real_play"----Abandon some stream data when queue is full
				"full_play"----Wait for the free seat when queue is full
		
	*Control audio decode when parsing, support g711 only, nonsupport this function for video.
		pstrInfoName ==
			"audio_decode_parsing"	
			pstrValue ==
				"y"----Start decode audio, return in audio call back
				"n"----Stop decode audio

	*Control decode or stream call back whether following FPS
		pstrInfoName ==
			"follow_fps"	
			pstrValue ==
				"y"----follow fps
				"n"----as soon as possible

	*Control decode when playing, all the data-source default to be decoded if needed.
		pstrInfoName ==
			"video0"
			"video1"
			"audio"
			"data"
			pstrValue ==
				"decode"
				"nodecode"

	*Set stream info
	pstrInfoName ==
		"video0_fps"
			pstrValue == "n"---- > 0 
		"audio_samplerate"
			pstrValue == "n"---- > 0 

	*Clear cache and queue
	pstrInfoName ==
		"clear"
			pstrValue == 
				"all"
				"all_que"
				"cache"
				"v0_que"
				"v1_que"
				"au_que"
				"data_que"				
	--*/
	PMP_API GBool	pw_mp_SetInfo(
		PMP_IN	GInt32			lSourceID,
		PMP_IN	const GChar*	pstrInfoName,
		PMP_IN	const GChar*	pstrValue
		);

	/*-- Force the parameters of video. 
		pstrPanoInfoName:
			"pano_type"		GUInt32
			"pano_typeex"	GUInt64
			"pano_tilt"		GFloat
			"pano_centerx"	GFloat
			"pano_centery"	GFloat
			"pano_radius"	GFloat
		 Engine will achieve these parameters from video stream if not call this API 
	--*/
// 	PMP_API GBool	pw_mp_ResetPanoType(
// 		PMP_IN	GInt32			lSourceID,
// 		PMP_IN	const GChar*	pstrPanoInfoName,
// 		PMP_IN	const GChar*	pstrValue
// 		);

	/*-- Set the buff size as number of frame. --*/
	PMP_API GBool	pw_mp_SetBufPoolSize(
		PMP_IN	GInt32			lSourceID,
		PMP_IN	GUInt32			dwBufPoolSize
		);

// 	/*-- Set information of video by force ?? 
// 		pstrVideoInfoName:
// 			"FPS"
// 	--*/
// 	PMP_API GBool	pw_mp_SetVideoInfo(
// 		PMP_IN	GInt32			lSourceID,
// 		PMP_IN	const GChar*	pstrVideoInfoName,
// 		PMP_IN	GUInt64			dw2Data
// 		);

// 	/*-- Set the type of standard stream callback.
// 		bStreamCBImmediately:
// 			GFalse:	call stream call back function during the 'pw_mp_RunStep' function running or the play thread running.
// 			GTrue:	call stream call back function during the 'pw_mp_InputData' function running. 
// 					'pw_mp_InputData' function will be blocked. stream platform not work.
// 	--*/
// 	PMP_API GBool	pw_mp_SetSteamCBType( 
// 		PMP_IN	GInt32				lSourceID,
// 		PMP_IN	GBool				bStreamCBImmediately
// 		);

	/*-- Set the standard stream callback function. --*/
	PMP_API GBool	pw_mp_SetSteamCallBack( 
		PMP_IN	GInt32				lSourceID,
		PMP_IN	pmpfStreamCBFun		cbStreamCBFun,
		PMP_IN	GUInt64				dw2UserData
		);

	/*-- Set the frame output callback function. --*/
	PMP_API GBool	pw_mp_SetVideoDecCallBack( 
		PMP_IN	GInt32				lSourceID,
		PMP_IN	pmpfVideoDecCBFun	cbVideoDecFun,
		PMP_IN	GUInt64				dw2UserData
		);

	/*-- Set the PCM output callback function. --*/
	PMP_API GBool	pw_mp_SetAudioDecCallBack( 
		PMP_IN	GInt32				lSourceID,
		PMP_IN	pmpfAudioDecCBFun	cbAudioDecFun, 
		PMP_IN	GUInt64				dw2UserData 
		);

	/*-- Set the event output callback function. --*/
	PMP_API GBool	pw_mp_SetEventCallBack( 
		PMP_IN	pmpfEventCBFun		cbEventDecFun, 
		PMP_IN	GUInt64				dw2UserData 
		);

	/*-- Set the error output callback function. --*/
	PMP_API GBool	pw_mp_SetErrorCallBack( 
		PMP_IN	pmpfErrorCBFun		cbErrorDecFun, 
		PMP_IN	GUInt64				dw2UserData 
		);

	/*-- Run step by step in the thread which created by client. --*/
	PMP_API GBool	pw_mp_RunStep(
		PMP_IN	GInt32			lSourceID
		);

	/*-- Start the decode thread. --*/
	PMP_API GBool	pw_mp_Play(
		PMP_IN	GInt32			lSourceID
		);

	/*-- Pause the decode thread, output the last frame as FPS. --*/
	PMP_API GBool	pw_mp_Pause(
		PMP_IN	GInt32			lSourceID
		);

	/*-- Stop the decode thread, end the frame output. --*/
	PMP_API GBool	pw_mp_Stop(
		PMP_IN	GInt32			lSourceID
		);

	/*-- Input streamData.
		pstrType:
			"mix_stream_pw1"
			"mix_stream_pw2"
			"video_h264"
			"video_h264_single"
			"audio_g711a"
			"video_h264_pw1"
			"video_h264_single_pw1"
			"audio_g711a_pw1"
			"video_h264_pw2"
			"video_h264_single_pw2"
			"audio_g711a_pw2"
			"jpeg"
			"gvm_screen"
	--*/
	PMP_API GBool	pw_mp_InputData( 
		PMP_IN	GInt32				lSourceID,
		PMP_IN	const GInt8*		pcBuff, 
		PMP_IN	GUInt32				lBuffSize, 
		PMP_IN	const GChar*		pstrType
		);

	/*-- Capture the original image of video ?? --*/
	PMP_API GBool	pw_mp_Capture( 
		PMP_IN	GChar*				pstrFilePath
		);

	/*-- Record the original video ?? --*/
	PMP_API GBool	pw_mp_Record( 
		PMP_IN	GChar*				pstrFilePath
		);
	PMP_API GBool	pw_mp_StopRecord();

/*---------------------------------------------------
*	OpenGL es 2.0 Show API 
*--------------------------------------------------*/
	PMP_API	GBool	pw_mpgl_Init(); 

	PMP_API	GBool	pw_mpgl_Uninit(); 

	PMP_API	GBool	pw_mpgl_SetInfo(
		PMP_IN	const GChar*	pstrInfoName,
		PMP_IN	const GChar*	pstrValue
		); 

	PMP_API	GBool	pw_mpgl_GetInfo(
		PMP_IN	const GChar*	pstrInfoName,
		PMP_OUT	const GChar**	ppstrValue
		); 

	PMP_API	GBool	pw_mpgl_GetPatternWithTime( 
		PMP_IN GUInt32			dwTickCount, 
		PMP_IN const GSIZE*		pstScreenSize, 
		PMP_OUT PW_PATTERN_OUT*	pstPatternOut
		);

	PMP_API	GBool	pw_mpgl_PatternCtrlFingerDown( 
		PMP_IN GLPOINTINSCREEN*	pstFingerList, 
		PMP_IN GUInt32			dwFingerCount, 
		PMP_IN GUInt32			dwTickCount
		);
	/*-- Set the Physical Vector 
		*Set gravity
		pstrInfoName ==
			"gravity"
			pstVector.x/y/z	illegal
			pstVector.x * pstVector.x + pstVector.y * pstVector.y + pstVector.z * pstVector.z = 9.82 * 9.82

		*Set acceleration
		pstrInfoName ==
			"acceleration"
			pstVector.x/y/z	illegal		
	--*/
	PMP_API	GBool	pw_mpgl_PhysicalVector( 
		PMP_IN	const GChar*		pstrInfoName,
		PMP_IN	GUInt32				dwTickCount,
		PMP_IN	GFPOINT4D*			pstVector
		);

/*---------------------------------------------------
*	File play
*--------------------------------------------------*/

	PMP_API	GBool	pw_mpfl_Openfile(
		PMP_IN	const GChar*		pstrFilePath
		); 

	PMP_API	GBool	pw_mpfl_Closefile(); 

	/*-- Set information
		*Ctrl play
		pstrInfoName ==	
		"play"
		"pause"
		"stop"
		"fast"
		"slow"
		"back"
		"step"

		*Set time length
		pstrInfoName ==	"file_length"	
			pstrValue == file length by byte
		pstrInfoName ==	"file_time"
			pstrValue == file time by second
	--*/
	PMP_API	GBool	pw_mpfl_SetInfo(
		PMP_IN	const GChar*	pstrInfoName,
		PMP_OUT	const GChar*	pstrValue
		); 
	/*-- Get information
		*Get file length
		pstrInfoName ==	"file_length"

		*Get time length
		pstrInfoName ==	"file_time"	
	--*/
	PMP_API	GBool	pw_mpfl_GetInfo(
		PMP_IN	const GChar*	pstrInfoName,
		PMP_OUT	const GChar**	ppstrValue
		); 

private:
	GInt32	m_lEngineID;


}*LP_PMP_MONTAGE_PLAY;



#endif	//	_MONTAGE_PLAY_SDK_
