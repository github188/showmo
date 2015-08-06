package com.showmo.rxErr;

public class NetErrInfo extends Exception {
	public long netErrCode;
	public NetErrInfo(){
		
	}
	public NetErrInfo(long errcode){
		netErrCode=errcode;
	}
}
