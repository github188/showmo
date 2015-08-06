#ifndef __EXCHANGE_PCS_H
#define __EXCHANGE_PCS_H

#include <string>
#include "../Infra/Types.h"

struct DeviceCode
{
	std::string device_code;
	std::string user_code;
	std::string verification_url;
	std::string qrcode_url;
	int expires_in;  //Device_code/User_code 过期时间,单位为s
	int interval;    //获取Access_token间隔,单位为s	
};

struct AccessTokenCode
{
	std::string access_token;
	int expires_in;   			//Access Token的有效期，以秒为单位
	std::string refresh_token;	 
	std::string scope;
	std::string session_key;
	std::string session_secret;
};

struct QuotaCode		////空间配额信息
{
	uint quota;		////空间配额,单位字节
	uint used;		////已使用空间,单位字节
};

struct UploadCode			////上传文件
{
	std::string path;		////文件路径
	uint size;			////文件字节大小
	uint ctime;			////文件创建时间	
	uint mtime;			////文件修改时间
	std::string md5;		////文件md5签名	
	uint fs_id;			////文件在PCS的临时唯一标识ID
};

struct SliceUploadCode	    ////分片上传--文件分片及上传
{
	std::string md5;
};

struct MergeFileCode		////分片上传--合并文件
{
	std::string path;		////文件路径
	uint size;			////文件字节大小
	uint ctime;			////文件创建时间	
	uint mtime;			////文件修改时间
	std::string md5;		////文件md5签名	
	uint fs_id;			////文件在PCS的临时唯一标识ID
};

struct BaiduPcsConfig
{
	int 	action;		// 0:绑定; 1: 解除绑定; 2: 设置参数
	bool  bind;		// 0:未绑定; 1: 已绑定
	char 	userCode[32];
	int	bConfirm;	// 绑定过程的操作步骤标识，0表示第一步获取usercode
	bool	fileDetect;
	bool	fileAlarm;
	bool	fileBlind;
	bool	snapDetect;
	bool	snapAlarm;
	bool	snapBlind;
};


#endif
