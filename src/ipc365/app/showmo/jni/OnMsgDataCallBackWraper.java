package ipc365.app.showmo.jni;

import java.lang.reflect.Field;
import java.security.KeyStore.PrivateKeyEntry;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import com.showmo.util.LogUtils;

import android.util.Log;
import ipc365.app.showmo.jni.JniDataDef.OnMsgDataCallBackListener;

public class OnMsgDataCallBackWraper {
	private static OnMsgDataCallBackWraper m_instance=null;
	private OnMsgDataCallBackListener m_listener=null;
	
	public static OnMsgDataCallBackWraper getInstance(OnMsgDataCallBackListener listener){
		if(m_instance==null){
			m_instance=new OnMsgDataCallBackWraper();
		}
		m_instance.m_listener=listener;
		return m_instance;
	}
	private OnMsgDataCallBackWraper(){}
	public void onMsgDataCallBack(Object pBuffer,long lmsgid){
		
		if(pBuffer==null){
			LogUtils.v("reflect", "onMsgDataCallBack pBuffer==null "+lmsgid);
			return;
		}
		
		JniObjectInfoMap ObjectInfo=(JniObjectInfoMap)pBuffer;
		if(ObjectInfo.mClassName.equals("null")){//空类，即只有消息ID没有数据的消息
			m_listener.onMsgDataCallBack(null, lmsgid);
			return;
		}
		Class clazz=null;
		Object outObj=null;
		try {
		   clazz=Class.forName(ObjectInfo.mClassName);
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			//m_listener.onMsgDataCallBack(null, lmsgid);
			return;
		}
		try {
			outObj=clazz.newInstance();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return;
		}
		Set<Entry<String, String>> sets=ObjectInfo.m_ObjectInfo.entrySet();
		for (Entry<String, String> entry: sets) {
			
			String fieldName=entry.getKey();
			String fieldValue=entry.getValue();
			Field field=null;
			try {
				field=clazz.getField(fieldName);
			} catch (Exception e) {
				// TODO: handle exception
				e.printStackTrace();
				return;
			}
			//LogUtils.v("reflect", "reflect classname: "+ObjectInfo.mClassName+" type "+field.getGenericType().toString());
			String fieldType=field.getGenericType().toString();
			if(fieldType.equals("int")){
				field.setAccessible(true);
				try {
					field.setInt(outObj, Integer.parseInt(fieldValue));
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
					return;
				}
			}
			if(fieldType.equals("long")){
				field.setAccessible(true);
				try {
					field.setLong(outObj, Long.parseLong(fieldValue));
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
					return;
				}
			}
			else if(fieldType.equals("class java.lang.String")){
				//LogUtils.v("reflect", "trans string "+fieldValue);
				try {
					field.set(outObj, fieldValue);
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
					return;
				}			
			}
		}
		//LogUtils.v("wraper", outObj.toString()+" msgid "+lmsgid);
		if(m_listener!=null)
			m_listener.onMsgDataCallBack(outObj, lmsgid);
	}
}
