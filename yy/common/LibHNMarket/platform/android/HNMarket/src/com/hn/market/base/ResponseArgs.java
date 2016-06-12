package com.hn.market.base;


public class ResponseArgs
{
	private String callback;
	private String args;

	public ResponseArgs(String callback, String args) {
		this.callback = callback;
		this.args = args;
	}

	public String getCallBack() {
		return this.callback;
	}

	public String getArgs() {
		return this.args;
	}
}
