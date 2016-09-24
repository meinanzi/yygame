<%@ page language="java" contentType="text/html; charset=gbk"%>
<%@ page import="com.yeepay.*" import="java.util.Map, java.util.HashMap"%>
<%! String formatString(String text) {
		return text == null ? "" : text.trim();
	}
%>
<%
	request.setCharacterEncoding("GBK");

	String p0_Cmd           = formatString(request.getParameter("p0_Cmd"));
	String p2_Order         = formatString(request.getParameter("p2_Order"));
	String p3_Amt           = formatString(request.getParameter("p3_Amt"));
	String p4_Cur           = formatString(request.getParameter("p4_Cur"));
	String p5_Pid           = formatString(request.getParameter("p5_Pid"));
	String p6_Pcat          = formatString(request.getParameter("p6_Pcat"));
	String p7_Pdesc         = formatString(request.getParameter("p7_Pdesc"));
	String p8_Url           = formatString(request.getParameter("p8_Url"));
	String p9_SAF           = formatString(request.getParameter("p9_SAF"));
	String pa_MP            = formatString(request.getParameter("pa_MP"));
	String pd_FrpId         = formatString(request.getParameter("pd_FrpId"));
	String pm_Period        = formatString(request.getParameter("pm_Period"));
	String pn_Unit         	= formatString(request.getParameter("pn_Unit"));
	String pr_NeedResponse  = formatString(request.getParameter("pr_NeedResponse"));

	String p1_MerId			= Configuration.getInstance().getValue("p1_MerId");
	String keyValue			= Configuration.getInstance().getValue("keyValue");

	String[] strArr			= {p0_Cmd, p1_MerId, p2_Order, p3_Amt, p4_Cur, p5_Pid, p6_Pcat, 
								p7_Pdesc, p8_Url, p9_SAF, pa_MP, pd_FrpId, pm_Period, pn_Unit, pr_NeedResponse};

	String hmac				= DigestUtil.getHmac(strArr, keyValue);

	String requestURL		= Configuration.getInstance().getValue("requestURL");

%>
<html>
	<head>
		<title>To YeePay Page
		</title>
	</head>
	<body>
		<form name="yeepay" action='<%=requestURL%>' method='POST'>
			<input type='hidden' name='p0_Cmd'   		value='<%=p0_Cmd%>'>
			<input type='hidden' name='p1_MerId' 		value='<%=p1_MerId%>'>
			<input type='hidden' name='p2_Order' 		value='<%=p2_Order%>'>
			<input type='hidden' name='p3_Amt'   		value='<%=p3_Amt%>'>
			<input type='hidden' name='p4_Cur'   		value='<%=p4_Cur%>'>
			<input type='hidden' name='p5_Pid'   		value='<%=p5_Pid%>'>
			<input type='hidden' name='p6_Pcat'  		value='<%=p6_Pcat%>'>
			<input type='hidden' name='p7_Pdesc' 		value='<%=p7_Pdesc%>'>
			<input type='hidden' name='p8_Url'   		value='<%=p8_Url%>'>
			<input type='hidden' name='p9_SAF'   		value='<%=p9_SAF%>'>
			<input type='hidden' name='pa_MP'    		value='<%=pa_MP%>'>
			<input type='hidden' name='pd_FrpId' 		value='<%=pd_FrpId%>'>
			<input type='hidden' name='pm_Period' 		value='<%=pm_Period%>'>
			<input type='hidden' name='pn_Unit' 		value='<%=pn_Unit%>'>
			<input type="hidden" name="pr_NeedResponse" value="<%=pr_NeedResponse%>">
			<input type='hidden' name='hmac'     		value='<%=hmac%>'>
		</form>
		 <script language="javascript">
 			document.yeepay.submit();
		</script>
	</body>
</html>
