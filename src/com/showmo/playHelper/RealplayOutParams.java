package com.showmo.playHelper;

import com.showmo.deviceManage.Device;
import com.showmo.playHelper.IDevicePlayer.EnumRealplayCmd;

import android.R.integer;
import android.R.interpolator;

public class RealplayOutParams {
	private EnumRealplayCmd m_curOrder;//当前执行完成的命令
	private boolean execRes;//执行结果
	private int errcode=0;
	private int arg1=0;
	private Object obj=null;
	private boolean arg2=false;
	private Device device;
	
	public Device getDevice() {
		return device;
	}
	public void setDevice(Device device) {
		this.device = device;
	}
	public boolean isArg2() {
		return arg2;
	}
	public void setArg2(boolean arg2) {
		this.arg2 = arg2;
	}
	public RealplayOutParams(EnumRealplayCmd order,boolean res){
		m_curOrder=order;
		execRes=res;
	}
	public RealplayOutParams(EnumRealplayCmd order,boolean res,int errcode){
		m_curOrder=order;
		execRes=res;
		this.errcode=errcode;
	}
	public EnumRealplayCmd getCurOrder() {
		return m_curOrder;
	}
	public void setCurOrder(EnumRealplayCmd m_curOrder) {
		this.m_curOrder = m_curOrder;
	}
	public boolean isExecRes() {
		return execRes;
	}
	public void setExecRes(boolean execRes) {
		this.execRes = execRes;
	}
	public int getErrcode() {
		return errcode;
	}
	public void setErrcode(int errcode) {
		this.errcode = errcode;
	}
	public int getArg1() {
		return arg1;
	}
	public void setArg1(int arg1) {
		this.arg1 = arg1;
	}
	public Object getObj() {
		return obj;
	}
	public void setObj(Object obj) {
		this.obj = obj;
	}
	
	
}
