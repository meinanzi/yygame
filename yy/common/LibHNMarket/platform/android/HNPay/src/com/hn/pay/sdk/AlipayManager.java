package com.hn.pay.sdk;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;

import android.app.Activity;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.View;
import android.widget.Toast;

import com.alipay.sdk.app.PayTask;
import com.hn.pay.base.PayResultCallBack;
import com.hn.pay.base.PayResultCallBack.PAY_SDK;

public class AlipayManager {
	public static final String PARTNER = "2088421471797022";
	public static final String SELLER = "2088421471797022";
	public static final String RSA_PRIVATE = "MIICXAIBAAKBgQCwXb4hCkjOvTtiOO+5IzQDbBEVCKlt9/ZxKL8omrbZiNUnA3ye"
		"upD+EFH1m8vGaZCG5xfez8ZaCnh4tOHib3wM8HzFcPXZkugFCkZTDnyrpgjt8YNt"
		"L55AazJXckze7UmWGc/musXirI/7IZQZ8ukrLfASMbQ39X6yeHPEevDLEQIDAQAB"
		"AoGAYL37JgqLJ4Rbh6RtsuNhi6MCt8P5lE3ZC/dq7C35sNfC/FSITlVOZV60ZoJg"
		"Q+TyF126FUlpp0hpDuVBpsC6jUD9kx9K48UUt7pHldMuzh9uwlfvnJV/0JPjTPOn"
		"OFWbWCUnPx/bI2V4dGuvyx+kB7dlKTUrXujkU/4Mr7aKPKUCQQDg06z5A9OGGrpn"
		"Tzao/6DhLjbf4pEJGIv0Tdvk00rMIjbgQ64RZauOFRnhJrSUPWE8zWFWpRGOiiT/"
		"IycdIeCDAkEAyNHvUryvdqtYnZKCTW21PArvMXXfaUQ18bZzzEhdNkjhBPRzzW24"
		"8PoCIRXdESQ2gGhlXl628HTy3OLDyKBp2wJABUo1ua4zXeZnGGMuwA8zs9ioheAI"
		"1ZP0Rw1F7T5jdiYf8GrnP5WK0c8BSuDkQtCm5w1WCvnbTeVGNZKYKwFPDQJAUZXA"
		"dcuEOWDZSoLX/x7f7zm9/UkW74wvojhuiQKpI8Gt8TZiyYWXCgd3z/sy6aDdp4NC"
		"E3nFNqE8fXJ++cX52wJBALProEMAIVTylgQBL9P8MQAq15grei73DX8gMZ5BoUi8"
		"amuGFDdoLkmBnIDj8k06rPV6yYp3yRwPgBD3k3/iVBo=";
	
	public static final String NOTIFY_URL = "https://mapi.alipay.com/gateway.do?";

	private static final int SDK_PAY_FLAG = 1;

	private static final int SDK_CHECK_FLAG = 2;

	private Activity activity;

