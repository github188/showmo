package com.showmo.network;

public class ResponseInfo {
	long errorCode ;

	boolean isSuccess ;
	long dateLong ;
	
	public boolean boolValue;
	Object obj;

	public ResponseInfo() {

	}


	public boolean  isSuccess() {
		return isSuccess;
	}
	public void setIsSuccess(Boolean isSuccess) {
		this.isSuccess = isSuccess;
	}


	public long getErrorCode() {
		return errorCode;
	}


	public void setErrorCode(long errorCode) {
		this.errorCode = errorCode;
	}


	public long getDateLong() {
		return dateLong;
	}


	public void setDateLong(long dateLong) {
		this.dateLong = dateLong;
	}
	public Object getObj() {
		return obj;
	}
	public void setObj(Object obj) {
		this.obj = obj;
	}
 
	

}
