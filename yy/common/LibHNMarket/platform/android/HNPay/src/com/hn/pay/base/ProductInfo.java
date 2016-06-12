package com.hn.pay.base;

import org.json.JSONException;
import org.json.JSONObject;


public class ProductInfo {
    // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	public long				number = 0;
    // ï¿½ï¿½Æ·ï¿½Û¸ï¿½
	public double			price = 0;
    // ï¿½ï¿½Æ·ID
	public String           identifier = null;
	// Ö§ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ sdk
	public int				payType = -1;
	
	public String           xmlFile = null;
	
	// ¶©µ¥ºÅ
    public String           orderID = null;
	
	public ProductInfo()
    {    
    }
	
    public ProductInfo(long number, double price, String identifier, int payType, String orderID)
    {
        // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
    	this.number = number;
        // ï¿½ï¿½Æ·ï¿½Û¸ï¿½
    	this.price = price;
        // ï¿½ï¿½Æ·ID
    	this.identifier = identifier;       
    	// Ö§ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ sdk
    	this.payType = payType;
    	// ¶©µ¥ºÅ
    	this.orderID = orderID;
    }

    public String serialize()
    {
    	return "";
    }
    
    public void deserialize(String data)
    {
    	try {
			JSONObject json = new JSONObject(data);
			this.number = json.getInt("number");
			this.price = json.getDouble("price");
			this.identifier = json.getString("identifier");
			this.payType = json.getInt("payType");
			this.xmlFile = json.getString("xmlFile");
			this.orderID = json.getString("orderID");
		} catch (JSONException e) {
			e.printStackTrace();
		}
    }
}
