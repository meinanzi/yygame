package com.ips.merchant;

import org.json.JSONObject;

public class DataService {
    protected IPSLogger Logger = IPSLogger.ipsLog();
    
    public String getDatas(String accCode, String merBillNo, String ccyCode, String prdCode, String tranAmt, String requestTime, String ordPerVal,
            String merNoticeUrl, String orderDesc, String bankCard) throws Exception {
        JSONObject param = new JSONObject();
        param.put("accCode", accCode);
        param.put("merBillNo", merBillNo);
        param.put("ccyCode", ccyCode);
        param.put("prdCode", prdCode);
        param.put("tranAmt", tranAmt);
        param.put("requestTime", requestTime);
        param.put("ordPerVal", ordPerVal);
        param.put("merNoticeUrl", merNoticeUrl);
        param.put("orderDesc", orderDesc);
        String bankCards = DESUtil.encode(Constant.DES_KEY, Constant.DES_IV, bankCard);
        param.put("bankCard", bankCards);
        String content = param.toString();
        String result = DESUtil.encode(Constant.DES_KEY, Constant.DES_IV, content);
        Logger.e("with BankCard result");
        return result;
    }

    public String getData(String accCode, String merBillNo, String ccyCode, String prdCode, String tranAmt, String requestTime, String ordPerVal,
            String merNoticeUrl, String orderDesc) throws Exception {
        JSONObject param = new JSONObject();
        param.put("accCode", accCode);
        param.put("merBillNo", merBillNo);
        param.put("ccyCode", ccyCode);
        param.put("prdCode", prdCode);
        param.put("tranAmt", tranAmt);
        param.put("requestTime", requestTime);
        param.put("ordPerVal", ordPerVal);
        param.put("merNoticeUrl", merNoticeUrl);
        param.put("orderDesc", orderDesc);
        String content = param.toString();
        String result = DESUtil.encode(Constant.DES_KEY, Constant.DES_IV, content);
        return result;
    }

}
