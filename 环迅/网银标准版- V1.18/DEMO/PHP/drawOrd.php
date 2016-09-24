<?php
header("Content-type: text/html; charset=gb2312"); 
/*
 * @Description 易宝支付产品通用支付接口范例 
 * @V3.0
 * @Author xin.li
 */
 	
include 'yeepayCommon.php';
require_once 'HttpClient.class.php';
 		
$p0_Cmd 	= "CancelOrd";	            #接口类型
$pb_TrxId = $_POST['pb_TrxId'];						#商户订单号
$pv_Ver = "1";							#版本号
#正式请求地址
#$CancelOrdURL_onLine	= "https://www.yeepay.com/app-merchant-proxy/command";			
$CancelOrdURL_onLine="https://cha.yeepay.com/app-merchant-proxy/command";			
#测试请求地址					
#$CancelOrdURL_onLine	= "http://tech.yeepay.com:8080/robot/debug.action";									
    
#	进行签名处理，一定按照文档中标明的签名顺序进行
$sbOld ="";
#	加入订单撤销请求，固定值"CancelOrd"
$sbOld = $sbOld.$p0_Cmd;
#	加入商户编号
$sbOld = $sbOld.$p1_MerId;
#	加入商户订单号
$sbOld = $sbOld.$pb_TrxId;
#	加入版本号
$sbOld = $sbOld.$pv_Ver;
                   
$hmac	 = null;
$hmac	 = HmacMd5($sbOld,$merchantKey);     
#echo "==================".$sbOld;           
	logstr($pb_TrxId,$sbOld,HmacMd5($sbOld,$merchantKey));
           
#	进行签名处理，一定按照文档中标明的签名顺序进行
#	加入订单撤销请求，固定值"CancelOrd"
$params = array('p0_Cmd' => $p0_Cmd,
#	加入商户编号
'p1_MerId'	=>  $p1_MerId,
#	加入商户订单号
'pb_TrxId'	=>  $pb_TrxId,
#	加入版本号
'pv_Ver'=>  $pv_Ver,
#	加入校验码
'hmac' 			=>  $hmac);


$pageContents = HttpClient::quickPost($CancelOrdURL_onLine, $params);

$result = explode("\n",$pageContents);
#echo "=======".$pageContents;
## 声明查询结果
	$r0_Cmd					= "";			#	取得业务类型
	$r1_Code				= "";     #	撤销结果
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
		}elseif($sKey == "hmac"){									#取得校验码
			$hmac = $sValue;	      
		}
	}
		

	#进行校验码检查 取得加密前的字符串
	$sbOld="";
	#加入业务类型
	$sbOld = $sbOld.$r0_Cmd;
	#加入撤销操作是否成功
	$sbOld = $sbOld.$r1_Code;              
            	
 // echo "[".$sbOld."]";
  //echo $sNewString;  
  //echo $sNewString;
  
	$sNewString = HmacMd5($sbOld,$merchantKey);
	
	#logstr($r6_Order,$sbOld,HmacMd5($sbOld,$merchantKey));
	//校验码正确
	if($sNewString==$hmac) {
	  if($r1_Code=="1"){
	      echo "<br>撤销成功!";
	     
	  } else if($r1_Code=="53"){
	      echo "<br>订单已经成功,不可撤销";
	      exit; 
	  } else{
	      echo "<br>撤销失败";	
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
