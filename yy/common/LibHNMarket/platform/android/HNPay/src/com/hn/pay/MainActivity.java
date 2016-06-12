package com.hn.pay;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.PopupWindow;

public class MainActivity extends Activity {
 
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		this.initView();
	}

	private void initView() {
		Button btn = (Button) findViewById(R.id.button);
		btn.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				//View view = LayoutInflater.from(MainActivity.this).inflate(
				//		R.layout.activity_pay, null);
				//new AlertDialog.Builder(MainActivity.this).setView(view).show();
				//startActivity(new Intent(MainActivity.this, ShowActivity.class));
				startActivity(new Intent(MainActivity.this, HNPayActivity.class));
			}
		});
	}

}
