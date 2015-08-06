package com.showmo.commonAdapter;

import java.security.acl.Group;
import java.util.HashMap;
import java.util.List;

import com.showmo.util.LogUtils;

import android.content.Context;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;

public abstract class CommonGroupAdapter<T,K> extends BaseAdapter {
	public static interface IViewHolder{
		public View findViewById(int ID);
	}
	private static class ViewHolder implements IViewHolder{
		private View mView;
		private SparseArray<View> mHolderChildViewList;
		private static SparseArray<ViewHolder> mViewHolderList=new SparseArray<CommonGroupAdapter.ViewHolder>();
		public ViewHolder(int resId,ViewGroup parent){
			LayoutInflater inflater=LayoutInflater.from(parent.getContext());
			mView=inflater.inflate(resId, parent,false);
			mHolderChildViewList=new SparseArray<View>();
			LogUtils.e("adapter", "new holder "+mView.hashCode());
			mViewHolderList.append(mView.hashCode(), this);
		}
		public static ViewHolder getViewHolder(int resId,View convertView, ViewGroup parent){
			if(convertView == null){
				return new ViewHolder(resId,parent);
			}
			return mViewHolderList.get(convertView.hashCode());
		}
		@Override
		public View findViewById(int ID) {
			// TODO Auto-generated method stub
			View target=mHolderChildViewList.get(ID);
			if (target==null) {
				target = mView.findViewById(ID);
				mHolderChildViewList.append(ID, target);
			}
			return target;
		}
	}
	private List<T> mDataList;
	private HashMap<Integer, K> mGroupList;
	private int mContentResLayout;
	private int mGroupResLayout;
	/*
	 * 数据里面填充null表示这一项是Group项
	 */
			
	public CommonGroupAdapter(List<T> dataList,List<K> groupList,int contentResLayout,int groupResLayout){
		super();
		mDataList=dataList;
		mGroupList=new HashMap<Integer,K>();
		int index=0;
		for (int i = 0; i < mDataList.size(); i++) {
			if(mDataList.get(i) == null){
				mGroupList.put(Integer.valueOf(i), groupList.get(index));
				index++;
			}
		}
		mContentResLayout=contentResLayout;
		mGroupResLayout=groupResLayout;
	}
	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return mDataList.size();
	}

	@Override
	public T getItem(int position) {
		// TODO Auto-generated method stub
		return mDataList.get(position);
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		boolean bGroupItem=false;
		if(mDataList.get(position) == null){
			bGroupItem=true;
		}
		ViewHolder mViewHolder=null;
		if (bGroupItem) {
			mViewHolder=ViewHolder.getViewHolder(mGroupResLayout, convertView, parent);
			onLayoutGroup(mViewHolder, mGroupList.get(Integer.valueOf(position)));
		}else {
			mViewHolder=ViewHolder.getViewHolder(mContentResLayout, convertView, parent);
			onLayoutContent(mViewHolder, mDataList.get(position));
		}
		
		return mViewHolder.mView;
	}
	public abstract void onLayoutContent(IViewHolder viewHolder,T data);
	public abstract void onLayoutGroup(IViewHolder viewHolder,K data);
}
