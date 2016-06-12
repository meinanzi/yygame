#ifndef _GoldenFlowerGAME_NOTICE_
#define _GoldenFlowerGAME_NOTICE_

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace goldenflower
{
	class GameNotice : public  HN::HNLayer
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



#endif // !_NOTICE_DIALOG_
