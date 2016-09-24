/**
 * 
 */
package com.ips.merchant;
/**
 * @author IH847
 *
 */
public class RequestInfo {
    private String accCode;
    private String merBillNo;
    private String ccyCode;
    private String prdCode;
    private String tranAmt;
    private String requestTime;
    private String ordPerVal;
    private String merNoticeUrl;
    private String orderDesc;
    private String bankCard;

    public String getAccCode() {
        return accCode;
    }

    public void setAccCode(String accCode) {
        this.accCode = accCode;
    }

    public String getMerBillNo() {
        return merBillNo;
    }

    public void setMerBillNo(String merBillNo) {
        this.merBillNo = merBillNo;
    }

    public String getCcyCode() {
        return ccyCode;
    }

    public void setCcyCode(String ccyCode) {
        this.ccyCode = ccyCode;
    }

    public String getPrdCode() {
        return prdCode;
    }

    public void setPrdCode(String prdCode) {
        this.prdCode = prdCode;
    }

    public String getTranAmt() {
        return tranAmt;
    }

    public void setTranAmt(String tranAmt) {
        this.tranAmt = tranAmt;
    }

    public String getRequestTime() {
        return requestTime;
    }

    public void setRequestTime(String requestTime) {
        this.requestTime = requestTime;
    }

    public String getOrdPerVal() {
        return ordPerVal;
    }

    public void setOrdPerVal(String ordPerVal) {
        this.ordPerVal = ordPerVal;
    }

    public String getMerNoticeUrl() {
        return merNoticeUrl;
    }

    public void setMerNoticeUrl(String merNoticeUrl) {
        this.merNoticeUrl = merNoticeUrl;
    }

    public String getOrderDesc() {
        return orderDesc;
    }

    public void setOrderDesc(String orderDesc) {
        this.orderDesc = orderDesc;
    }

    public String getBankCard() {
        return bankCard;
    }

    public void setBankCard(String bankCard) {
        this.bankCard = bankCard;
    }

}
