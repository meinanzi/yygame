/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.YX.project;

import java.util.Date;
import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import com.umeng.social.CCUMSocialController;

import cn.paypalm.pppayment.PPInterface;
import cn.paypalm.pppayment.global.ResponseDataToMerchant;

import com.hn.market.core.HNPhotoModule;
import com.hn.market.export.ChannelExport;
import com.hn.market.export.Constant;
import com.hn.market.export.GameChannel;
import com.hn.pay.base.PayResultCallBack;
import com.hn.pay.base.ProductInfo;
import com.hn.pay.sdk.AlipayManager;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.WindowManager;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity {

	// 支付状态
	private int status = 0;
	
	// 商品信息
	private ProductInfo product = new ProductInfo();
	
	private Handler uiMessageHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			switch(msg.what){
			case Constant.COMMOAND_PAY:
				payRequest(msg);
				break;
			case Constant.COMMOAND_PAY_RESULT:
				payResponse(msg);
				break;
			case Constant.COMMAND_RESPONSECHANNEL:
				channelResonpse(msg);
				break;
			}
		}
	};
		
	// 支付请求
	public void payRequest(Message msg){
		String data = (String)msg.obj;
		product.deserialize(data);
		if(Constant.SDK_TYPE_WEIXIN == product.payType){
			Toast.makeText(AppActivity.this, "程序猿玩命开发中",
					Toast.LENGTH_SHORT).show();
			AppActivity.this.status = 0;
			updatePayResult();
		}
		else if(Constant.SDK_TYPE_ZHIFUBAO == product.payType){
			AlipayManager alipay = new AlipayManager(AppActivity.this);
			alipay.pay(String.format("%d筹码", product.number), 
					String.format("%.2f元购%d 筹码", product.price, product.number), product.orderID, 
					String.format("%.2f", product.price),
					new PayResultCallBack() {

						@Override
						public void onPayResult(PAY_SDK sdk, int status) {

							AppActivity.this.status = status;

							switch (sdk) {
							case ALIPAY:

								break;
							case WECHAT:

								break;
							case UNIONPAY:

								break;
							default:
								break;
							}

							if (1 == status) {
								Toast.makeText(AppActivity.this, "支付成功",
										Toast.LENGTH_SHORT).show();
							} else {
								Toast.makeText(AppActivity.this, "支付失败",
										Toast.LENGTH_SHORT).show();
							}
							updatePayResult();
						}

					});
		}
		else if(Constant.SDK_TYPE_PPQIANBAO == product.payType){
			String data1 = (String)msg.obj;
			product.deserialize(data1);
			Date d = new Date();
			long longtime = d.getTime();
			String order = "" + longtime;
			String userId = order;
			String productDesc = "金币";
			int goods = (int)product.price * 100;
			String goodsPrice = "" + goods;
			PPInterface.startPPPayment(AppActivity.this, order, "", "2015080615",
					userId, goodsPrice, "100001",
					"", "http://www.test.com", productDesc, new HashMap<String, String>(),
					"", (ResponseDataToMerchant)AppActivity.this);
			
			//String pointID = (String)msg.obj;
			//GameChannel.getInstance().notifyPay(pointID);
		}
		else if(Constant.SDK_TYPE_ZHIFU == product.payType){
			String data2 = (String)msg.obj;
			product.deserialize(data2);
			String xml = product.xmlFile;
			startPay(xml);
		}
	}
	
	// 支付返回
	public void payResponse(Message msg){
		final Intent intent = (Intent)msg.obj;
		Cocos2dxHelper.runOnGLThread(new Runnable() {

			@Override
			public void run() {
				GameChannel.getInstance().notifyPayCB(intent);
			}

		});
	}
	
	// 
	public void channelResonpse(Message msg){
		final Object obj = msg.obj;
		Cocos2dxHelper.runOnGLThread(new Runnable() {
				@Override
				public void run() {
					GameChannel.getInstance().notifyResponseChannel(obj);
				}

		});
	}

	// 支付回调更新
	public void updatePayResult() {
		Intent intent = new Intent();
		intent.putExtra("status", status);
		intent.putExtra("identifier", product.identifier);
		
		Message msg = new Message();
		msg.what = Constant.COMMOAND_PAY_RESULT;
		msg.obj = intent;
		uiMessageHandler.sendMessage(msg);	
	}
	
	// 开始支付
	public void startPay(String xml){
		Intent intent = new Intent(this, MerchantPayResultActivity.class);
		intent.putExtra("xml", xml);
		//intent.putExtra("ActivityName", "com.JulyNet.MixProject.MerchantPayResultActivity");
		startActivity(intent);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		CCUMSocialController.initSocialSDK(this, "com.umeng.social.share");
		
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		GameChannel.getInstance().start(this, uiMessageHandler);

		HNPhotoModule photo = HNPhotoModule.getInstance();
		photo.start(this);
		ChannelExport.getInstance().registerModule(photo.GetModuleName(), photo);
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		
		GameChannel.getInstance().onResume(this);
		
		if(MerchantPayResultActivity.isFromPay){
			MerchantPayResultActivity.isFromPay = false;
			
			Intent intent = new Intent();
			intent.putExtra("status", 1);
			intent.putExtra("identifier", product.identifier);
			
			//Log.i(TAG, product.identifier);
			
			Message msg = new Message();
			msg.what = Constant.COMMOAND_PAY_RESULT;
			msg.obj  = intent;
			uiMessageHandler.sendMessage(msg);
		}
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		GameChannel.getInstance().onPause(this);
		super.onPause();
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		GameChannel.getInstance().onDestroy();
		super.onDestroy();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		//super.onActivityResult(requestCode, resultCode, data);
		CCUMSocialController.onActivityResult(requestCode, resultCode, data);
		GameChannel.getInstance().onActivityResult(requestCode, resultCode, data);

		HNPhotoModule.getInstance().onActivityResult(requestCode, resultCode, data);	
		
		// 取支付数据		
		if(data != null){
			boolean isFromPay = data.getBooleanExtra("isFromPay", false);
			if(isFromPay){
				Intent intent = new Intent();
				intent.putExtra("status", status);
				intent.putExtra("identifier", product.identifier);
				
				//Log.i(TAG, product.identifier);
				
				Message msg = new Message();
				msg.what = Constant.COMMOAND_PAY_RESULT;
				msg.obj  = intent;
				uiMessageHandler.sendMessage(msg);
			}
		}	
	}
	
	
}
