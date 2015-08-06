package com.showmo.ormlite.model;

import com.j256.ormlite.field.DatabaseField;
import com.j256.ormlite.table.DatabaseTable;
import com.showmo.ormlite.dao.impl.AccountDaoImpl;

@DatabaseTable(daoClass = AccountDaoImpl.class,tableName="showmo_account")
public class ShowmoAccount  {
	

	
	@DatabaseField(id = true)
	private String userName ;
	
	@DatabaseField
	private String psssword ;
	
	@DatabaseField
	private int accountState ;
	
	@DatabaseField
	private boolean isSavePsw ;

	public String getUserName() {
		return userName;
	}

	public void setUserName(String userName) {
		this.userName = userName;
	}

	public String getPsssword() {
		return psssword;
	}

	public void setPsssword(String psssword) {
		this.psssword = psssword;
	}


	public boolean isSavePsw() {
		return isSavePsw;
	}

	public void setSavePsw(boolean isSavePsw) {
		this.isSavePsw = isSavePsw;
	}

	public int getAccountState() {
		return accountState;
	}

	public void setAccountState(int accountState) {
		this.accountState = accountState;
	}
	

	public ShowmoAccount(){
		
	}

	
	public ShowmoAccount(String userName, String psssword, int accountState,
			boolean isSavePsw) {
		super();
		this.userName = userName;
		this.psssword = psssword;
		this.accountState = accountState;
		this.isSavePsw = isSavePsw;
	}

	@Override
	public String toString() {
		return "ShowmoAccount [userName=" + userName + ", psssword=" + psssword
				+ ", accountState=" + accountState + ", isSavePsw=" + isSavePsw
				+ "]";
	}
	
	

	 
 
	
}