	private PayResultCallBack callback;

	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG: {
				Result resultObj = new Result((String) msg.obj);
				String resultStatus = resultObj.resultStatus;

				// 判断resultStatus 为“9000”则代表支付成功，具体状态码代表含义可参考接口文档
				if (TextUtils.equals(resultStatus, "9000")) {
					callback.onPayResult(PAY_SDK.ALIPAY, 1);
				} else {
					// 判断resultStatus 为非“9000”则代表可能支付失败
					// “8000”
					// 代表支付结果因为支付渠道原因或者系统原因还在等待支付结果确认，最终交易是否成功以服务端异步通知为准（小概率状态）
					if (TextUtils.equals(resultStatus, "8000")) {
						Toast.makeText(activity, "支付结果确认中", Toast.LENGTH_SHORT)
								.show();

					} else {
						Toast.makeText(activity, "支付失败", Toast.LENGTH_SHORT)
								.show();
						callback.onPayResult(PAY_SDK.ALIPAY, 0);
					}
				}
				break;
			}
			case SDK_CHECK_FLAG: {
				Toast.makeText(activity, "检查结果为：" + msg.obj, Toast.LENGTH_SHORT)
						.show();
				break;
			}
			default:
				break;
			}
		};
	};

	public AlipayManager(Activity activity) {
		this.activity = activity;
	}

	/**
	 * call alipay sdk pay. 调用SDK支付
	 * 
	 */
	public void pay(String product, String productDescription,
			String productPrice,  String orderID, PayResultCallBack callback) {
		this.callback = callback;
		String orderInfo = getOrderInfo(product, productDescription, orderID,
				 productPrice);
		String sign = sign(orderInfo);
		try {
			// 仅需对sign 做URL编码
			sign = URLEncoder.encode(sign, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"
				+ getSignType();

		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				// 构造PayTask 对象
				PayTask alipay = new PayTask(activity);
				// 调用支付接口
				String result = alipay.pay(payInfo);

				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}

	/**
	 * check whether the device has authentication alipay account.
	 * 查询终端设备是否存在支付宝认证账户
	 * 
	 */
	public void check() {
		Runnable checkRunnable = new Runnable() {

			@Override
			public void run() {
				PayTask payTask = new PayTask(activity);
				boolean isExist = payTask.checkAccountIfExist();

				Message msg = new Message();
				msg.what = SDK_CHECK_FLAG;
				msg.obj = isExist;
				mHandler.sendMessage(msg);
			}
		};

		Thread checkThread = new Thread(checkRunnable);
		checkThread.start();

	}

	/**
	 * get the sdk version. 获取SDK版本号
	 * 
	 */
	public void getSDKVersion() {
		PayTask payTask = new PayTask(activity);
		String version = payTask.getVersion();
		Toast.makeText(activity, version, Toast.LENGTH_SHORT).show();
	}

	/**
	 * create the order info. 创建订单信息
	 * 
	 */
	public String getOrderInfo(String subject, String body, String price, String orderID) {
		
		// 参数编码， 固定值
		String orderInfo = "_input_charset=\"utf-8\"";
		
		// 商品详情
		orderInfo += "&body=" + "\"" + body + "\"";
		
		// 设置未付款交易的超时时间
				// 默认30分钟，一旦超时，该笔交易就会自动被关闭。
				// 取值范围：1m～15d。
				// m-分钟，h-小时，d-天，1c-当天（无论交易何时创建，都在0点关闭）。
				// 该参数数值不接受小数点，如1.5h，可转换为90m。
		orderInfo += "&it_b_pay=\"30m\"";
		
		// 服务器异步通知页面路径
		orderInfo += "&notify_url=" + "\"" + NOTIFY_URL + "\"";
		
		// 商户网站唯一订单号
		orderInfo += "&out_trade_no=" + "\"" + orderID + "\"";
				
		// 合作者身份ID
		orderInfo += "&partner=" + "\"" + PARTNER + "\"";
		
		// 支付类型， 固定值
		orderInfo += "&payment_type=\"1\"";

		// 卖家支付宝账号
		orderInfo += "&seller_id=" + "\"" + SELLER + "\"";
		
		// 接口名称， 固定值
		orderInfo += "&service=\"mobile.securitypay.pay\"";

		// 商品名称
		orderInfo += "&subject=" + "\"" + subject + "\"";

		// 商品金额
		orderInfo += "&total_fee=" + "\"" + price + "\"";
		
		// 支付宝处理完请求后，当前页面跳转到商户指定页面的路径，可空
		// orderInfo += "&return_url=\"m.alipay.com\"";

		// 调用银行卡支付，需配置此参数，参与签名， 固定值
		// orderInfo += "&paymethod=\"expressGateway\"";

		return orderInfo;
	}

	/**
	 * get the out_trade_no for an order. 获取外部订单号
	 * 
	 */
	public String getOutTradeNo() {
		SimpleDateFormat format = new SimpleDateFormat("MMddHHmmss",
				Locale.getDefault());
		Date date = new Date();
		String key = format.format(date);

		Random r = new Random();
		key = key + r.nextInt();
		key = key.substring(0, 15);
		return key;
	}

	/**
	 * sign the order info. 对订单信息进行签名
	 * 
	 * @param content
	 *            待签名订单信息
	 */
	public String sign(String content) {
		return SignUtils.sign(content, RSA_PRIVATE);
	}

	/**
	 * get the sign type we use. 获取签名方式
	 * 
	 */
	public String getSignType() {
		return "sign_type=\"RSA\"";
	}

}
