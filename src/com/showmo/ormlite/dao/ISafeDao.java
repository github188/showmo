package com.showmo.ormlite.dao;

import java.util.List;

import com.showmo.deviceManage.Device;
import com.showmo.safe.Safe;

public interface ISafeDao {
	public Safe queryBySafeName(String safeName);
	public List<Safe> queryAllSafeLevel();
	public boolean RemoveBySafeName(String safeName);
	public boolean Remove(List<Safe> data);
	public boolean insertSafe(Safe data);
}
