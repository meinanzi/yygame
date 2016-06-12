package com.YX.project;

import com.dinpay.plugin.activity.DinpayChannelActivity;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MerchantPayResultActivity extends Activity{
	
	public static final String TAG = MerchantPayResultActivity.class.getSimpleName();
	public static boolean isFromPay = false;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		//this.setContentView(R.layout.test_activity);
		
		Log.i(TAG, "MerchantPayResultActivity onCreate");
		
		if(isFromPay){
			handleJump();		
		}
		else{
			Bundle xmlData = getIntent().getExtras();
			if(xmlData != null){
				String xml = xmlData.getString("xml");
				Log.i(TAG, xml);
				isFromPay = true;
				handlePayRequest(xml);
			}
		}
		
		
//		if(isFromPay){
//			Bundle xmlData = getIntent().getExtras();
//			if(xmlData != null){
//				String xml = xmlData.getString("xml");
//				Log.i(TAG, xml);
//				handlePayResponse(xml);
//				isFromPay = false;
//			}			
//		}
//		else{
//			Bundle xmlData = getIntent().getExtras();
//			if(xmlData != null){
//				String xml = xmlData.getString("xml");
//				Log.i(TAG, xml);
//				handlePayRequest(xml);
//				isFromPay = true;
//			}
//		}
		
		
//		if(payFlag){
//			Log.i("test", "pay back");
//			Intent intent = new Intent(MerchantPayResultActivity.this, AppActivity.class);
//			startActivity(intent);
//			//finish();
//		}
//		else{
//			Bundle xmlData = getIntent().getExtras();
//			if(xmlData != null){
//				String xml = xmlData.getString("xml");
//				Log.i("test", xml);
//				Intent intent = new Intent(this, DinpayChannelActivity.class);
//				intent.putExtra("xml", xml);
//				intent.putExtra("ActivityName", "com.JulyNet.MixProject.MerchantPayResultActivity");
//				startActivity(intent);
//				finish();
//			}
//			payFlag = true;
//			Log.i("test", "pay go.");
//		}
		/*
		Button test = (Button)this.findViewById(R.id.button1);
		test.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Log.i(TAG, "MerchantPayResultActivity onClick");
				Intent intent = new Intent(MerchantPayResultActivity.this, AppActivity.class);
				startActivity(intent);
			}
			
		});*/
		
//		Bundle xmlData = getIntent().getExtras();
//		if(xmlData!=null){
//			String response = xmlData.getString("xml");
//			try{
//				String status = "<trade_status>";
//				int start = response.indexOf(status);
//				int end = response.indexOf("</trade_status>");
//				String str = response.substring(start+status.length(), end);
//				if("SUCCESS".equals(str)){
//					//merchant_payResult.setText("支付结果：支付成功");
//				}else if("UNPAY".equals(str)){
//					//merchant_payResult.setText("支付结果：未支付");
//				}else{
//					//merchant_payResult.setText("支付结果：支付失败");
//				}
//				Intent intent = new Intent(this, AppActivity.class);
//				startActivity(intent);
//			}catch (Exception e) {
//				e.printStackTrace();
//			}
//		}

		//setContentView(R.layout.merchant_pay_result);
		
		//TextView merchant_payResult = (TextView)this.findViewById(
		//		R.id.merchant_payResult);
			
//		Bundle data = getIntent().getExtras();
//		if(data != null){
//			
//			if(count == 0){
//				String param  = data.getString("request");
//				if(param == null){
//					Log.e("test", "request param null.");
//				}
//				
//				if(param != null && param.equals("")){
//					Log.e("test", "request param 0");
//				}
//				
//				if(param != null && !param.equals("")){
//					Log.e("test", param);
//					AppActivity.isFromPay = true;
//					handlePayRequest(param);					
//					return;
//				}
//			}
//			else{
//				handleJump();
//			}
			//return;
			
//			String param;
//			
//			if(param != null && !param.equals("")){
//				Log.e("test", param);
//				handlePayRequest(param);
//				isPay = false;
//				return;
//			}
			
//			// 支付响应
//			param = data.getString("xml");
//			
//			if(param == null){
//				Log.e("test", "xml param null.");
//			}
//			
//			if(param != null && param.equals("")){
//				Log.e("test", "xml param 0");
//			}
//			
//			if(param != null && !param.equals("")){
//				Log.e("test", param);
//				//handlePayResponse(param);
//				//return;
//			}
//			
//			if(!isPay){
//				handleJump();
//			}
						
//			// 请求支付
//			param  = data.getString("request");
//			if(param == null){
//				Log.e("test", "request param null.");
//			}
//			
//			if(param != null && param.equals("")){
//				Log.e("test", "request param 0");
//			}
//			
//			if(param != null && !param.equals("")){
//				Log.e("test", param);
//				handlePayRequest(param);
//				isPay = false;
//				return;
//			}
			
			
			
			// 其他跳转
//			handleJump();
//		}	
	}
	
	// 支付请求跳转
	private void handlePayRequest(String param){
		Log.i(TAG, "**************************handlePayRequest");
		Log.i(TAG, param);
		
		Intent intent = new Intent(this, DinpayChannelActivity.class);
		intent.putExtra("xml", param);
		intent.putExtra("ActivityName", "com.JulyNet.MixProject.MerchantPayResultActivity");
		startActivity(intent);
		finish();
	}
		
	// 支付响应跳转
	private void handlePayResponse(String param){
		try{
			boolean payResult = false;
			
			Log.i(TAG, "************************handlePayResponse");
			Log.i(TAG, param);
			
			String status = "<trade_status>";
			int start  = param.indexOf(status);
			int end    = param.indexOf("</trade_status>");
			String str = param.substring(start+status.length(), end);
			if("SUCCESS".equals(str)){
				payResult = true;
			}else if("UNPAY".equals(str)){
				
			}else{
				
			}
			
			Intent intent = new Intent(this, AppActivity.class);
			intent.putExtra("payResult", payResult);
			startActivity(intent);
			finish();
		}catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	// 异常响应
	private void handleJump(){
		Intent intent = new Intent(this, AppActivity.class);
		intent.putExtra("isFromPay", true);
		startActivity(intent);
		finish();
		
		Log.i(TAG, "**************************handleJump");
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		Log.i(TAG, "MerchantPayResultActivity onDestroy");
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		Log.i(TAG, "MerchantPayResultActivity onPause");
		super.onPause();
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		Log.i(TAG, "MerchantPayResultActivity onRestart");
		super.onRestart();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub			
		super.onResume();
		
		Log.i(TAG, "MerchantPayResultActivity onResume");	
		
	}
	
	
}
