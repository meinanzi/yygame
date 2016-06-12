package com.hn.market.base;

import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

public class Module {
	private static final String TAG = Module.class.getSimpleName();
	protected Map<String, Method> mMethodMap = null;
	protected Activity activity = null;
	
	private String moduleName = null;

	public Module(String name) {
		moduleName = name.toLowerCase();
		if (mMethodMap == null) {
			mMethodMap = new HashMap<String, Method>();
		}
		mMethodMap.clear();
	}
	
	public void start(Activity activity) {
		this.activity = activity;
	}
	
	// 注册方法
	public void Register(String key, Method method) {
		if (key == null || key.equals("")) {
			Log.e(TAG, "key is null");
			throw new NullPointerException("key is null");
		}

		if (method == null) {
			Log.e(TAG, "method is null");
			throw new NullPointerException("method is null");
		}
		String lowString = key.toLowerCase();
		mMethodMap.put(lowString, method);
	}

	// 查找方法
	public Method FindMethod(String function) {
		if (function == null || function.equals("")) {
			Log.e(TAG, "function is null");
			throw new NullPointerException("function is null");
		}
		String lowfunction = function.toLowerCase();
		Method method = null;
		method = mMethodMap.get(lowfunction);
		return method;
	}

	// 执行方法处理
	public String Execute(String function, String args, String callBack) {
		Method method = FindMethod(function.toLowerCase());
		if (method == null) {
			Log.e(TAG, "method is null");
			throw new NullPointerException("method is null");
		}
		return method.Execute(args, callBack);
	}

	// 获取系统模块
	public String GetModuleName() {
		return moduleName;
	}

	public void onResume(Activity page) {

	}

	public void onPause(Activity page) {

	}

	public void onDestroy() {

	}

}
