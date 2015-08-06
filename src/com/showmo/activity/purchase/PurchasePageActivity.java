package com.showmo.activity.purchase;

import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ProgressBar;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.util.LogUtils;

public class PurchasePageActivity extends BaseActivity {
	private WebView mPurchaseView;
	private ProgressBar mLoadProgressBar;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_purchase_main);
		this.initView();
	}

	private void initView(){
		setBarTitleWithBackFunc(R.string.purchase_camera);
		mPurchaseView=(WebView)findViewById(R.id.purchase_main_link);
		mLoadProgressBar=(ProgressBar)findViewById(R.id.purchase_load_progress);
		
		mPurchaseView.loadUrl("http://www.showmo365.com");
		WebSettings settings = mPurchaseView.getSettings();
		settings.setJavaScriptEnabled(true);
		mPurchaseView.setWebViewClient(new WebViewClient(){
			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				// TODO Auto-generated method stub
				//返回值是true的时候控制去WebView打开，为false调用系统浏览器或第三方浏览器
				view.loadUrl(url);
				return true;
			}
		});
		mPurchaseView.setWebChromeClient(new WebChromeClient() {
	            @Override
	            public void onProgressChanged(WebView view, int newProgress) {
	                // TODO Auto-generated method stub
	            	LogUtils.e("progress", "onProgressChanged newProgress "+newProgress);
	                if (newProgress == 100) {
	                    // 网页加载完成
	                	mLoadProgressBar.setVisibility(View.GONE);
	                } else {
	                    // 加载中
	                	mLoadProgressBar.setVisibility(View.VISIBLE);
	                	mLoadProgressBar.setProgress(newProgress);
	                }

	            }
	        });
	}
	@Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // TODO Auto-generated method stub
        if(keyCode==KeyEvent.KEYCODE_BACK)
        {
            if(mPurchaseView.canGoBack())
            {
            	mPurchaseView.goBack();//返回上一页面
                return true;
            }
            else
            {
            	finish();
                //System.exit(0);//退出程序
            }
        }
        return super.onKeyDown(keyCode, event);
    }
	
}
