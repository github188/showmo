package com.showmo.ormlite.dao;

import java.util.List;

import com.showmo.ormlite.model.ShowmoAccount;


public interface AccountDao {
	public int insertAccount(ShowmoAccount account)  ;
	
	public List<ShowmoAccount> queryByUserName( String userName);
	
	public List<ShowmoAccount> queryForAllAccount() ;
	
	public int updateAccount(ShowmoAccount account);
}
