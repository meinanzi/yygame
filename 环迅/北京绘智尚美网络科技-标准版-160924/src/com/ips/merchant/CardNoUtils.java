package com.ips.merchant;

/**
 * 银行卡号校验工具类
 * @author IH847
 *
 */
public class CardNoUtils {
    //public static void main(String[] args) {
    //    //String card = "36088388888888";//工银大来信用卡长度为14位        
    //    //String card = "377155788888888";//民生运通信用卡长度为15位
    //    //String card = "6225881414207430";//招商银行储蓄卡16位
    //    //String card = "6227007200120897790";//建行储蓄卡19位
    //    System.out.println("      card: " + card);
    //    System.out.println("check code: " + getBankCardCheckCode(card.substring(0, card.length() - 1)));
    //    System.out.println("是否为银行卡:" + checkBankCard(card));
    //}

    /**
     * 校验银行卡卡号
     * 
     * @param bankCard
     * @return
     */
    public static boolean checkBankCard(String bankCard) {
        char bit = getBankCardCheckCode(bankCard.substring(0, bankCard.length() - 1));
        if (bit == 'N') {
            return false;
        }
        return bankCard.charAt(bankCard.length() - 1) == bit;
    }

    /**
     * 从不含校验位的银行卡卡号采用 Luhm校验算法获得校验位
     * @param nonCheckCodeCardId
     * @return
     */
    private static char getBankCardCheckCode(String nonCheckCodeCardId) {
        if (nonCheckCodeCardId == null || nonCheckCodeCardId.trim().length() == 0 || !nonCheckCodeCardId.matches("\\d+")
                || nonCheckCodeCardId.trim().length() < 13 || nonCheckCodeCardId.trim().length() > 18) {
            return 'N';
        }
        char[] chs = nonCheckCodeCardId.trim().toCharArray();
        int luhmSum = 0;
        for (int i = chs.length - 1, j = 0; i >= 0; i--, j++) {
            int k = chs[i] - '0';
            if (j % 2 == 0) {
                k *= 2;
                k = k / 10 + k % 10;
            }
            luhmSum += k;
        }
        return (luhmSum % 10 == 0) ? '0' : (char) ((10 - luhmSum % 10) + '0');
    }
}
