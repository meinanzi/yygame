package com.hn.market.core;

import com.hn.framework.HNCommonUtils;
import com.hn.pay.HNPayActivity;
import com.hn.market.base.Method;
import com.hn.market.base.Module;

import android.content.Intent;
import android.util.Log;

public class HNPayModule extends Module {
	private static final String TAG = HNPayModule.class.getSimpleName();
	
	private static HNPayModule payModule = null;

	public static HNPayModule getInstance() {
		if (payModule == null) {
			synchronized (HNPayModule.class) {
				if (payModule == null) {
					payModule = new HNPayModule();
				}
			}
		}
		return payModule;
	}

	public HNPayModule() {
		super("PayModule");
		Register("Pay", new Pay());
		Log.d(TAG, "init systmModule");
	}
	
	public class Pay implements Method {

		@Override
		public String Execute(final String args, String callBack) {
		
			return "";
		}
	}
}
