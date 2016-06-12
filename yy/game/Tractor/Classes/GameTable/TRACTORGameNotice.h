#ifndef __TRACTORGameNotice_h__
#define __TRACTORGameNotice_h__


#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
namespace TRACTOR
{
	class GameNotice : public  HNLayer
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
		static GameNotice* create(cocos2d::Node* parent, std::string noticeMsg);
		bool init(cocos2d::Node* parent, std::string& noticeMsg);
		void show();
	private:
		HNSprite* _bg;
		cocos2d::Node* _parent;
	};
}

#endif // __TRACTORGameNotice_h__