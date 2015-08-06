package com.puwell.opengles;

public class MemTexCFG {
	public char TextureName[];
	public Object privateTexutreObject;  //reserve
	
	public MemTexCFG(){
		
	}
	
	public MemTexCFG(MemTexCFG cfg){
		TextureName = new char[cfg.TextureName.length];
		System.arraycopy(cfg.TextureName, 0, TextureName, 0, cfg.TextureName.length);
		privateTexutreObject = null;
	}
	
	public MemTexCFG(char[] name){
		TextureName = new char[name.length];
		System.arraycopy(name, 0, TextureName, 0, name.length);
		privateTexutreObject = null;
	}
	
}
