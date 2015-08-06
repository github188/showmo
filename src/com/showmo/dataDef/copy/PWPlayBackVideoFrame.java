
package com.showmo.dataDef.copy;
import java.lang.String;

import android.text.format.Time;
public class PWPlayBackVideoFrame {
	public PWPlayBackVideoFrame(Time bTime,Time eTime,String fileName,int fileType,int size,int cameraId){
		beginDateTime=bTime;
		endDateTime=eTime;
		sFileName=fileName;
		nFileType=fileType;
		videoSize=size;
		nCameraId=cameraId;
	}
	public Time beginDateTime;//视频开始时间
	public Time endDateTime;//视频结束时间
	public String sFileName;
	public int  nFileType;				    // 文件类型 枚举：Remote_File_Type
	public int videoSize;
	public int  nCameraId;
}
