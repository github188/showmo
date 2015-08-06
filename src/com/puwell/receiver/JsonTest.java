package com.puwell.receiver;

import org.json.JSONException;
import org.json.JSONObject;



public class JsonTest {
	//{"userbean":{"Uid":"100196","Showname":"\u75af\u72c2\u7684\u7334\u5b50","Avtar":null,"State":1}}
	String  stringer;//服务器返回的数据
	String Uid; 
	String Showname; 
	String Avtar; 
	String State; 
	public void jsonTest() throws JSONException{ 
		JSONObject jsonObject = new JSONObject(stringer.toString()) .getJSONObject("userbean"); 	

		Uid = jsonObject.getString("Uid"); 
		Showname = jsonObject.getString("Showname"); 
		Avtar = jsonObject.getString("Avtar"); 
		State = jsonObject.getString("State");
	} 
}
