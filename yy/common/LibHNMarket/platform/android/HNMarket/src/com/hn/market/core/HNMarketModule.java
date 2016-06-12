package com.hn.market.core;

import com.hn.framework.HNCommonUtils;
import com.hn.market.base.Method;
import com.hn.market.base.Module;

import android.util.Log;

public class HNMarketModule extends Module {
	private static final String TAG = HNMarketModule.class.getSimpleName();

	public HNMarketModule() {
		super("MarketModule");
		Register("GetISP", new GetISP());
		Log.i(TAG, "Init");
	}

	// //////////////////////////////////////////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////////////////////////////////////////

	public class GetISP implements Method {

		@Override
		public String Execute(final String args, String callBack) {
			return HNCommonUtils.getISP(activity);
		}
	}

}
