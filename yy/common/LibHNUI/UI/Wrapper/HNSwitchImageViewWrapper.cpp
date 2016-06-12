#include "HNSwitchImageViewWrapper.h"

namespace HN
{
	std::vector<SwitchImageViewWrapper*> SwitchImageViewWrapper::_groupController;

	SwitchImageViewWrapper::SwitchImageViewWrapper()
		: _isSelected(false)
		, _group(0)
		, _callback(nullptr)
	{
	}

	SwitchImageViewWrapper::SwitchImageViewWrapper(const std::string& normalImage, const std::string& selectedImage)
		: _normalImage(normalImage)
		, _selectedImage(selectedImage)
		, _isSelected(false)
		, _group(0)
		, _callback(nullptr)
	{
	}

	SwitchImageViewWrapper::~SwitchImageViewWrapper()
	{
		unload();
	}

	void SwitchImageViewWrapper::setSelected(bool selected)
	{
		if (_isSelected != selected)
		{
			_isSelected = selected;

			ImageView* pComponent = this->getComponent<ui::ImageView>();
			if (_isSelected)
			{
				pComponent->loadTexture(_selectedImage, ui::Widget::TextureResType::PLIST);
			}
			else
			{
				pComponent->loadTexture(_normalImage, ui::Widget::TextureResType::PLIST);
			}
		}
	}

	void SwitchImageViewWrapper::setImage(const std::string& normalImage, const std::string& selectedImage)
	{
		if (_normalImage.compare(normalImage) != 0)
		{
			_normalImage = normalImage;
		}

		if (_selectedImage.compare(selectedImage) != 0)
		{
			_selectedImage = selectedImage;
		}
	}

	bool SwitchImageViewWrapper::isSelected() const
	{
		return _isSelected;
	}

	void SwitchImageViewWrapper::addClickEventListener(const ccSwitchImageViewClickCallback& callback)
	{
		_callback = callback;
	}

	bool SwitchImageViewWrapper::load()
	{
		ImageView* pComponent = this->getComponent<ui::ImageView>();
		pComponent->setTouchEnabled(true);
		pComponent->addClickEventListener([this](Ref* pSender)
		{
			auto pImageView = (ui::ImageView*)pSender;
			if (!isSelected())
			{
				setSelected(true);

				for (auto control : _groupController)
				{
					if (control == this) continue;
					if (control->isSelected())
					{
						control->setSelected(false);
					}
				}

				if (nullptr != _callback) _callback(this);
			}
		});

		_groupController.push_back(this);

		return true;
	}

	void SwitchImageViewWrapper::unload()
	{
		auto iter = std::find(_groupController.begin(), _groupController.end(), this);
		if (iter != _groupController.end())
		{
			_groupController.erase(iter);
		}
	}

	void SwitchImageViewWrapper::restore()
	{
	}

}