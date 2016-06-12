//Copyright (c) 2012¡ª2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved


#ifndef __SpreadEditLayer_h__
#define __SpreadEditLayer_h__


#include "HNLogicExport.h"
#include "HNUIExport.h"


namespace HN
{
	class SpreadEditLayer : public HNLayer, public HNSpreadLogic::Delegate, public ui::EditBoxDelegate
	{
	public:
		class Delegate
		{
		public:
			virtual ~Delegate(){}
		public:
			virtual void onSpreadInfoChanged() = 0;
		};

	public:
		static SpreadEditLayer* create(Delegate* dlte = nullptr);

	public:
		SpreadEditLayer(Delegate* dlte);
		virtual ~SpreadEditLayer();

	private:
		struct HN_STUDIO_UI
		{
			Node*				root;
			Layout*				layout;
			Text*				tips;
			HNEditBox*		editName;
			HNEditBox*		editPhone;
			HNEditBox*		editAddress;
			HNEditBox*		editPersonNum;
			HNEditBox*		editBankName;
			HNEditBox*		editBankNum;
			Button*				scSFZZM_Btn;
			Button*				scSFZFM_Btn;
			Sprite*				picSFZZM;
			Sprite*				picSFZFM;
			Text*				scSFZZM_Tips;
			Text*				scSFZFM_Tips;

			HN_STUDIO_UI()
			{
				root = nullptr;
				layout = nullptr;
				tips = nullptr;
				editName = nullptr;
				editPhone = nullptr;
				editAddress = nullptr;
				editPersonNum = nullptr;
				editBankName = nullptr;
				editBankNum = nullptr;
				scSFZZM_Btn = nullptr;
				scSFZFM_Btn = nullptr;
				picSFZZM = nullptr;
				picSFZFM = nullptr;
				scSFZZM_Tips = nullptr;
				scSFZFM_Tips = nullptr;
			}	
		}_studioUI;

		HNSpreadLogic _spreadLogic;
		Delegate* _delegate;
		std::string _cardInfoMain; 
		std::string _cardInfoOther;

		HNSprite* _titleSprite_Info;
		HNSprite* _titleSprite_Apply;

	public:
		virtual bool init() override;

	public:
		void show(Node* parent, int orderZ, int tagV);
		void close();
		void closeEventCallBack(Ref* pSender, Widget::TouchEventType type);

	private:
		void showBase64Pic(std::string code, Sprite* sprite);
		void getSFZZM_CB(std::string code);
		void getSFZFM_CB(std::string code);
		void btnCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);
    
	public:
		virtual void onBeSpreader(bool result, const std::string& msg) override;
		virtual void onSpreadInfo(bool result, bool bInfo, const std::string& msg) override;
	public:
		virtual void editBoxReturn(ui::EditBox* editBox) {};
	};

}
#endif // __SpreadEditLayer_h__

