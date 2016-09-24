<%@ page language="java" contentType="text/html; charset=gbk" pageEncoding="gbk"%>
<%@ page import="com.yeepay.*" import="java.util.*" %>
<%! String formatString(String text) {
		return text==null ? "" : text.trim();
	}
%>
<%
	String p0_Cmd				= formatString(request.getParameter("p0_Cmd"));
	String pb_TrxId				= formatString(request.getParameter("pb_TrxId"));
	
	Map<String, String> params	= new HashMap<String, String>();
	params.put("p0_Cmd", p0_Cmd);
	params.put("pb_TrxId", pb_TrxId);
	
	Map<String, String> cancelResult	= YeepayService.cancelOrder(params);
	String r0_Cmd		= formatString(cancelResult.get("r0_Cmd"));
	String r1_Code		= formatString(cancelResult.get("r1_Code"));
	String errorMsg		= formatString(cancelResult.get("errorMsg"));
	String hmacError	= formatString(cancelResult.get("hmacError"));

	if(!"".equals(errorMsg)) {
		out.println("error msg : " + errorMsg);
		return;
	} else if(!"".equals(hmacError)) {
		out.println("hamc error : " + hmacError);
		return;
	} else if(!"1".equals(r1_Code)) {
		out.println("取消失败");
		out.println("<br>r1_Code : " + r1_Code);
		out.println("<br>pb_TrxId : " + pb_TrxId);
		return;
	}
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<title>订单取消成功</title>
</head>
	<body>	
		<br /> <br />
		<table width="70%" border="0" align="center" cellpadding="5" cellspacing="0" style="border:solid 1px #107929">
			<tr>
		  		<th align="center" height="30" colspan="5" bgcolor="#6BBE18">
					订单取消成功
				</th>
		  	</tr>

			<tr>
				<td width="25%" align="left">&nbsp;取消结果</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r1_Code%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r1_Code</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;商户订单号</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=pb_TrxId%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">pb_TrxId</td> 
			</tr>

		</table>
	</body>
</html>
