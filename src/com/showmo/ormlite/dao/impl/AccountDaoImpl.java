package com.showmo.ormlite.dao.impl;

import java.sql.SQLException;
import java.util.List;

import android.util.Log;

import com.j256.ormlite.dao.BaseDaoImpl;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.table.DatabaseTableConfig;
import com.showmo.ormlite.dao.AccountDao;
import com.showmo.ormlite.model.ShowmoAccount;
import com.showmo.util.StringUtil;

public class AccountDaoImpl extends BaseDaoImpl<ShowmoAccount, String>
implements AccountDao{



	public AccountDaoImpl(Class<ShowmoAccount> dataClass) throws SQLException {
		super(dataClass);

	}



	public AccountDaoImpl(ConnectionSource connectionSource,
			Class<ShowmoAccount> dataClass) throws SQLException {
		super(connectionSource, dataClass);
	}



	public AccountDaoImpl(ConnectionSource connectionSource,
			DatabaseTableConfig<ShowmoAccount> tableConfig) throws SQLException {
		super(connectionSource, tableConfig);
	}



	public int insertAccount(ShowmoAccount account)  {
		if(account == null){
			return 0 ;
		}
		try {
			Log.e("AccountDaoImpl", "insert-->"+account.toString());
			return create(account);
 
		} catch (SQLException e) {
			e.printStackTrace();
			return 0 ;
		}


	}

	public int updateAccount(ShowmoAccount account){
		if(account == null){
			return 0 ;
		}

		try {
			Log.e("AccountDaoImpl", "update-->"+account.toString());
			return update(account);
		} catch (SQLException e) {
			e.printStackTrace();
			return 0;
		}





	}


	@Override
	public List<ShowmoAccount> queryByUserName(String userName) {
		if(!StringUtil.isNotEmpty(userName)){
			return null ;
		}
		List<ShowmoAccount> res = null ;
		try {
			res = queryForEq("userName", userName);
		} catch (SQLException e) {
			e.printStackTrace();
		}
	 
		if(res != null && res.size()!= 0){
			Log.e("AccountDaoImpl", "queryByUserName-->"+res.get(0).toString());
			return res ;
		}else{
			return null;
		}

		
	}

	





	@Override
	public List<ShowmoAccount> queryForAllAccount() {
		List<ShowmoAccount> res = null ;
		try {
			res = queryForAll();
		} catch (SQLException e) {
			e.printStackTrace();
		}

		if(res != null && res.size()!= 0){
			Log.e("AccountDaoImpl","queryForAllAccount.size-->"+ res.size());
			return res ;
		}else{
			return null;
		}
	}




}
