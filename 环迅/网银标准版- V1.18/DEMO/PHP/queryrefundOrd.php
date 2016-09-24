<?php
header("Content-type: text/html; charset=gb2312"); 
/*
 * @Description 易宝支付产品通用支付接口范例 
 * @V3.0
 * @Author xin.li
 */
 	
include 'yeepayCommon.php';
require_once 'HttpClient.class.php';
 		
$p0_Cmd 	= "RefundResults";	            #接口类型
$p2_Order = $_POST['p2_Order'];						#商户订单号
$pb_TrxId = $_POST['pb_TrxId'];
#正式请求地址
$RefundResultsURL_onLine	= "https://www.yeepay.com/app-merchant-proxy/node";			
#$QueryOrdURL_onLine	= "https://cha.yeepay.com/app-merchant-proxy/node";			
#测试请求地址					
#$QueryOrdURL_onLine	= "http://tech.yeepay.com:8080/robot/debug.action";									
    
#	进行签名处理，一定按照文档中标明的签名顺序进行
$sbOld ="";
#	加入订单查询请求，固定值"QueryOrdDetail"
$sbOld = $sbOld.$p0_Cmd;
#	加入商户编号
$sbOld = $sbOld.$p1_MerId;
#	加入商户订单号
$sbOld = $sbOld.$p2_Order;
#	加入易宝交易流水号
$sbOld = $sbOld.$pb_TrxId;
                   
$hmac	 = null;
$hmac	 = HmacMd5($sbOld,$merchantKey);     
           
	logstr($p2_Order,$sbOld,HmacMd5($sbOld,$merchantKey));
           
#	进行签名处理，一定按照文档中标明的签名顺序进行
#	加入退款查询请求，固定值"RefundResults"
$params = array('p0_Cmd' => $p0_Cmd,
#	加入商户编号
'p1_MerId'	=>  $p1_MerId,
#	加入商户订单号
'p2_Order'	=>  $p2_Order,
#	加入易宝交易流水号
'pb_TrxId'	=>  $pb_TrxId,
#	加入校验码
'hmac' 			=>  $hmac);


$pageContents = HttpClient::quickPost($RefundResultsURL_onLine, $params);


$result = explode("\n",$pageContents);
 echo $p2_Order;

## 声明查询结果
	$r0_Cmd					= "";			#	取得业务类型
	$r1_Code				= "";     #	查询结果状态码
	$r2_TrxId				= "";			#	易宝支付交易流水号
	$r4_Order				= "";			#	退款请求号
	$refundStatus	= "";			#退款申请结果
	$refundFrpStatus		= "";			#	退至银行状态
	$hmac						= "";     #	查询返回数据的签名串
    		
	for($index=0;$index<count($result);$index++){//数组循环
		$result[$index] = trim($result[$index]);
		if (strlen($result[$index]) == 0) {
			continue;
		}
		$aryReturn = explode("=",$result[$index]);
		$sKey = $aryReturn[0];
		#$sValue = $aryReturn[1];
		$sValue = urldecode($aryReturn[1]);
		if($sKey=="r0_Cmd"){											#业务类型 
			$r0_Cmd = $sValue;
		}elseif($sKey=="r1_Code"){								#查询结果状态码  
			$r1_Code = $sValue;
		}elseif($sKey == "r2_TrxId"){			        #易宝支付交易流水号
			$r2_TrxId = $sValue;
		}elseif($sKey == "r4_Order"){			          #退款请求号
			$r4_Order = $sValue;
		}elseif($sKey == "refundStatus"){			          #退款申请结果
			$refundStatus = $sValue;
		}elseif($sKey == "refundFrpStatus"){			          #退至银行状态
			$refundFrpStatus = $sValue;
		}elseif($sKey == "hmac"){									#取得校验码
			$hmac = $sValue;	      
		}
	}
		

	#进行校验码检查 取得加密前的字符串
	$sbOld="";
	#加入业务类型
	$sbOld = $sbOld.$r0_Cmd;
	#加入查询操作是否成功
	$sbOld = $sbOld.$r1_Code;
	#加入易宝支付交易流水号
	$sbOld = $sbOld.$r2_TrxId;
	#加入退款请求号
	$sbOld = $sbOld.$r4_Order;	
	#加入退款申请结果
	$sbOld = $sbOld.$refundStatus;	
	#加入退至银行状态
	$sbOld = $sbOld.$refundFrpStatus;	
	            
            	
  //echo "[".$sbOld."]";
  
  //echo $sNewString;  
  //echo $sNewString;
  
	$sNewString = HmacMd5($sbOld,$merchantKey);
	
	logstr($r4_Order,$sbOld,HmacMd5($sbOld,$merchantKey));
	//校验码正确
	if($sNewString==$hmac) {
	  if($r1_Code=="1"){
	      echo "<br>查询成功!";
	      echo "<br>易宝交易流水号:".$r2_TrxId;
	      echo "<br>退款请求号:".$r4_Order;
		  echo "<br>退款申请结果:".$refundStatus;
	      echo "<br>退至银行状态:".$refundFrpStatus;
	
	  } else if($r1_Code=="50"){
	      echo "<br>该订单不存在";
		 
	      exit; 
	  } else{
	      echo "<br>查询失败";	
	      exit;       
	  }
	} else{
		echo "<br>localhost:".$sNewString;	
		echo "<br>YeePay:".$hmac;
		echo "<br>交易信息被篡改";
		exit; 
	}
 
	function decode($text) {
			return str_replace("+", " ", $text);
	}
     
?> 
<html>
<head>
<title>To YeePay Page</title>
</head>
<body>
</html>
