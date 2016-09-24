<%@ page language="java" contentType="text/html; charset=gbk" pageEncoding="gbk"%>
<%@ page import="com.yeepay.*" import="java.util.*" %>
<%! String formatString(String text) {
		return text==null ? "" : text.trim();
	}
%>
<%
	String p2_Order				= formatString(request.getParameter("p2_Order"));
	//String p3_ServiceType		= formatString(request.getParameter("p3_ServiceType"));
	String p3_ServiceType		= "2";
	
	Map<String, String> queryparams = new HashMap<String, String>();
	
	queryparams.put("p2_Order"		, 	p2_Order);
	queryparams.put("p3_ServiceType",	p3_ServiceType);
	
	Map<String, String> queryResult	= YeepayService.queryByOrder(queryparams);
	String r0_Cmd					= formatString(queryResult.get("r0_Cmd")); 
	String r1_Code              	= formatString(queryResult.get("r1_Code")); 
	String r2_TrxId             	= formatString(queryResult.get("r2_TrxId")); 
	String r3_Amt               	= formatString(queryResult.get("r3_Amt")); 
	String r4_Cur               	= formatString(queryResult.get("r4_Cur")); 
	String r5_Pid               	= formatString(queryResult.get("r5_Pid")); 
	String r6_Order             	= formatString(queryResult.get("r6_Order")); 
	String r8_MP                	= formatString(queryResult.get("r8_MP")); 
	String rw_RefundRequestID   	= formatString(queryResult.get("rw_RefundRequestID")); 
	String rx_CreateTime        	= formatString(queryResult.get("rx_CreateTime")); 
	String ry_FinshTime        	= formatString(queryResult.get("ry_FinshTime")); 
	String rz_RefundAmount      	= formatString(queryResult.get("rz_RefundAmount")); 
	String rb_PayStatus         	= formatString(queryResult.get("rb_PayStatus")); 
	String rc_RefundCount       	= formatString(queryResult.get("rc_RefundCount")); 
	String rd_RefundAmt         	= formatString(queryResult.get("rd_RefundAmt")); 
	String hmacError	        	= formatString(queryResult.get("hmacError")); 
	String errorMsg	            	= formatString(queryResult.get("errorMsg")); 
	
/* 	System.out.println("rx_CreateTime======"+rx_CreateTime);
	System.out.println("ry_FinshTime======"+ry_FinshTime);

	System.out.println("r5_Pid======"+r5_Pid);
	System.out.println("r8_MP======"+r8_MP); */
	
	if("".equals(errorMsg)) {
		if("".equals(hmacError)) {
	if(r1_Code.equals("50")) {
		out.println("订单[" + p2_Order + "]不存在！");
		return;
	}
		} else {
	out.println("Hamc Mismatchig!");
	out.println("HmacError : " + hmacError);
	return;
		}
	} else {
		out.println("Query failed !");
		out.println("errorMsg : " + errorMsg);
		return;
	}
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<title>订单查询结果</title>
</head>
	<body>	
		<br /> <br />
		<table width="70%" border="0" align="center" cellpadding="5" cellspacing="0" style="border:solid 1px #107929">
			<tr>
		  		<th align="center" height="30" colspan="5" bgcolor="#6BBE18">
					订单查询结果
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
				<td width="25%" align="left">&nbsp;商品名称</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r5_Pid%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r5_Pid</td> 
			</tr> 

			<tr>
				<td width="25%" align="left">&nbsp;商户订单号</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r6_Order%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r6_Order</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;扩展信息</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=r8_MP%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">r8_MP</td> 
			</tr>


			<tr>
				<td width="25%" align="left">&nbsp;退款请求号</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=rw_RefundRequestID%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">rw_RefundRequestID</td> 
			</tr> 

			<tr>
				<td width="25%" align="left">&nbsp;订单创建时间</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=rx_CreateTime%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">rx_CreateTime</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;订单成功时间</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=ry_FinshTime%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">ry_FinshTime</td> 
			</tr>


			<tr>
				<td width="25%" align="left">&nbsp;退款请求金额</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=rz_RefundAmount%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">rz_RefundAmount</td> 
			</tr> 

			<tr>
				<td width="25%" align="left">&nbsp;支付状态</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=rb_PayStatus%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">rb_PayStatus</td> 
			</tr>

			<tr>
				<td width="25%" align="left">&nbsp;已退款次数</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=rc_RefundCount%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">rc_RefundCount</td> 
			</tr>


			<tr>
				<td width="25%" align="left">&nbsp;已退款金额</td>
				<td width="5%"  align="center"> : </td> 
				<td width="35%" align="left"> <%=rd_RefundAmt%> </td>
				<td width="5%"  align="center"> - </td> 
				<td width="30%" align="left">rd_RefundAmt</td> 
			</tr> 

		</table>

	</body>
</html>
