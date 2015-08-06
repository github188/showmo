package com.showmo.commonAdapter;

import java.util.List;

import android.R.integer;
import android.content.Context;
import android.text.GetChars;
import android.text.TextDirectionHeuristic;
import android.util.Log;
import android.util.SparseArray;
import android.view.InflateException;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class AdapterViewHolder {
	private LayoutInflater m_inflater;
	private View m_convertView;
	private SparseArray<View> m_viewList;
	
	
	public AdapterViewHolder(Context context,ViewGroup parent,int resourceId){
		m_inflater=LayoutInflater.from(context);
		this.m_viewList=new SparseArray<View>();
		//LogUtils.v("adapter","inflate bf");
		try {
			m_convertView=m_inflater.inflate(resourceId, parent,false);
		} catch (InflateException e) {
			// TODO: handle exception
			//LogUtils.v("adapter","inflate exc");
			e.printStackTrace();
		}
		
		m_convertView.setTag(this);	
	}
	public static AdapterViewHolder get(Context context,View convertView,ViewGroup parent,int resourceId){
		//LogUtils.v("adapter", "AdapterViewHolder get"+convertView+resourceId);
		if(convertView==null){
			return new AdapterViewHolder(context, parent, resourceId);
		}else{
			return (AdapterViewHolder)convertView.getTag();
		}
	}
	public <T extends View> T  getView(int viewId){
		View view=m_viewList.get(viewId);
		if(view == null){
			view =m_convertView.findViewById(viewId);
			m_viewList.put(viewId, view);
		}
		
		return (T)view;
	}

	public View getConvertView(){
		return m_convertView;
	}
	
}
