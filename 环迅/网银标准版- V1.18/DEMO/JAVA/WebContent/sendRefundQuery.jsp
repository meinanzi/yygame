<%@ page language="java" contentType="text/html; charset=GBK"
    pageEncoding="GBK"%>
<%@ page import="java.util.Map" import="java.util.HashMap" import="com.yeepay.*"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<%! String formatString(String text){
	return text==null ? "" : text;
	}
%>
<%
	request.setCharacterEncoding("GBK");
	String p2_Order			= formatString(request.getParameter("p2_Order"));
	String pb_TrxId			= formatString(request.getParameter("pb_TrxId"));	
	
	Map<String, String> refundQueryParams = new HashMap<String, String>();
	refundQueryParams.put("p2_Order", p2_Order);
	refundQueryParams.put("pb_TrxId", pb_TrxId);

	Map<String, String> refundQueryResult = YeepayService.refundQuery(refundQueryParams);
	String r0_Cmd			   = formatString(refundQueryResult.get("r0_Cmd"));
    String r1_Code             = formatString(refundQueryResult.get("r1_Code"));
    String r2_TrxId            = formatString(refundQueryResult.get("r2_TrxId"));
    String r4_Order            = formatString(refundQueryResult.get("r4_Order"));
    String refundStatus        = formatString(refundQueryResult.get("refundStatus"));
    String refundFrpStatus     = formatString(refundQueryResult.get("refundFrpStatus"));
	String hmacError		   = formatString(refundQueryResult.get("hmacError"));
	String errorMsg			   = formatString(refundQueryResult.get("errorMsg"));

	if("".equals(errorMsg)) {
		if("".equals(hmacError)) {
	if(!r1_Code.equals("1")) {
		if(r1_Code.equals("-1")) {
			out.println("请求参数不合法");
			out.println("<br> r1_Code : " + r1_Code);
		} else if(r1_Code.equals("-2")) {
			out.println("商户不存在");
			out.println("<br> r1_Code  : " + r1_Code);
			out.println("<br> p1_MerId : " + Configuration.getInstance().getValue("p1_MerId"));
		} else if(r1_Code.equals("-3")) {
			out.println("给定的流水号、退款请求号，没有对应的退款记录");
			out.println("<br> r1_Code : " + r1_Code);
			out.println("<br> p2_Order : " + p2_Order);
			out.println("<br> pb_TrxId : " + pb_TrxId);
		} else { 
			out.println("订单查询失败！！！");
			out.println("</br> Error code :  r1_Code = " + r1_Code);
		}
		return;
	}
		} else {
	out.println("Hmac Error - " + hmacError);
	return;
		}
	} else {
	out.println("</br> errorMsg : " + errorMsg);
	return;
	}
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<title>退款查询结果</title>
</head>
	<body>	
		<br /> <br />
		<table width="70%" border="0" align="center" cellpadding="5" cellspacing="0" style="border:solid 1px #107929">
			<tr>
		  		<th align="center" height="30" colspan="5" bgcolor="#6BBE18">
					退款查询结果
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
				<td width="25%" align="left">&nbsp;查询结果</td>
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
				<td width="25%" align="left">&nbsp;退款请求号</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r4_Order%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r4_Order</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;退款申请结果</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=refundStatus%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">refundStatus</td> 
			</tr>


			<tr>
				<td width="25%" align="left">&nbsp;退至银行状态</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=refundFrpStatus%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">refundFrpStatus</td> 
			</tr> 


		</table>

		<br>
		<table  width="70%" align="center">
			<tr>
				<td>
					<span style="font-size:12px; font-weight:100;"> 
						退款申请结果 : 1 - 退款申请成功。[ - refundStatus]
					</span>
				</td>
			</tr>

			<tr>
				<td>
					<span style="font-size:12px; font-weight:100;"> 
						退至银行状态 : 1 - 申请处理中；2 - 处理成功；3 - 处理失败；4 - 已退回。[ - refundFrpStatus]
					</span>
				</td>
			</tr>
		</table>

	</body>
</html>
