package com.showmo.activity.login;

import junit.framework.Assert;
import android.R;
import android.test.ActivityInstrumentationTestCase;
import android.test.ActivityInstrumentationTestCase2;
import android.test.AndroidTestCase;
import android.test.MoreAsserts;
import android.test.ViewAsserts;
import android.util.Log;

public class Test extends ActivityInstrumentationTestCase2<LoginActivity>{
	public Test(){
		super("com.showmo.activity.login", LoginActivity.class);
	}
	public void test(){
		Assert.assertNull(getActivity().findViewById(R.id.button1));
		Assert.assertNull(getActivity().findViewById(R.id.button2));
		//ViewAsserts.assertOnScreen(getActivity().findViewById(R.id.button1), getActivity().findViewById(R.id.button2));
	}
}
