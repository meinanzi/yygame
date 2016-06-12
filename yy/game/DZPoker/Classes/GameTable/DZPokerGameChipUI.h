#ifndef __DZPokerGameChipUI_h__
#define __DZPokerGameChipUI_h__

#include "HNNetExport.h"
#include "ui/CocosGUI.h"
using namespace ui;

namespace DZPoker
{

	class GameChipUI: public HNSprite
	{
	public:
		static GameChipUI* create(const std::string& filename);
		void setValue(LLONG value);
		LLONG getValue();
	public:
		virtual bool initWithFile(const std::string& filename);
	private:
		GameChipUI();
		~GameChipUI();
		LLONG _value;
		ui::Text* _text;
	};

}

#endif // __DZPokerGameChipUI_h__