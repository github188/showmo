package com.showmo.widget;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

import com.showmo.util.LogUtils;

import android.R.integer;
import android.R.string;
import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.EditText;

public class PWFilterEditView extends EditText {
	
	
	public final SearchWather wather=new SearchWather(this);
	
	public PWFilterEditView(Context context) {
		super(context);
		this.init();
	}
	
	public PWFilterEditView(Context context, AttributeSet attrs) {
		super(context, attrs);
		this.init();
	}

	public PWFilterEditView(Context context, AttributeSet attrs, int style) {
		super(context, attrs, style);
		this.init();
	}
	public void init(){
		//this.addTextChangedListener(wather);
		//wather.setRegFilter("[^a-zA-Z0-9]");
	} 
	
	class SearchWather implements TextWatcher{
		   
		   
	      //监听改变的文本框  
	      private EditText editText;  
	  
	        
	      public SearchWather(EditText editText){  
	          this.editText = editText;  
	      }  

	      @Override  
	      public void onTextChanged(CharSequence ss, int start, int before, int count) {  
	          String editable = editText.getText().toString();  
	          String str = stringFilter(editable.toString());
	          LogUtils.v("edit", str);
	          if(!editable.equals(str)){
	              editText.setText(str);
	              //设置新的光标所在位置  
	              editText.setSelection(str.length());
	          }
	      }  
	  
	      @Override  
	      public void afterTextChanged(Editable s) {  
	  
	      }  
	      @Override  
	      public void beforeTextChanged(CharSequence s, int start, int count,int after) {  
	    	  	LogUtils.v("editb", ""+s);
	      }
	      public  String stringFilter(String str)throws PatternSyntaxException{     
		      // 只允许字母和数字       
		      String   regEx  = "("+m_regEx+")";// m_regEx; 
		      
		      Pattern   p   =   Pattern.compile(regEx);     
		      Matcher   m   =   p.matcher(str);   
		      LogUtils.v("REG", " reg: "+regEx+" str: "+str);
		      while(m.find()){
		    	  LogUtils.v("REG", " reg: "+m.group(1));
		    	  return m.group(1);
		      }
		      return   "";     
		  }
	      private String  m_regEx;
	  		public void setRegFilter(String reg){
	  		m_regEx=reg;
	  	}

	  }  
	 
	  
}
