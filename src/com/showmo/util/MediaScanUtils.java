package com.showmo.util;

import java.io.File;

import com.showmo.MainActivity;

import android.content.ContextWrapper;
import android.content.Intent;
import android.net.Uri;

public class MediaScanUtils {
	public static void MediaScan(ContextWrapper context,String path){
		Intent mScanIntent=new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
		Uri uri = Uri.fromFile(new File(path));
		mScanIntent.setData(uri);
		context.sendBroadcast(mScanIntent);
	}
}
