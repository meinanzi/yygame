#ifndef __GameNotice_h__
#define __GameNotice_h__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <queue>

struct NotifyItem
{
	int repeat;
	std::string message;
	NotifyItem(const std::string& message, int repeat) : message(message), repeat(repeat) {}
};

class GameNotice : public HNLayer
{
protected:
	GameNotice();
	virtual ~GameNotice();
public:
	/*
	 * create a notice dialog.
	 * @param parent node.
	 * @param dialog's context.
	 * @param dialog's title. 
	 */

	static GameNotice* createGameNotice(const char* stencil_Path);
	virtual bool init(const char* stencil_Path);

	void postMessage(const std::string& message, int repeat = 1);
	void setCallBack(std::function<void ()> sure);
	void setNoticTextColor(Color4B color);
	void setTextFontSize(int fontSize);

private:
	Size _showSize;
	Size _wordSize;
	Label* _text;
	std::queue<NotifyItem> _notifyMsgs;
	bool _runing;

private:
	void start();
	void stop();
	void updateNextMessage();
	
private:
	void moveWord(float delta);

private:
	std::function<void ()> _sure;
};

#endif // __GameNotice_h__
