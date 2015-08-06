package com.showmo.widget.swipemenulistview;

import android.content.Context;
import android.support.v4.view.MotionEventCompat;
import android.util.AttributeSet;
import android.util.Log;
import android.util.TypedValue;
import android.view.MotionEvent;
import android.view.View;
import android.view.animation.Interpolator;
import android.widget.AbsListView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.AbsListView.OnScrollListener;

/**
 * 
 * @author baoyz
 * @date 2014-8-18
 * 
 */
public class SwipeMenuListView extends ListView  
implements OnScrollListener {

	private static final int TOUCH_STATE_NONE = 0;
	private static final int TOUCH_STATE_X = 1;
	private static final int TOUCH_STATE_Y = 2;

	private int MAX_Y = 5;
	private int MAX_X = 3;
	private float mDownX;
	private float mDownY;
	private int mTouchState;
	private int mTouchPosition;
	private SwipeMenuLayout mTouchView;
	private OnSwipeListener mOnSwipeListener;

	private SwipeMenuCreator mMenuCreator;
	private OnMenuItemClickListener mOnMenuItemClickListener;
	private Interpolator mCloseInterpolator;
	private Interpolator mOpenInterpolator;

	// -- footer view
	private XListViewFooter mFooterView;
	private boolean mEnablePullLoad;
	private boolean mPullLoading;
	private boolean mIsFooterReady = false;




	public SwipeMenuListView(Context context) {
		super(context);
		init(context);
	}

	public SwipeMenuListView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		init(context);
	}

	public SwipeMenuListView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}

	private void init(Context context) {
		MAX_X = dp2px(MAX_X);
		MAX_Y = dp2px(MAX_Y);
		mTouchState = TOUCH_STATE_NONE;

		// init footer view
		mFooterView = new XListViewFooter(context);
		setOnScrollListener(this);

	}

	/**
	 * enable or disable pull up load more feature.
	 * 
	 * @param enable
	 */
	public void setPullLoadEnable(boolean enable) {//使能上拉加载
		mEnablePullLoad = enable;
		if (!mEnablePullLoad) {
			mFooterView.hide();
			mFooterView.setOnClickListener(null);
		} else {
			mPullLoading = false;
			mFooterView.show();
			mFooterView.setState(XListViewFooter.STATE_LOADING);
			// both "pull up" and "click" will invoke load more.
			//			mFooterView.setOnClickListener(new OnClickListener() {
			//				@Override
			//				public void onClick(View v) {
			//					startLoadMore();
			//				}
			//			});
		}
	}

	// the interface to trigger refresh and load more.
	private IXListViewListener mListViewListener;

	private void startLoadMore() {
		mPullLoading = true;
		mFooterView.setState(XListViewFooter.STATE_LOADING);
		if (mListViewListener != null) {
			mListViewListener.onLoadMore();
		}
	}

	/**
	 * stop load more, reset footer view.
	 */
	public void stopLoadMore() {
		if (mPullLoading == true) {
			mPullLoading = false;
		}
	}


	@Override
	public void setAdapter(ListAdapter adapter) {

		if (mIsFooterReady == false) {
			mIsFooterReady = true;
			addFooterView(mFooterView);
		}


		super.setAdapter(new SwipeMenuAdapter(getContext(), adapter) {
			@Override
			public void createMenu(SwipeMenu menu) {
				if (mMenuCreator != null) {
					mMenuCreator.create(menu);
				}
			}

			@Override
			public void onItemClick(SwipeMenuView view, SwipeMenu menu,
					int index) {
				boolean flag = false;
				if (mOnMenuItemClickListener != null) {
					flag = mOnMenuItemClickListener.onMenuItemClick(
							view.getPosition(), menu, index);
				}
				if (mTouchView != null && !flag) {
					mTouchView.smoothCloseMenu();
				}
			}
		});
	}

	public void setCloseInterpolator(Interpolator interpolator) {
		mCloseInterpolator = interpolator;
	}

	public void setOpenInterpolator(Interpolator interpolator) {
		mOpenInterpolator = interpolator;
	}

	public Interpolator getOpenInterpolator() {
		return mOpenInterpolator;
	}

	public Interpolator getCloseInterpolator() {
		return mCloseInterpolator;
	}

	@Override
	public boolean onInterceptTouchEvent(MotionEvent ev) {
		return super.onInterceptTouchEvent(ev);
	}

	@Override
	public boolean onTouchEvent(MotionEvent ev) {
		if (ev.getAction() != MotionEvent.ACTION_DOWN && mTouchView == null)
			return super.onTouchEvent(ev);
		int action = MotionEventCompat.getActionMasked(ev);
		action = ev.getAction();
		switch (action) {
		case MotionEvent.ACTION_DOWN:

			canLoadMore = true ;
			
			int oldPos = mTouchPosition;
			mDownX = ev.getX();
			mDownY = ev.getY();
			mTouchState = TOUCH_STATE_NONE;

			mTouchPosition = pointToPosition((int) ev.getX(), (int) ev.getY());

			if (mTouchPosition == oldPos && mTouchView != null
					&& mTouchView.isOpen()) {
				mTouchState = TOUCH_STATE_X;
				mTouchView.onSwipe(ev);
				return true;
			}

			View view = getChildAt(mTouchPosition - getFirstVisiblePosition());

			if (mTouchView != null && mTouchView.isOpen()) {
				mTouchView.smoothCloseMenu();
				mTouchView = null;
				// return super.onTouchEvent(ev);
				// try to cancel the touch event
				MotionEvent cancelEvent = MotionEvent.obtain(ev);  
				cancelEvent.setAction(MotionEvent.ACTION_CANCEL);    
				onTouchEvent(cancelEvent);
				return true;
			}
			if (view instanceof SwipeMenuLayout) {
				mTouchView = (SwipeMenuLayout) view;
			}
			if (mTouchView != null) {
				mTouchView.onSwipe(ev);
			}
			break;
		case MotionEvent.ACTION_MOVE:
			float dy = Math.abs((ev.getY() - mDownY));
			float dx = Math.abs((ev.getX() - mDownX));
			if (mTouchState == TOUCH_STATE_X) {
				if (mTouchView != null) {
					mTouchView.onSwipe(ev);
				}
				getSelector().setState(new int[] { 0 });
				ev.setAction(MotionEvent.ACTION_CANCEL);
				super.onTouchEvent(ev);
				return true;
			} else if (mTouchState == TOUCH_STATE_NONE) {
				if (Math.abs(dy) > MAX_Y) {
					mTouchState = TOUCH_STATE_Y;
				} else if (dx > MAX_X) {
					mTouchState = TOUCH_STATE_X;
					if (mOnSwipeListener != null) {
						mOnSwipeListener.onSwipeStart(mTouchPosition);
					}
				}
			}
			break;
		case MotionEvent.ACTION_UP:
			if (mTouchState == TOUCH_STATE_X) {
				if (mTouchView != null) {
					mTouchView.onSwipe(ev);
					if (!mTouchView.isOpen()) {
						mTouchPosition = -1;
						mTouchView = null;
					}
				}
				if (mOnSwipeListener != null) {
					mOnSwipeListener.onSwipeEnd(mTouchPosition);
				}
				ev.setAction(MotionEvent.ACTION_CANCEL);
				super.onTouchEvent(ev);
				return true;
			}
			break;
		}
		return super.onTouchEvent(ev);
	}

	public void smoothOpenMenu(int position) {
		if (position >= getFirstVisiblePosition()
				&& position <= getLastVisiblePosition()) {
			View view = getChildAt(position - getFirstVisiblePosition());
			if (view instanceof SwipeMenuLayout) {
				mTouchPosition = position;
				if (mTouchView != null && mTouchView.isOpen()) {
					mTouchView.smoothCloseMenu();
				}
				mTouchView = (SwipeMenuLayout) view;
				mTouchView.smoothOpenMenu();
			}
		}
	}

	private int dp2px(int dp) {
		return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, dp,
				getContext().getResources().getDisplayMetrics());
	}

	public void setMenuCreator(SwipeMenuCreator menuCreator) {
		this.mMenuCreator = menuCreator;
	}

	public void setOnMenuItemClickListener(
			OnMenuItemClickListener onMenuItemClickListener) {
		this.mOnMenuItemClickListener = onMenuItemClickListener;
	}

	public void setOnSwipeListener(OnSwipeListener onSwipeListener) {
		this.mOnSwipeListener = onSwipeListener;
	}

	public static interface OnMenuItemClickListener {
		boolean onMenuItemClick(int position, SwipeMenu menu, int index);
	}

	public static interface OnSwipeListener {
		void onSwipeStart(int position);

		void onSwipeEnd(int position);
	}


	// total list items, used to detect is at the bottom of listview.
	private int mTotalItemCount;

	@Override
	public void onScrollStateChanged(AbsListView view, int scrollState) {

		if(scrollState != OnScrollListener.SCROLL_STATE_FLING ){
			if (mEnablePullLoad && !mPullLoading && isSlie2Bottom) {
				if(canLoadMore  ){
					startLoadMore();
					canLoadMore = false ;
				}
			}

		} 


	}


	private boolean canLoadMore ;

	private boolean isSlie2Bottom ; 


	@Override
	public void onScroll(AbsListView view, int firstVisibleItem,
			int visibleItemCount, int totalItemCount) {
		// send to user's listener
		mTotalItemCount = totalItemCount;
		
		if( totalItemCount == visibleItemCount  ){
			mFooterView.setVisibility(View.INVISIBLE);
		}else{
			mFooterView.setVisibility(View.VISIBLE);
		}
		
		if (getLastVisiblePosition() == totalItemCount - 1) {
			// invoke load more.
			isSlie2Bottom = true ;
			
			
		}else{
			isSlie2Bottom =false ;
		}

	}

	Runnable runStartLoadMore = new Runnable() {

		@Override
		public void run() {
			startLoadMore();
		}
	};




	public void setXListViewListener(IXListViewListener l) {
		mListViewListener = l;
	}


	/**
	 * implements this interface to get refresh/load more event.
	 */
	public interface IXListViewListener {
		public void onRefresh();

		public void onLoadMore();
	}

}
