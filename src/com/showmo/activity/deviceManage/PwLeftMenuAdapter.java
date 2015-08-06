package com.showmo.activity.deviceManage;

import java.util.List;

import com.showmo.R;
import com.showmo.commonAdapter.CommonGroupAdapter;

import android.widget.TextView;


public class PwLeftMenuAdapter extends CommonGroupAdapter<LeftMenuData, LeftMenuGroup> {
	public PwLeftMenuAdapter(List<LeftMenuData> menuDatas,List<LeftMenuGroup> menuGroups){
		super(menuDatas, menuGroups, R.layout.left_menu_content_item, R.layout.left_menu_group);
	}
	public void onLayoutContent(CommonGroupAdapter.IViewHolder viewHolder, LeftMenuData data) {
		TextView tView = (TextView)viewHolder.findViewById(R.id.menu_title);
		tView.setText(data.title);
	};
	public void onLayoutGroup(CommonGroupAdapter.IViewHolder viewHolder, LeftMenuGroup data) {
		TextView tView = (TextView)viewHolder.findViewById(R.id.group_name);
		tView.setText(data.groupString);
	};
}
