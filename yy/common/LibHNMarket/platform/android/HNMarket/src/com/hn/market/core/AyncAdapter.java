package com.hn.market.core;


import com.hn.market.base.Method;
import com.hn.market.base.Module;
import com.hn.market.export.ChannelExport;

import android.util.Log;

public class AyncAdapter extends Module {
	private static final String TAG = AyncAdapter.class.getSimpleName();
	
	public AyncAdapter() {
		super("AyncModule");
		Register("downnetdata", new DownNetData());
		Log.i(TAG, "Init");
	}

	@Override
	public String Execute(String function, String args, String callBack) {
		Method method = FindMethod(function);
		if (method != null) {
			AyncTaskManager.SharedAyncTaskManager().addTask(new AyncTask(method, args, callBack));
		}
		return "";
	}

	// //////////////////////////////////////////////////////////////////////////////////////////////

	class DownNetData implements Method {
		@Override
		public String Execute(String args, String callBack) {
			ChannelExport.getInstance().executeAyncMethod(callBack,
					"callback testing......");
			return "";
		}
	}
}
