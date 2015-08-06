package com.showmo.network;
import com.showmo.base.ShowmoSystem;

import android.os.AsyncTask;
import android.util.Log;
/**
 * 网络请求帮助类
 * 单例模式 getInstance()
 * 异步任务处理耗时请求
 * @author Terry
 *
 */
public class NetworkHelper {
	private static NetworkHelper m_instance;
	public  synchronized static  NetworkHelper  getInstance(){
		if(m_instance==null){
			m_instance=new NetworkHelper();
		}
		
		return m_instance ;
	}
	/**
	 * 发起网络请求时调用此方法 
	 * @param requestCallBack 抽象类，重写需要的方法
	 */
	public void newNetTask(RequestCallBack requestCallBack ){	
		 
		new NetBaseAsyncTask(requestCallBack). execute();
	}
	
	private class NetBaseAsyncTask   extends AsyncTask< Void , ResponseInfo,  ResponseInfo> {
		
		private RequestCallBack  mRequestCallBack ;
		
		public NetBaseAsyncTask(RequestCallBack  requestCallBack){
			this.mRequestCallBack = requestCallBack ;
		}
 
		@Override
		protected void onPostExecute(ResponseInfo result) {
				//根据 isSuccess 标志是否成功
				if(result == null){
					mRequestCallBack.onFinally();
					return   ;
				}
				if(result.isSuccess ){
					mRequestCallBack.onSuccess(result );
				}else{
					mRequestCallBack.onFailure( result );
				}
				mRequestCallBack.onFinally();
		}
		
		@Override
		protected void onProgressUpdate(ResponseInfo... values) {
			if(mRequestCallBack == null){
				return  ;
			}
			mRequestCallBack.onLoading(values[0]);
		}
		
		@Override
		protected ResponseInfo doInBackground(Void... params) {
			if(mRequestCallBack == null){
				return null ;
			}
			return mRequestCallBack.doInBackground();
		}
		
		@Override
		protected void onPreExecute() {
			if(mRequestCallBack == null){
				return   ;
			}
			mRequestCallBack.onPrepare();
		}
	}
}
