package com.showmo.base;

import java.io.File;
import java.io.IOException;

import android.util.Log;


public class BaseFileOperate {
	private static BaseFileOperate m_Instance = null;
	private static String m_pathApp = "";
	
	private BaseFileOperate() {
		m_pathApp = ShowmoApplication.getInstance().getFilesDir().getAbsolutePath();
		File dstLogDir = new File(m_pathApp+"/Log");
		if(!dstLogDir.exists()) {
			dstLogDir.mkdirs();
		}
		File dstDataDir = new File(m_pathApp+"/Data");
		if(!dstDataDir.exists()) {
			dstDataDir.mkdirs();
		}
	}
	
	public static BaseFileOperate getInstance() {
		if(m_Instance == null)
	    	m_Instance = new BaseFileOperate();
	    
	    return m_Instance;
	}
	
	public String getLogDir() {
		return m_pathApp+"/Log";
	}
	
	public String getDataDir() {
		return m_pathApp+"/Data";
	}
	
	public boolean CreateFile(String filename) {
		File dstFile = new File(filename);
		if(!dstFile.exists()) {
			try {
				dstFile.createNewFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		return true;
	}
}
