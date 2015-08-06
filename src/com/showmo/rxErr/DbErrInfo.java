package com.showmo.rxErr;

public class DbErrInfo extends Exception {
	public long dbErrCode;
	public DbErrInfo(){
		
	}
	public DbErrInfo(long errcode){
		dbErrCode=errcode;
	}
}
