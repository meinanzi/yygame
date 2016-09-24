package com.ips.merchant;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.gson.Gson;
import com.ips.commons.security.IpsVerify;
import com.ips.upmp.assist.StartPluginAssist;

public class MainActivity extends Activity {
    protected IPSLogger Logger = IPSLogger.ipsLog();
    private TextView tv_ccyCode, tv_requestTime;
    private EditText et_mer_code, et_accCode, et_mer_bill_no, et_prdCode, et_tran_amt, et_ordPerVal, et_mer_notice_url, et_order_desc, et_bank_card_num;;

    String orderEncodeType = "5";// 签名方式:5#采用MD5签名认证方式 ;
    SimpleDateFormat dateTimeFormatter = new SimpleDateFormat("yyyyMMddHHmmss", Locale.CHINA);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_shengchan_huizhishangmei);//北京绘智尚美网络科技发展有限公司(生产环境)
        
        et_mer_code = (EditText) findViewById(R.id.et_mobilePay_mer_code);
        et_accCode = (EditText) findViewById(R.id.et_mobilePay_accCode);
        et_mer_bill_no = (EditText) findViewById(R.id.et_mobilePay_mer_bill_no);
        tv_ccyCode = (TextView) findViewById(R.id.tv_mobilePay_ccyCode);
        et_prdCode = (EditText) findViewById(R.id.et_mobilePay_prdCode);
        et_tran_amt = (EditText) findViewById(R.id.et_mobilePay_tranAmt);
        tv_requestTime = (TextView) findViewById(R.id.tv_mobilePay_requestTime);
        et_ordPerVal = (EditText) findViewById(R.id.et_mobilePay_ordPerVal);
        et_mer_notice_url = (EditText) findViewById(R.id.et_mobilePay_merNoticeUrl);
        et_order_desc = (EditText) findViewById(R.id.et_mobilePay_orderDesc);
        et_bank_card_num = (EditText) findViewById(R.id.et_mobilePay_bank_card_num);

        Date currentTime = new Date();
        et_mer_bill_no.setText("MER" + dateTimeFormatter.format(currentTime));
        tv_requestTime.setText(dateTimeFormatter.format(currentTime));
    }

    // 重用该实例
    protected void onNewIntent(Intent intent) {
        printExtras(intent.getExtras());
    }

    // 提交进行支付
    public void confirmMobilePay(View v) throws Exception {
        String merCode = et_mer_code.getText().toString().trim();// 商户号
        String accCode = et_accCode.getText().toString().trim();// 交易账户号
        String merBillNo = et_mer_bill_no.getText().toString().trim();// 商户订单号
        String ccyCode = tv_ccyCode.getText().toString().trim();// 币种:156(人民币)
        String prdCode = et_prdCode.getText().toString().trim();// 支付类型：2301(移动无卡支付)
        String tranAmt = et_tran_amt.getText().toString().trim();// 订单金额(单位：元,如10.00)
        String requestTime = tv_requestTime.getText().toString().trim();// 请求时间:格式样例(2015-7-30 00:00:00)
        String ordPerVal = et_ordPerVal.getText().toString().trim();// 订单有效期
        String merNoticeUrl = et_mer_notice_url.getText().toString().trim();// 商户自己系统后台通知URL
        String orderDesc = et_order_desc.getText().toString().trim();// 订单描述
        String bankCard = et_bank_card_num.getText().toString().trim();// 银行卡号

        Bundle mobilePayInfo = new Bundle();
        String sign = "";
        String merRequestInfo = "";
        RequestInfo requestInfo  = new RequestInfo();
        String signInfo = "";
        
        requestInfo.setAccCode(accCode);
        requestInfo.setMerBillNo(merBillNo);
        requestInfo.setCcyCode(ccyCode);
        requestInfo.setPrdCode(prdCode);
        requestInfo.setTranAmt(tranAmt);
        requestInfo.setRequestTime(requestTime);
        requestInfo.setOrdPerVal(ordPerVal);
        requestInfo.setMerNoticeUrl(merNoticeUrl);
        requestInfo.setOrderDesc(orderDesc);
        
        int bankCardLength = bankCard.length();
        if (bankCardLength == 0) {
            merRequestInfo = getMerRequestInfo(requestInfo);
            signInfo = merCode+merRequestInfo;
            sign = IpsVerify.signMD5(signInfo, Constant.MD5_CERT);
        } else if (!CardNoUtils.checkBankCard(bankCard)) {
            Toast.makeText(MainActivity.this, "请输入正确银行卡号", Toast.LENGTH_LONG).show();
            return;
        } else {
            bankCard = DESUtil.encode(Constant.DES_KEY, Constant.DES_IV,bankCard);
            requestInfo.setBankCard(bankCard);
            merRequestInfo = getMerRequestInfo(requestInfo);
            signInfo = merCode+merRequestInfo;
            sign = IpsVerify.signMD5(signInfo, Constant.MD5_CERT);
        }
        mobilePayInfo.putString("merCode", merCode);
        mobilePayInfo.putString("merRequestInfo", merRequestInfo);
        mobilePayInfo.putString("sign", sign);
        mobilePayInfo.putString("orderEncodeType", orderEncodeType);
        mobilePayInfo.putString("bankCard", bankCard);

        StartPluginAssist.start_ips_plugin(MainActivity.this, mobilePayInfo);
    }

    public String getMerRequestInfo(RequestInfo requestInfo) throws Exception{
        Gson gson = new Gson();
        String merRequestInfo;
        merRequestInfo = DESUtil.encode(Constant.DES_KEY, Constant.DES_IV,gson.toJson(requestInfo));
        return merRequestInfo;
    }
    
    // 放弃支付
    public void cancelMobilePay(View v) {
        Toast.makeText(getApplicationContext(), "取消支付", Toast.LENGTH_SHORT).show();
        finish();
    }

    protected void printExtras(Bundle extras) {
        if (extras != null) {
            for (String key : extras.keySet()) {
                Logger.e(key + ":	" + extras.get(key));
            }
        } else {
            Logger.w("Extras is null");
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        finish();
    }
}
