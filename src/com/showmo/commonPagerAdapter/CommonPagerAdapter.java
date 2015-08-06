package com.showmo.commonPagerAdapter;

import java.util.List;

import com.showmo.util.LogUtils;

import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.util.Log;
import android.view.View;

public class CommonPagerAdapter extends PagerAdapter {
	private List<View> mListView;
	private int mCurPostion;
	public CommonPagerAdapter(List<View> listView){
		mListView=listView;
		mCurPostion=-1;
	}
	public int getNextIndex(){
		LogUtils.v("pager", "getNextIndex "+mCurPostion+" "+mListView.size());
		mCurPostion++;
		if(mCurPostion>=mListView.size())
		{
			mCurPostion=0;
		}
		return mCurPostion;
	}
	
	@Override
	public void destroyItem(View container, int position, Object object) {
		// TODO Auto-generated method stub
		LogUtils.v("pager", "destroyItem "+position);
		((ViewPager)container).removeView(mListView.get(position));
	}

	@Override
	public Object instantiateItem(View container, int position) {
		// TODO Auto-generated method stub
		LogUtils.v("pager", "instantiateItem "+position);
		((ViewPager)container).addView(mListView.get(position),0);
		return mListView.get(position);
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		return mListView.size();
	}

	@Override
	public boolean isViewFromObject(View arg0, Object arg1) {
		// TODO Auto-generated method stub
		return arg0 == arg1;
	}

}
