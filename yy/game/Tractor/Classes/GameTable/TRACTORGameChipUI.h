#ifndef __TRACTORGameChipUI_h__
#define __TRACTORGameChipUI_h__


#include "HNUIExport.h"
#include "HNNetExport.h"
#include "ui/CocosGUI.h"

namespace TRACTOR
{
	class GameChipUI: public HNSprite
	{
	public:
		static GameChipUI* create(const std::string& filename);

	public:
		virtual bool initWithFile(const std::string& filename);


	public:
		void setValue(LLONG value);
		LLONG getValue() const;

	private:
		GameChipUI();
		virtual ~GameChipUI();

		LLONG       _value;
		ui::Text*   _text;
	};

}

#endif // __TRACTORGameChipUI_h__