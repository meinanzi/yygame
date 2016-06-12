package com.hn.market.core;

import org.json.JSONException;
import org.json.JSONObject;

import com.hn.framework.HNCommonUtils;
import com.hn.market.base.Method;
import com.hn.market.base.Module;
import android.util.Log;

public class HNSysModule extends Module {
	private static final String TAG = HNSysModule.class.getSimpleName();
	
	private static HNSysModule sysModule = null;

	public static HNSysModule getInstance() {
		if (sysModule == null) {
			synchronized (HNSysModule.class) {
				if (sysModule == null) {
					sysModule = new HNSysModule();
				}
			}
		}
		return sysModule;
	}

	public HNSysModule() {
		super("sysmodule");
		Register("GetIMSI", new GetIMSI());
		Register("GetIMEI", new GetIMEI());
		Register("GetMNO", new GetMNO());
		Register("GetVersion", new GetVersion());
		Register("GetSerialNumber", new GetSerialNumber());
		Register("DealPhone", new DealPhone());
		Register("DealMsg", new DealMsg());
		Register("SendEmail", new SendEmail());
		Register("installApp", new InstallApp());
		Register("GetNetIp", new GetNetIp());
		Log.d(TAG, "init systmModule");
	}
	
	public class GetSerialNumber implements Method {
		@Override
		public String Execute(final String args, String callBack) {
			return HNCommonUtils.getSerialNumber(activity);
		}
	}

	public class GetVersion implements Method {

		@Override
		public String Execute(final String args, String callBack) {
			return HNCommonUtils.getGameVersion(activity);
		}
	}

	public class GetIMSI implements Method {

		@Override
		public String Execute(final String args, String callBack) {
			return HNCommonUtils.getIMSI(activity);
		}
	}

	public class GetIMEI implements Method {

		@Override
		public String Execute(final String args, String callBack) {
			return HNCommonUtils.getIMEI(activity);
		}
	}

	public class GetMNO implements Method {

		@Override
		public String Execute(final String args, String callBack) {
			return HNCommonUtils.getISP(activity);
		}
	}
	
	public class DealPhone implements Method {

		@Override
		public String Execute(final String args, String callBack) {
			JSONObject json;
			try {
				json = new JSONObject(args);
				String number = json.getString("number");
				HNCommonUtils.dealPhone(activity, number);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			return "1";
		}
	}
	
	public class DealMsg implements Method {

		@Override
		public String Execute(final String args, String callBack) {
			
			JSONObject json;
			try {
				json = new JSONObject(args);
				String number = json.getString("number");
				HNCommonUtils.dealMsg(activity, number);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			return "1";
		}
	}
	
	public class SendEmail implements Method {
		
		@Override
		public String Execute(final String args, String callBack) {
			
			JSONObject json;
			try {
				json = new JSONObject(args);
				String to = json.getString("to");
				String text = json.getString("text");
				HNCommonUtils.sendEmail(activity, to, text);
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	
			return "1";
		}
	}
	
	public class InstallApp implements Method{

		@Override
		public String Execute(String args, String callBack) {
			// TODO Auto-generated method stub
			HNCommonUtils.installApp(activity, args);
			return "";
		}
		
	}
	
	public class GetNetIp implements Method {
		public String Execute(final String args, String callBack) {
			return HNCommonUtils.getNetIp(activity);
		}
	}
}
