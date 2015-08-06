package com.showmo.addHelper;

import com.showmo.deviceManage.Device;

public class ReConfigProcess extends AbstractAddProcess {

	@Override
	protected boolean onDevStateFilter(DEV_BIND_STATE state) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	protected Device performBindBySelfDev(DevSimpleInfo sdevinfo) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	protected Device performBindByOtherDev(DevSimpleInfo sdevinfo) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	protected Device performBindByNoBodyDev(DevSimpleInfo sdevinfo) {
		// TODO Auto-generated method stub
		return null;
	}

}
