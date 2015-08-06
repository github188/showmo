package ipc365.app.showmo.jni;

import java.util.HashMap;

public class JniObjectInfoMap {
	String mClassName;
	HashMap<String, String> m_ObjectInfo;
	public JniObjectInfoMap(){
		m_ObjectInfo=new HashMap<String, String>();
	}
	public void setNewClassName(String className){
		mClassName=className;
		m_ObjectInfo.clear();
	}
	public void addField(String fieldName,String FieldValue){
		m_ObjectInfo.put(fieldName, FieldValue);
	}
	
}
