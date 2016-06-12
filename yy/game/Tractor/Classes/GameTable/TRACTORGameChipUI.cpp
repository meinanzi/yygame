#include "TRACTORGameChipUI.h"

using namespace HN;
using namespace ui;

namespace TRACTOR
{
	GameChipUI* GameChipUI::create(const std::string& filename)
	{
		GameChipUI* chip = new GameChipUI();
		if(chip && chip->initWithFile(filename))
		{
			chip->autorelease();
			return chip;
		}
		else
		{
			CC_SAFE_DELETE(chip);
			return nullptr;
		}
	}

	void GameChipUI::setValue(LLONG value)
	{
		if(value != _value)
		{
			_value = value;
			char tmp[50] = {0};
			sprintf(tmp,"%lld",_value);
			_text->setString(tmp);
		}

		//_text->setVisible(_value > 0);
	}

	LLONG GameChipUI::getValue() const
	{
		return _value;
	}

	bool GameChipUI::initWithFile(const std::string& filename)
	{
		if(!HNSprite::initWithFile(filename))
		{
			return false;
		}

		Size size = this->getContentSize();

		_text = ui::Text::create("0","Arial",25);
		_text->setColor(Color3B(255,255,0));
		_text->setAnchorPoint(Vec2(0.5f, 1.0f));
		_text->setPosition(Vec2(size.width / 2, 5));
		_text->setVisible(false);
		addChild(_text);

		return true;
	}

	GameChipUI::GameChipUI():_value(0),_text(nullptr)
	{

	}

	GameChipUI::~GameChipUI()
	{

	}
}
