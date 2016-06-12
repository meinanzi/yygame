#ifndef __BJLNotice_h__
#define __BJLNotice_h__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace BJL
{
	class GameNotice : public HNLayer
	{
	protected:
		GameNotice();
		~GameNotice();
	public:
		/*
		 * create a notice dialog.
		 * @param parent node.
		 * @param dialog's context.
		 * @param dialog's title. 
		 */
		static GameNotice* create(cocos2d::Node* parent,  std::string context, cocos2d::Node* shelter = nullptr , std::string title="", std::string fileName = "");
		bool init(cocos2d::Node* parent,  std::string& context, cocos2d::Node* shelter , std::string& title, std::string& fileName);
		void show();
	private:
		bool loadUI(std::string& context, std::string& title, std::string& fileName);
		//ui::ImageView* _bground;
		//ui::Widget* _widget;
		HNSprite* _bg;

		cocos2d::Node* _shelter;
		cocos2d::Node* _parent;
	};
}


#endif // __BJLNotice_h__
