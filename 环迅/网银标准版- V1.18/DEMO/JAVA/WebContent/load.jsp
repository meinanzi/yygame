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
	String pa_MP            = formatString(request.getParameter("pa_MP"));
	String pa_Ext           = formatString(request.getParameter("pa_Ext"));
	String pb_Oper          = formatString(request.getParameter("pb_Oper"));
	String pd_FrpId         = formatString(request.getParameter("pd_FrpId"));
	String pd_BankBranch    = formatString(request.getParameter("pd_BankBranch"));
	String pt_ActId         = formatString(request.getParameter("pt_ActId"));
	String pv_Ver  			= formatString(request.getParameter("pv_Ver"));

	Map<String, String> params = new HashMap<String, String>();
	params.put("p0_Cmd", 	p0_Cmd);
	params.put("p2_Order",	p2_Order);
	params.put("p3_Amt",	p3_Amt);
	params.put("p4_Cur",	p4_Cur);
	params.put("p5_Pid",	p5_Pid);
	params.put("p6_Pcat",	p6_Pcat);
	params.put("p7_Pdesc",	p7_Pdesc);
	params.put("p8_Url",	p8_Url);
	params.put("pa_MP",		pa_MP);
	params.put("pa_Ext",	pa_Ext);
	params.put("pb_Oper",	pb_Oper);
	params.put("pd_FrpId",	pd_FrpId);
	params.put("pd_BankBranch",	pd_BankBranch);
	params.put("pt_ActId",	pt_ActId);
	params.put("pv_Ver",	pv_Ver);

	out.println("params : " + params);

	String payURL		= YeepayService.getLoadURL(params);

	if("".equals(payURL)) {
		out.println("Éú³ÉÁ´½ÓÊ§°Ü£¡");
	} else {
		response.sendRedirect(payURL);
	}
%>
