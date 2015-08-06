package com.showmo.commonAdapter;

import java.util.Calendar;
import java.util.List;

import com.showmo.util.LogUtils;

import android.R.integer;
import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ArrayAdapter;
import android.widget.ListView;
public abstract class CommonAdapter<T> extends BaseAdapter {
	protected  List<T> m_dataList;
	protected  Context  m_context;
	protected  final int  m_resourceId;
	protected int m_curItemPos=0;
	protected Drawable m_curItemBgDrawable=null;
	protected int m_curItemBgColor=-1;
	public CommonAdapter(Context context,List<T> dataList,int resourceId)throws NullPointerException {
		// TODO Auto-generated constructor stub
		if(dataList==null){
			throw new NullPointerException("CommonAdapter: give adapter a null list");
		}
		this.m_dataList=dataList;
		this.m_context=context;
		this.m_resourceId=resourceId;
	}
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return m_dataList.size();
	} 
	public List<T> getDataList(){
		return m_dataList;
	}
	public T getCurData(){
		if(m_curItemPos<0 || m_curItemPos>=m_dataList.size()){
			return null;
		}
		return m_dataList.get(m_curItemPos);
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return m_dataList.get(position);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub

		AdapterViewHolder viewHolder=AdapterViewHolder.get(m_context, convertView, parent, m_resourceId);

		this.displayViewLayout(viewHolder, m_dataList.get(position));
		View conView=viewHolder.getConvertView();  

		if(m_curItemPos==position){
			if(m_curItemBgDrawable!=null){
				conView.setBackgroundDrawable(m_curItemBgDrawable);
			}
			else{
				if(m_curItemBgColor!=-1){
					conView.setBackgroundColor(m_curItemBgColor);
				}else{
					m_curItemBgColor=Color.argb(155, 200, 200, 200);
					conView.setBackgroundColor(m_curItemBgColor);
				}
			}
		}else{
			conView.setBackgroundColor(Color.TRANSPARENT);
		}
		return viewHolder.getConvertView();  
	}
	public void setCurItem(int pos){
		//LogUtils.e("curItem", "setCurItem "+pos+" object hashcode "+hashCode());
		m_curItemPos=pos;
		notifyDataSetChanged();
	}
	public void setCurItemBackgroundColor(int color){
		m_curItemBgColor=color;
	}
	public void setCurItemBackgroundDrawable(Drawable drawable){
		m_curItemBgDrawable=drawable;
	}
	public abstract void displayViewLayout(AdapterViewHolder viewHolder,T data);
}
