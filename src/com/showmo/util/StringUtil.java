
package com.showmo.util;

import java.util.ArrayList;
import java.util.List;

import android.R.interpolator;

public class StringUtil {

	/**
	 * 判断是否为空
	 * 
	 * @param obj
	 * @return
	 */
	public static boolean isNotEmpty(String obj) {
		if (obj == null) {
			return false;
		}

		if (obj.trim().length() == 0) {
			return false;
		}

		if (obj.equalsIgnoreCase("null")) {
			return false;
		}
		return true;
	}

	/**
	 * 将 email 的后几位 变为小写
	 * @param email
	 * @return
	 */
	public static String email2LowerCase(String email )
	{
		/*String[] all = email.split("@");
		if(all.length == 1 || all.length != 2 ){
			return null ;
		}*/
		char[] data = email.toCharArray();

		int dist = 'a' - 'A';
		for (int i = 0 ; i < email.length(); i++)
		{
			if (data[i] >= 'A' && data[i] <= 'Z')
			{
				data[i] += dist;
			}
		}

		return String.valueOf(data) ;

	}


	public static boolean checkPsw(String s){
		if(isNotEmpty(s)){
			return s.matches("\\w{6,18}");
		}else{
			return false ;
		}

	}
	
	public static boolean checkPswRe(String psw,String pswre){
		
		if(isNotEmpty(psw) && isNotEmpty(pswre) ){
		    return	psw.equals(pswre);
		}else{
			return false ;
		}

	}

	public static boolean checkEmail(String s){
		if(isNotEmpty(s)){
			return s.matches("\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*");
		}else{
			return false ;
		}

	}

	/*
	 * 国内三家运营商的号段分别如下：
	 *联通号段:130/131/132/155/156/185/186/145/176；
	 *电信号段:133/153/180/181/189/177；
	 *移动号段：134/135/136/137/138/139/150/151/152/157/158/159/182/183/184/187/188/147/178。
	 * 
	 * 13 0-9
	 * 14 5 7
	 * 15 0-3 5-8
	 * 17 6-8
	 * 18 0-9
	 */
	public static boolean checkPhoneNumber(String s){
		if(isNotEmpty(s)){
			LogUtils.e("login", "checkPhoneNumber  "+s);
			boolean bret=s.matches("^((13[0-9])|(14[5,7])|(15[^4,\\D])|(17[6-8])|(18[0-9]))\\d{8}$");
			LogUtils.e("login", "checkPhoneNumber res "+bret);
			return bret;
		}else{
			return false ;
		}
	}
	
	public static boolean checkVerificationCode(String s){
		if(isNotEmpty(s)){
			return s.matches("\\d{6}");
		}else{
			return false ;
		}
		
		
	}

	public static List<String> splitWithChar(String src,char c){
		List<String> strlist=new ArrayList<String>();
		String str=new String();
		for(int i=0;i<src.length();i++){
		
			if(src.charAt(i)==c){
				strlist.add(str);
				str=new String();
			}else{
				str=str+String.valueOf(src.charAt(i));
			}
		}
		strlist.add(str);
		return strlist;
	}

}
