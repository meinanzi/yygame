#ifndef __DZPokerGameNotice_h__
#define __DZPokerGameNotice_h__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "HNLobbyExport.h"
USING_NS_CC;
using namespace cocostudio;
using namespace ui;

namespace DZPoker
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
		static GameNotice* create(cocos2d::Node* parent,  std::string context, std::string title="", std::string fileName = "");
		bool init(cocos2d::Node* parent,  std::string& context, std::string& title, std::string& fileName);
		void show();
	private:
		bool loadUI(std::string& context, std::string& title, std::string& fileName);
		//ui::ImageView* _bground;
		//ui::Widget* _widget;
		HNSprite* _bg;

		cocos2d::Node* _parent;
	};
}


#endif // __DZPokerGameNotice_h__
