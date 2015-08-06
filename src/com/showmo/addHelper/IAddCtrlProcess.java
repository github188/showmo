package com.showmo.addHelper;

public interface IAddCtrlProcess {
	public void beginWork(String ssid,String psw,String keyType);
	public void pauseWork();
	public void continueWork();
	public void exitWork();
}
