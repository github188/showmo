package com.showmo.addHelper;

public interface IAddProcess extends IAddCtrlProcess{
	public void setAddListener(IAddListener lis);
	public IAddListener getAddListener();
}
