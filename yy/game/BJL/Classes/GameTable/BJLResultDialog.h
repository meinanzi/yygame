#ifndef _RESULT_DIALOG_
#define _RESULT_DIALOG_

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ResultDialog : public HN::HNDialogBase
{
public:
	static ResultDialog* create(Node* parent, LLONG dealerWin, LLONG myWin);
	bool init(Node* parent, LLONG dealerWin, LLONG myWin);
	
protected:
	ResultDialog();
	~ResultDialog();
	bool onTouchBegan(Touch *touch, Event *unused_event);
private:
	LLONG _dealerWin;
	LLONG _myWin;
	ui::Widget* _widget;
	void initUI();
};

#endif // !_RESULT_DIALOG_
