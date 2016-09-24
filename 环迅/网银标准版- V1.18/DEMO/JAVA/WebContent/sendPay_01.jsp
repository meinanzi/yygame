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
	String pa_Ext         	= formatString(request.getParameter("pa_Ext"));
	String pb_Oper        	= formatString(request.getParameter("pb_Oper"));
	String pd_FrpId         = formatString(request.getParameter("pd_FrpId"));
	String pd_BankBranch  	= formatString(request.getParameter("pd_BankBranch"));
	String pt_ActId  		= formatString(request.getParameter("pt_ActId"));
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
	params.put("p9_SAF",	p9_SAF);
	params.put("pa_MP",		pa_MP);
	params.put("pd_FrpId",	pd_FrpId);
	params.put("pm_Period",	pm_Period);
	params.put("pn_Unit",	pn_Unit);
	params.put("pr_NeedResponse",pr_NeedResponse);

	out.println("params : " + params);

	String payURL		= YeepayService.getPayURL(params);

	if("".equals(payURL)) {
		out.println("Éú³ÉÁ´½ÓÊ§°Ü£¡");
	} else {
		response.sendRedirect(payURL);
	}
%>
