#ifndef Products_h__
#define Products_h__

#include "HNLobbyExport.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
static PRODUCT_INFO prodecuInfos[] = 
{
	PRODUCT_INFO(60000, 6.0, "com.redbird.Landlord.60000"),
	PRODUCT_INFO(130000, 12.0, "com.redbird.Landlord.130000"),
	PRODUCT_INFO(3010000, 50.0, "com.redbird.Landlord.3010000")
};
#else
static PRODUCT_INFO prodecuInfos[] = 
{
	PRODUCT_INFO(60000, 6.0, "com.redbird.Landlord.60000"),
	PRODUCT_INFO(130000, 12.0, "com.redbird.Landlord.130000"),
	PRODUCT_INFO(3010000, 50.0, "com.redbird.Landlord.3010000")
	//PRODUCT_INFO(60000,   6.0,  "com.redbird.RunBeard.60000"),
	//PRODUCT_INFO(150000,  12.0, "com.redbird.RunBeard.150000"),
	//PRODUCT_INFO(270000,  18.0, "com.redbird.RunBeard.270000"),
	//PRODUCT_INFO(1500000, 50.0, "com.redbird.RunBeard.1500000"),
	//PRODUCT_INFO(3000000,  80.0, "com.redbird.RunBeard.3000000"),
	//PRODUCT_INFO(5000000, 100.0, "com.redbird.RunBeard.5000000"),
	//PRODUCT_INFO(10000000,  120.0, "com.redbird.RunBeard.10000000"),
	//PRODUCT_INFO(15000000, 150.0, "com.redbird.RunBeard.15000000")
};
#endif

#endif // Products_h__
