<?php
header("Content-type: text/html; charset=gb2312"); 
/*
 * @Description 易宝支付产品通用支付接口范例 
 * @V3.0
 * @Author xin.li
 */

include 'yeepayCommon.php';	
		
##易宝支付平台统一使用GBK/GB2312编码方式,参数如用到中文，请注意转码

$p0_Cmd           ="Load";
#	商户订单号,选填.
##若不为""，提交的订单号必须在自身账户交易中唯一;为""时，易宝支付会自动生成随机的商户订单号.
$p2_Order					= $_REQUEST['p2_Order'];
#	支付金额,必填.
##单位:元，精确到分.
$p3_Amt						= $_REQUEST['p3_Amt'];
#	交易币种,固定值"CNY".
$p4_Cur						= "CNY";
#	商品名称
##用于支付时显示在易宝支付网关左侧的订单产品信息.
$p5_Pid						= $_REQUEST['p5_Pid'];
#	商品种类
$p6_Pcat					= $_REQUEST['p6_Pcat'];
#	商品描述
$p7_Pdesc					= $_REQUEST['p7_Pdesc'];
#	商户接收支付成功数据的地址,支付成功后易宝支付会向该地址发送两次成功通知.
$p8_Url						= $_REQUEST['p8_Url'];	
#	商户扩展信息
##商户可以任意填写1K 的字符串,支付成功时将原样返回.												
$pa_MP						= $_REQUEST['pa_MP'];
#	扩展属性
$pa_Ext	          = $_REQUEST['pa_Ext'];
#	操作员
$pb_Oper        	= $_REQUEST['pb_Oper'];
#	支付通道编码
##默认为""，到易宝支付网关.若不需显示易宝支付的页面，直接跳转到各银行支付页面，该字段可依照附录:银行列表设置参数值.			
$pd_FrpId					= $_REQUEST['pd_FrpId'];
#	银行分行
$pd_BankBranch	  = $_REQUEST['pd_BankBranch'];
#	充值目标账号
$pt_ActId       	= $_REQUEST['pt_ActId'];
#	版本号
$pv_Ver	          =  $_REQUEST['pv_Ver'];

#调用签名函数生成签名串
$hmac = getCzHmacString($p0_Cmd,$p2_Order,$p3_Amt,$p4_Cur,$p5_Pid,$p6_Pcat,$p7_Pdesc,$p8_Url,$pa_MP,$pa_Ext,$pb_Oper,$pd_FrpId,$pd_BankBranch,$pt_ActId,$pv_Ver);
     
?> 
<html>
<head>
<title>To YeePay Page</title>
</head>
<body onload="document.yeepay.submit();">
<form name='yeepay' action='<?php echo $reqURL_onLine; ?>' method='post'>
<input type='hidden' name='p0_Cmd'					value='<?php echo $p0_Cmd; ?>'>
<input type='hidden' name='p1_MerId'				value='<?php echo $p1_MerId; ?>'>
<input type='hidden' name='p2_Order'				value='<?php echo $p2_Order; ?>'>
<input type='hidden' name='p3_Amt'					value='<?php echo $p3_Amt; ?>'>
<input type='hidden' name='p4_Cur'					value='<?php echo $p4_Cur; ?>'>
<input type='hidden' name='p5_Pid'					value='<?php echo $p5_Pid; ?>'>
<input type='hidden' name='p6_Pcat'					value='<?php echo $p6_Pcat; ?>'>
<input type='hidden' name='p7_Pdesc'				value='<?php echo $p7_Pdesc; ?>'>
<input type='hidden' name='p8_Url'					value='<?php echo $p8_Url; ?>'>
<input type='hidden' name='pa_MP'						value='<?php echo $pa_MP; ?>'>
<input type='hidden' name='pa_Ext'					value='<?php echo $pa_Ext; ?>'>
<input type='hidden' name='pb_Oper'					value='<?php echo $pb_Oper; ?>'>
<input type='hidden' name='pd_FrpId'				value='<?php echo $pd_FrpId; ?>'>
<input type='hidden' name='pm_Period'				value='<?php echo $pm_Period; ?>'>
<input type='hidden' name='pd_BankBranch'		value='<?php echo $pd_BankBranch; ?>'>
<input type='hidden' name='pt_ActId'	      value='<?php echo $pt_ActId; ?>'>
<input type='hidden' name='pv_Ver'	        value='<?php echo $pv_Ver; ?>'>
<input type='hidden' name='hmac'						value='<?php echo $hmac; ?>'>
</form>
</body>
</html>