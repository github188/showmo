package com.showmo.activity.deviceManage;

import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.widget.ListView;
import android.widget.RelativeLayout;

import java.util.ArrayList;
import java.util.List;

import com.showmo.R;
import com.showmo.base.BaseActivity;
import com.showmo.util.LogUtils;

public class MainDeviceManagerActivity extends BaseActivity {
	private DrawerLayout mDrawer;
	private ListView mLeftList;
	private RelativeLayout mContentContainer;
	private FragmentManager mFm;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.activity_device_list_main);
		//getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.titlebar_feature);
		this.init();
	}
	public void init(){
		mDrawer=(DrawerLayout)findViewById(R.id.wrapper_drawerlayout);
		mLeftList=(ListView)findViewById(R.id.left_drawer);
		mLeftList.addHeaderView(getLayoutInflater().inflate(R.layout.left_nv_menu_header, mLeftList,false));
		mLeftList.setAdapter(buildLeftDrawerAdapter());
		mLeftList.setVerticalScrollBarEnabled(false);
		mContentContainer=(RelativeLayout)findViewById(R.id.content_container);
		mFm=getSupportFragmentManager();
		initContent();
	}
	private void initContent(){
		FragmentTransaction fTransaction=mFm.beginTransaction();
		fTransaction.add(R.id.content_container,new DeviceListFragment(), "content");
		fTransaction.commit();
	}
	private PwLeftMenuAdapter buildLeftDrawerAdapter(){
		List<LeftMenuData> menuDatas=new ArrayList<LeftMenuData>();
		List<LeftMenuGroup> menuGroups=new ArrayList<LeftMenuGroup>();
		menuDatas.add(new LeftMenuData("设备管理1", 0));
		menuDatas.add(new LeftMenuData("设备管理2", 0));
		menuDatas.add(null);
		menuDatas.add(new LeftMenuData("设备管理3", 0));
		menuDatas.add(new LeftMenuData("设备管理4", 0));
		menuDatas.add(new LeftMenuData("设备管理5", 0));
		menuDatas.add(null);
		menuDatas.add(new LeftMenuData("设备管理6", 0));
		menuDatas.add(new LeftMenuData("设备管理7", 0));
		menuGroups.add(new LeftMenuGroup("在线"));
		menuGroups.add(new LeftMenuGroup("不在线"));
		return new PwLeftMenuAdapter(menuDatas, menuGroups);
	}
	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// TODO Auto-generated method stub
		super.onCreateOptionsMenu(menu);
		getMenuInflater().inflate(R.menu.main_actionbar, menu);
		return true;
	}
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
		super.onOptionsItemSelected(item);
		switch (item.getItemId()) {
		case R.id.action_about:
			LogUtils.e("menu", "action_about");
			mDrawer.openDrawer(GravityCompat.START);
			break;

		default:
			break;
		}
		return true;
	}
	//	@Override
	//	public boolean onMenuItemSelected(int featureId, MenuItem item) {
	//		// TODO Auto-generated method stub
	//		super.onMenuItemSelected(featureId, item);
	//		switch (item.getItemId()) {
	//		case value:
	//			
	//			break;
	//
	//		default:
	//			break;
	//		}
	//		return true;
	//	}
}
