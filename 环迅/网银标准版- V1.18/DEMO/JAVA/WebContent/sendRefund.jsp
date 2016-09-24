<%@ page language="java" contentType="text/html; charset=GBK" pageEncoding="GBK"%>
<%@ page import="com.yeepay.*" import="java.util.Map, java.util.HashMap"%>
<%! String formatString(String text) {
		return text == null ? "" : text.trim();
	}
%>
<%
	request.setCharacterEncoding("GBK");
	
	String p0_Cmd				= formatString(request.getParameter("p0_Cmd"));
	String p2_order				= formatString(request.getParameter("p2_order"));
	String pb_TrxId				= formatString(request.getParameter("pb_TrxId"));
	String p3_Amt				= formatString(request.getParameter("p3_Amt"));
	String p4_Cur				= formatString(request.getParameter("p4_Cur"));
	String p5_Desc				= formatString(request.getParameter("p5_Desc"));
	
	Map<String, String> params	= new HashMap<String, String>();
	params.put("p0_Cmd", p0_Cmd);
	params.put("p2_order", p2_order);
	params.put("pb_TrxId", pb_TrxId);
	params.put("p3_Amt", p3_Amt);
	params.put("p4_Cur", p4_Cur);
	params.put("p5_Desc", p5_Desc);

	Map<String, String> refundResult = YeepayService.refundByTrxId(params);
	String r0_Cmd		= formatString(refundResult.get("r0_Cmd"));
	String r1_Code		= formatString(refundResult.get("r1_Code"));
	String r2_TrxId		= formatString(refundResult.get("r2_TrxId"));
	String r3_Amt		= formatString(refundResult.get("r3_Amt"));
	String r4_Cur		= formatString(refundResult.get("r4_Cur"));
	String r4_Order		= formatString(refundResult.get("r4_Order"));
	String rf_fee		= formatString(refundResult.get("rf_fee"));
	String hmacError	= formatString(refundResult.get("hmacError"));
	String errorMsg		= formatString(refundResult.get("errorMsg"));

	if(errorMsg.equals("")) {
		if(hmacError.equals("")) {
	if(r1_Code.equals("2")) {
		out.println("账户状态无效");
		return;
	} else if(r1_Code.equals("7")) {
		out.println("该订单不支持退款");
		return;
	} else if(r1_Code.equals("10")) {
		out.println("退款金额超限");
		return;
	} else if(r1_Code.equals("18")) {
		out.println("余额不足");
		return;
	} else if(r1_Code.equals("50")) {
		out.println("订单不存在");
		return;
	} else if(r1_Code.equals("55")) {
		out.println("历史退款未开通");
		return;
	} else if(r1_Code.equals("6801")) {
		out.println("IP限制");
		return;
	} 
		} else {
	out.println("Hmac error !");
	out.println(hmacError);
	return;
		}
	} else {
		out.println("Refund Failed!");
		out.println(errorMsg);
		return;
	}
%>
<html>
<head>
	<title>退款接口</title>
</head>
	<body>
		<br /> <br />
		<table width="70%" border="0" align="center" cellpadding="5" cellspacing="0" style="border:solid 1px #107929">
			<tr>
		  		<th align="center" height="30" colspan="5" bgcolor="#6BBE18">
					订单退款结果
				</th>
		  	</tr>

			<tr >
				<td width="25%" align="left">&nbsp;业务类型</td>
				<td width="5%"  align="center"> : </td> 
				<td width="45"  align="left"> <%=r0_Cmd%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r0_Cmd</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;退款结果</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r1_Code%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r1_Code</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;易宝流水号</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r2_TrxId%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r2_TrxId</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;支付金额</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r3_Amt%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r3_Amt</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;交易币种</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r4_Cur%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r4_Cur</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;退款请求号</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r4_Order%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r4_Order</td> 
			</tr> 

			<tr>
				<td width="25%" align="left">&nbsp;已退手续费</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=rf_fee%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">rf_fee</td> 
			</tr> 

		</table>
	</body>
</html>
