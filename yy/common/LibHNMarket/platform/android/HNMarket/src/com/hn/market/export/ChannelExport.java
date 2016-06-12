package com.hn.market.export;

//import java.lang.reflect.InvocationTargetException;
//import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import com.hn.market.base.Module;
import com.hn.market.base.ResponseArgs;
import com.hn.market.core.HNAyncModule;
import com.hn.market.core.HNMarketModule;
import com.hn.market.core.HNPayModule;
import com.hn.market.core.HNSysModule;

import android.app.Activity;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class ChannelExport {
	private static final String TAG = ChannelExport.class.getSimpleName();
	private static ChannelExport mChannelExport = null;
	private Handler mHandler;
	private Map<String, Module> mModule = null;

	public static ChannelExport getInstance() {
		if (mChannelExport == null) {
			mChannelExport = new ChannelExport();
		}
		return mChannelExport;
	}

	private ChannelExport() {

	}

	public boolean start(Activity activity, Handler handler) {
		if (mModule == null) {
			mModule = new HashMap<String, Module>();
		}
		mModule.clear();

		mHandler = handler;

		HNSysModule sysModule = HNSysModule.getInstance();
		sysModule.start(activity);
		mModule.put(sysModule.GetModuleName(), sysModule);

		HNMarketModule marketAdapter = new HNMarketModule();
		marketAdapter.start(activity);
		mModule.put(marketAdapter.GetModuleName(), marketAdapter);

		HNAyncModule ayncAdapter = new HNAyncModule();
		ayncAdapter.start(activity);
		mModule.put(ayncAdapter.GetModuleName(), ayncAdapter);
		
		HNPayModule payAdapter = new HNPayModule();
		payAdapter.start(activity);
		mModule.put(payAdapter.GetModuleName(), payAdapter);

		Log.i(TAG, "Init");
		return true;
	}

	public boolean registerModule(String key, Module module) {
		if (key == null || key.equals("")) {
			throw new NullPointerException("key is null.");
		}

		if (module == null || module.equals("")) {
			throw new NullPointerException("module is null.");
		}

		mModule.put(key, module);
		return true;
	}

	public String requestChannel(String moudleName, String function,
			String args, String callback) {
		if(!moudleName.equals("")){
			Log.d(TAG, moudleName);
		}
		if(!function.equals("")){
			Log.d(TAG, function);
		}
		if(!args.equals("")){
			Log.d(TAG, args);
		}
		if(!callback.equals("")){
			Log.d(TAG, callback);
		}
		
		if (moudleName == null || moudleName.equals("")) {
			throw new NullPointerException("key is null.");
		}

		if (function == null || function.equals("")) {
			throw new NullPointerException("key is null.");
		}

		Module module = mModule.get(moudleName.toLowerCase());
		if (module != null) {
			String ret = module.Execute(function, args, callback);
			return (ret == null) ? "" : ret;
		}
		return "";
	}

	public void executeAyncMethod(String callBack, String args) {
		Message msg = mHandler.obtainMessage();
		msg.what = Constant.COMMAND_RESPONSECHANNEL;
		ResponseArgs pragma = new ResponseArgs(callBack, args);
		msg.obj = pragma;
		mHandler.sendMessage(msg);
	}

	public void onDestroy() {
		Iterator<String> iter = mModule.keySet().iterator();
		while (iter.hasNext()) {
			Module module = mModule.get(iter.next());
			module.onDestroy();
		}
	}

	public void onResume(Activity page) {
		Iterator<String> iter = mModule.keySet().iterator();
		while (iter.hasNext()) {
			Module module = mModule.get(iter.next());
			module.onResume(page);
		}
	}

	public void onPause(Activity page) {
		Iterator<String> iter = mModule.keySet().iterator();
		while (iter.hasNext()) {
			Module module = mModule.get(iter.next());
			module.onPause(page);
		}
	}
}
