package com.hn.market.core;

import com.hn.market.base.Method;


public class AyncTask {
	private Method method;
	private String args;
	private String callBack;

	public AyncTask(Method method, String args, String callBack) {
		this.method = method;
		this.args = args;
		this.callBack = callBack;
	}
	
	public void executeMethod () {
		method.Execute(args, callBack);
	}
}