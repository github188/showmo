package com.showmo.playHelper;

import com.showmo.deviceManage.Device;
import com.showmo.playHelper.IDevicePlayer.EnumRealplayCmd;

import ipc365.app.showmo.jni.JniDataDef.OnRealdataCallBackListener;

public class RealplayInParams {
	private EnumRealplayCmd[] m_execCmdOrder = null;
	private Device m_devInfo = null;
	private OnRealdataCallBackListener m_dataCallback = null;

	RealplayInParams(EnumRealplayCmd[] cmd, Device dev,
			OnRealdataCallBackListener cb) {
		m_execCmdOrder = cmd;
		m_devInfo = dev;
		m_dataCallback = cb;
	}

	EnumRealplayCmd[] getExecCmdOrder() {
		return m_execCmdOrder;
	}

	void setExecCmdOrder(EnumRealplayCmd[] m_execCmdOrder) {
		this.m_execCmdOrder = m_execCmdOrder;
	}

	Device getDevInfo() {
		return m_devInfo;
	}

	void setDevInfo(Device m_devInfo) {
		this.m_devInfo = m_devInfo;
	}

	OnRealdataCallBackListener getDataCallback() {
		return m_dataCallback;
	}

	void setDataCallback(OnRealdataCallBackListener m_dataCallback) {
		this.m_dataCallback = m_dataCallback;
	}
}
