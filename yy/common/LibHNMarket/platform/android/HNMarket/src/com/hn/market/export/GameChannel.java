package com.hn.market.export;


import org.json.JSONException;
import org.json.JSONObject;

import com.hn.market.base.ResponseArgs;
import com.hn.pay.HNPayActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class GameChannel {
	private static final String TAG = GameChannel.class.getName();

	private native void payCB(String ID, boolean result, String args);

	private native void responseChannel(String callback, String args);
	
	private Activity activity = null;
	private Handler uiMessageHandler = null;

	public static GameChannel gameChannel = null;

	
	public static GameChannel getInstance() {
		if (gameChannel == null) {
			gameChannel = new GameChannel();
		}
		return gameChannel;
	}

	private GameChannel() {

	}

	public boolean start(Activity activity, Handler handler) {
		this.uiMessageHandler = handler;
		this.activity = activity;
		ChannelExport.getInstance().start(this.activity, this.uiMessageHandler);
		return true;
	}

	public void onDestroy() {
		ChannelExport.getInstance().onDestroy();
	}

	public void onResume(Activity page) {
		ChannelExport.getInstance().onResume(page);
	}

	public void onPause(Activity page) {
		ChannelExport.getInstance().onPause(page);
	}
	
	public static String requestChannel(String moudleName, String function,
			String args, String callback) {
		return ChannelExport.getInstance().requestChannel(moudleName, function,
				args, callback);
	}

	public static void pay(String ID) {
		Message msg = new Message();
		msg.what = Constant.COMMOAND_PAY;
		msg.obj = ID;
		gameChannel.uiMessageHandler.sendMessage(msg);
	}
	
	public void notifyPay(Object object)
	{
		String data = (String)object;		
		Intent intent = new Intent(activity, HNPayActivity.class);
		intent.putExtra("COMMOAND_PAY_RESULT", Constant.COMMOAND_PAY_RESULT);
		intent.putExtra("PAY_DATA", data);
		activity.startActivityForResult(intent, Constant.COMMOAND_PAY_RESULT);
	}
	
	public void notifyPayCB(Object object)
	{
		Intent intent = (Intent)object;
		int status = intent.getIntExtra("status", 0);
		String identifier = intent.getStringExtra("identifier");
		JSONObject json = new JSONObject();
		try {
			json.put("status", status);
			payCB(identifier, status == 1, json.toString());
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	public void notifyResponseChannel(Object object)
	{
		ResponseArgs pragma = (ResponseArgs)object;
		responseChannel(pragma.getCallBack(), pragma.getArgs());
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		switch (requestCode) {
		case Constant.COMMOAND_PAY_RESULT:
		{
			Message msg = new Message();
			msg.what = Constant.COMMOAND_PAY_RESULT;
			msg.obj = data;
			gameChannel.uiMessageHandler.sendMessage(msg);			
		} break;
		default:
			break;
		}
	}
}
