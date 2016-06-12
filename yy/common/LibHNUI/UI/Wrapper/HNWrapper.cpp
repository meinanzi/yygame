#include "HNWrapper.h"

namespace HN
{
	bool HNWrapper::doLoad(Node* component)
	{
		CCAssert(nullptr != component, "nullptr == _component");
		if (nullptr == component) return false;

		_component = component;

		load();

		return true;
	}

	void HNWrapper::doUnload()
	{
		unload();
		_component = nullptr;
	}

	void HNWrapper::doRestore()
	{
		restore();
	}

	void HNWrapper::setVisible(bool visible)
	{
		if (_visible != visible)
		{
			_visible = visible;
			_component->setVisible(_visible);
		}
	}

	bool HNWrapper::isVisible() const
	{
		return _visible;
	}

	int HNWrapper::getLocalZOrder() const
	{
		return _component->getLocalZOrder();
	}

	const Size& HNWrapper::getContentSize() const
	{
		return _component->getContentSize();
	}

	void HNWrapper::setContentSize(const Size& contentSize)
	{
		_component->setContentSize(contentSize);
	}

	const Vec2& HNWrapper::getPosition() const
	{
		return _component->getPosition();
	}

	void HNWrapper::getPosition(float* x, float* y) const
	{
		_component->getPosition(x, y);
	}

	void HNWrapper::setPosition(float x, float y)
	{
		_component->setPosition(x, y);
	}
		
	void HNWrapper::setPosition(const Vec2 &position)
	{
		_component->setPosition(position);
	}

	void HNWrapper::setTag(int tag)
	{
		_component->setTag(tag);
	}
	
	int HNWrapper::getTag() const
	{
		return _component->getTag();
	}

	void HNWrapper::setName(const std::string& name)
	{
		_component->setName(name);
	}

	std::string HNWrapper::getName() const
	{
		return _component->getName();
	}

	void HNWrapper::addChild(Node * child)
	{
		_component->addChild(child);
	}

	void HNWrapper::addChild(Node * child, int localZOrder)
	{
		_component->addChild(child, localZOrder);
	}

	void HNWrapper::addChild(Node* child, int localZOrder, int tag)
	{
		_component->addChild(child, localZOrder, tag);
	}

	void HNWrapper::addChild(Node* child, int localZOrder, const std::string &name)
	{
		_component->addChild(child, localZOrder, name);
	}

	void HNWrapper::removeChild(Node* child, bool cleanup)
	{
		_component->removeChild(child, cleanup);
	}

	void HNWrapper::removeChild(int tag, bool cleanup)
	{
		_component->removeChildByTag(tag, cleanup);
	}

	void HNWrapper::removeChild(const std::string &name, bool cleanup)
	{
		_component->removeChildByName(name, cleanup);
	}
}
