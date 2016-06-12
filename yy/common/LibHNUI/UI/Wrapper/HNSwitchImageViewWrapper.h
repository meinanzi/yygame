#ifndef __HN_SwitchImageViewWrapper_H__
#define __HN_SwitchImageViewWrapper_H__

#include "cocos2d.h"
#include "HNWrapper.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include <vector>

USING_NS_CC;

namespace HN
{
	class SwitchImageViewWrapper : public HN::HNWrapper
	{
	public:
		typedef std::function<void(SwitchImageViewWrapper* pSender)> ccSwitchImageViewClickCallback;

	private:
		int _group;
		bool _isSelected;
		std::string _normalImage;
		std::string _selectedImage;
		ccSwitchImageViewClickCallback _callback;

		static std::vector<SwitchImageViewWrapper*> _groupController;

	public:
		SwitchImageViewWrapper();

		SwitchImageViewWrapper(const std::string& normalImage, const std::string& selectedImage);

		virtual ~SwitchImageViewWrapper();

	public:
		void setGroup(int group) { _group = group; }
		
		int getGroup() const { return _group; }

		void setSelected(bool selected);

		void setImage(const std::string& normalImage, const std::string& selectedImage);

		bool isSelected() const;

		void addClickEventListener(const ccSwitchImageViewClickCallback& callback);

	protected:
		virtual bool load() override;

		virtual void unload() override;

		virtual void restore() override;
	};

}



#endif // !__HN_SwitchImageViewWrapper_H__
