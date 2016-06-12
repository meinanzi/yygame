package com.hn.pay;

import org.json.JSONException;
import org.json.JSONObject;

import com.hn.pay.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import com.hn.pay.base.PayResultCallBack;
import com.hn.pay.base.ProductInfo;
import com.hn.pay.sdk.AlipayManager;

public class HNPayActivity extends Activity {
	private int COMMOAND_PAY_RESULT = 0;
	private int status = 0;
	
	private ProductInfo product = new ProductInfo();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_hnpay);

		COMMOAND_PAY_RESULT = this.getIntent().getIntExtra("COMMOAND_PAY_RESULT", 0);
		
		String data = this.getIntent().getStringExtra("PAY_DATA");
		product.deserialize(data);
		
		TextView product_description = (TextView) findViewById(R.id.textView_product_description);
		product_description.setText(String.format("商品： %d 筹码", product.number));
		
		TextView product_money = (TextView) findViewById(R.id.textView_product_money);
		product_money.setText(String.format("价格： %.2f元", product.price));
		
		ImageView aliPay = (ImageView) findViewById(R.id.imageView_Alipay);
		aliPay.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				AlipayManager alipay = new AlipayManager(HNPayActivity.this);
				alipay.pay(String.format("%d筹码", product.number), 
						String.format("%.2f元购买%d 筹码", product.price, product.number), 
						String.format("%.2f", product.price), product.orderID,
						new PayResultCallBack() {

							@Override
							public void onPayResult(PAY_SDK sdk, int status) {

								HNPayActivity.this.status = status;

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
									Toast.makeText(HNPayActivity.this, "支付成功",
											Toast.LENGTH_SHORT).show();
									updatePayResult();
								} else {
									Toast.makeText(HNPayActivity.this, "支付失败",
											Toast.LENGTH_SHORT).show();
								}
							}

						});
			}
		});

		ImageView weChat = (ImageView) findViewById(R.id.imageView_WeChat);
		weChat.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				Toast.makeText(HNPayActivity.this, "微信支付", Toast.LENGTH_SHORT)
						.show();
			}
		});

		ImageView unionPay = (ImageView) findViewById(R.id.imageView_UnionPay);
		unionPay.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				Toast.makeText(HNPayActivity.this, "银联支付", Toast.LENGTH_SHORT)
						.show();
			}
		});

		ImageView back = (ImageView) findViewById(R.id.imageView_Back);
		back.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				updatePayResult();
				finish();
			}
		});
	}

	@Override
	public void onBackPressed() {
		updatePayResult();
		super.onBackPressed();
	}

	public void updatePayResult() {
		Intent intent = new Intent();
		intent.putExtra("COMMOAND_PAY_RESULT", COMMOAND_PAY_RESULT);
		intent.putExtra("status", status);
		intent.putExtra("identifier", product.identifier);
		setResult(COMMOAND_PAY_RESULT, intent);
	}
}
