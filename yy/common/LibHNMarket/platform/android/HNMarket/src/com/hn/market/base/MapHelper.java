package com.hn.market.base;


import java.util.HashMap;
import java.util.Map;
import java.util.Set;

public class MapHelper<KEY, T> {
	protected Map<KEY, T> mMap = null;
	
	public MapHelper() {
		mMap = new HashMap<KEY, T>();
	}
	
	public boolean Register(KEY key, T object) {
		if (key == null || key.equals("")) {
			throw new NullPointerException();
		}
		
		if (object == null) {
			throw new NullPointerException();
		}
		
		mMap.put(key, object);
		
		return true;
	}
	
	public T Find(KEY key) {
		if (key == null) {
			throw new NullPointerException();
		}
		KEY k = null;
		if (k instanceof String) {
			if ("".equals(k)) {
				throw new NullPointerException();
			}
		}
		T object = null;
		object = mMap.get(key);
		return object;
	}
	
	public Set<KEY> keySet()
	{
		return (Set<KEY>) mMap.keySet();
	}
}
