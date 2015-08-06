package com.showmo.safe;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.base.ShowmoApplication;
import com.showmo.ormlite.dao.impl.SafeDaoImpl;
import android.content.Context;

import com.j256.ormlite.field.DatabaseField;
import com.j256.ormlite.table.DatabaseTable;

@DatabaseTable(daoClass = SafeDaoImpl.class, tableName = "tb_safe")
public class Safe implements Serializable{
	public static Safe High_safe_level=new Safe("¸ß",true,false,false);
	public static Safe Mid_safe_level=new Safe("ÖÐ",true,true,false);
	public static Safe Low_safe_level=new Safe("µÍ",true,true,true);
	
	public static List<Safe> getSysLevel(){
		List<Safe> safeList=new ArrayList<Safe>();
		safeList.add(High_safe_level);
		safeList.add(Mid_safe_level);
		safeList.add(Low_safe_level);
		return safeList;
	}
	
	public static List<SafeType> getSafeTypesValue(Safe safe){
		List<SafeType> types=new ArrayList<Safe.SafeType>();
		Context context=ShowmoApplication.getInstance();
		String describ1=context.getResources().getString(R.string.allow_others_login);
		String describ2=context.getResources().getString(R.string.allow_cloud_manage);
		types.add(new SafeType("allow_others_loading_your_account", describ1, safe.allow_others_loading_your_account));
		types.add(new SafeType("allow_video_up_to_cloude", describ2, safe.allow_video_up_to_cloude));
		return types;
	}
	
	public Safe(){
		this.safeName="";
		this.allow_others_loading_your_account=false;
		this.allow_video_up_to_cloude=false;
		this.SysLevel=false;
	}
	
	public Safe(String safename,boolean isSysLevel,boolean allow_video_up_to_cloude,boolean allow_others_loading_your_account) {
		// TODO Auto-generated constructor stub
		this.safeName=safename;
		this.allow_others_loading_your_account=allow_others_loading_your_account;
		this.allow_video_up_to_cloude=allow_video_up_to_cloude;
		SysLevel=isSysLevel;
	}
	@DatabaseField(id=true)
	public String safeName;
	@DatabaseField
	public boolean allow_video_up_to_cloude;
	@DatabaseField
	public boolean allow_others_loading_your_account;
	@DatabaseField
	public boolean SysLevel=true;

//	public boolean selected=false;
	@Override
	public String toString() {
		return "Safe [safeName=" + safeName + ", allow_video_up_to_cloude="
				+ allow_video_up_to_cloude
				+ ", allow_others_loading_your_account="
				+ allow_others_loading_your_account + "]";
	}
	
	
	public static class SafeType{
		public String fieldName;
		public String fieldDescribe;
		public boolean value=false;
		public SafeType(String fieldName, String fieldDescribe,
				boolean selected) {
			super();
			this.fieldName = fieldName;
			this.fieldDescribe = fieldDescribe;
			this.value = selected;
		}
		
	}
}
