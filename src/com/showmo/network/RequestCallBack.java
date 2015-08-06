package com.showmo.network;

public abstract class RequestCallBack  {
	public abstract ResponseInfo doInBackground();
	public void onLoading(ResponseInfo info){};
	public void onSuccess(ResponseInfo info){};
	public void onFailure(ResponseInfo info){};
	public void onPrepare(){};
	public void onFinally(){};
}
