//Copyright (c) 2012¡ª2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

#ifndef __SpreadManager_h__
#define __SpreadManager_h__


#include <string>


namespace HN
{
	enum SPREADER_STATUS
	{
		SPREADER_STATUS_NO = -1,
		SPREADER_STATUS_ING,
		SPREADER_STATUS_SUCCESS,
		SPREADER_STATUS_FAILED
	};

	typedef struct UserSpreadInfo
	{
		int userId; 
		long long spreadIncom; 
		std::string reallName; 
		std::string personNum; 
		std::string phoneNum; 
		std::string address; 
		std::string bankName; 
		std::string bankNum; 
		std::string applyTime; 
		std::string spreadUrl; 
		std::string IDCardImgUrlMain;
		std::string IDCardImgUrlOther;
		std::string failedInfo;
		SPREADER_STATUS applyStatus;

		UserSpreadInfo()
		{
			userId = 0;
			spreadIncom = 0;
			reallName = "";					
			reallName = "";						
			personNum = "";						
			phoneNum = "";						
			address = "";						
			bankName = "";						
			bankNum = "";						
			applyTime = "";						
			spreadUrl = "";						
			IDCardImgUrlMain = "";
			IDCardImgUrlOther = "";
			failedInfo = "";
			applyStatus = SPREADER_STATUS_NO;
		}
	}SpreaderInfo;


	class HNSpreadManager
	{
	private:
		static HNSpreadManager* _spreadManager;

	public:
		static HNSpreadManager* getInstance();

	private:
		HNSpreadManager();

	public:
		virtual ~HNSpreadManager();

	public:
		bool isSpreader(){return _spreadInfo.applyStatus == SPREADER_STATUS_SUCCESS;}
		void setSpreadInfo(const SpreaderInfo& info){_spreadInfo = info;}
		void setName(const std::string& name){_spreadInfo.reallName = name;}
		void setPhone(const std::string& phone){_spreadInfo.phoneNum = phone;}
		void setAddress(const std::string& address){_spreadInfo.address = address;}
		SpreaderInfo getSpreadInfo(){return _spreadInfo;}

	private:
		SpreaderInfo _spreadInfo;
	};

}

#endif // __SpreadManager_h__

