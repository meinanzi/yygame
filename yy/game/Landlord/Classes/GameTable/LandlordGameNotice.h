#ifndef _Landlord_GAME_NOTICE_
#define _Landlord_GAME_NOTICE_

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace landlord
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
	void moveWord(float delta);
private:
	HNSprite* _bg;
	cocos2d::Node* _parent;
	Size _showSize;
	Size _wordSize;
	Label* _text;
	void start();
	void stop();
};
}


#endif // !_NOTICE_DIALOG_
