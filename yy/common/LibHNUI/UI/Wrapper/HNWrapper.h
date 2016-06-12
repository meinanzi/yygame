#ifndef __HN_Wrapper_H__
#define __HN_Wrapper_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

USING_NS_CC;
using namespace ui;

namespace HN
{
	class HNHelperWrapper final
	{
		Node* _component;

	public:
		HNHelperWrapper() : _component(nullptr) {}
		HNHelperWrapper(Node* component) : _component(component) {}
		~HNHelperWrapper() {}

	public:
		void load(Node* component) { _component = component; };

	public:
		template<class T>
		T* find(const std::string& name)
		{
			return dynamic_cast<T*>(_component->getChildByName(name));
		}

		template<class T>
		T* find(int tag)
		{
			return dynamic_cast<T*>(_component->getChildByTag(tag));
		}
	};
	
	class HNWrapper
	{
	public:
		enum class ALIGNMENT //: BYTE
		{
			NONE,
			TOP,
			LEFT,
			RIGHT,
			BOTTOM
		};

	private:
		bool _visible;

	private:
		Node* _component;

	public:
		HNWrapper() : _component(nullptr), _visible(true) {}
		virtual ~HNWrapper(){}

	public:
		bool doLoad(Node* component);
		void doUnload();
		void doRestore();

		virtual void setVisible(bool visible);
		virtual bool isVisible() const;

		int getLocalZOrder() const;

		const Size& getContentSize() const;
		void setContentSize(const Size& contentSize);

		void setPosition(float x, float y);
		void setPosition(const Vec2 &position);

		const Vec2& getPosition() const;
		void getPosition(float* x, float* y) const;

		void setTag(int tag);
		int getTag() const;

		void setName(const std::string& name);
		std::string getName() const;

	public:
		void addChild(Node * child);
		void addChild(Node * child, int localZOrder);
		void addChild(Node* child, int localZOrder, int tag);
		void addChild(Node* child, int localZOrder, const std::string &name);

		void removeChild(Node* child, bool cleanup = true);
		void removeChild(int tag, bool cleanup = true);
		void removeChild(const std::string &name, bool cleanup = true);

	public:
		template<class T>
		T* getComponent()
		{
			return dynamic_cast<T*>(_component);
		}

		Node* getComponent()
		{
			return dynamic_cast<Node*>(_component);
		}

		template<class T>
		T* find(const std::string& name)
		{
			return dynamic_cast<T*>(_component->getChildByName(name));
		}

		template<class T>
		T* find(int tag)
		{
			return dynamic_cast<T*>(_component->getChildByTag(tag));
		}

	protected:
		virtual bool load() = 0;
		virtual void unload() = 0;
		virtual void restore() = 0;
	};
}

#endif // !__HN_Wrapper_H__
