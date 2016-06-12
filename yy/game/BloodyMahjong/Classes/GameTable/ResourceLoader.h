#ifndef _RESOURCELOADER_H_
#define _RESOURCELOADER_H_


#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "XZMJ_MessageHead.h"

namespace XZMJ
{

#define CREATE_FUNC_STRING(__className) \
public: \
	static __className* create(const std::string& name)	\
	{ \
	auto p = new __className;	 \
	if (p && p->init(name)) \
	{ \
	p->autorelease(); \
	return p; \
} \
	delete p; \
	return nullptr; \
} \
private: \
	virtual bool init(std::string name) \
	{ \
	if (!ResourceLoader::init()) \
	{ \
	return false; \
} \
	loadFile(name); \
	return true; \
} \
	virtual void loadFile(std::string name) override; 

	class ResourceLoader :
		public cocos2d::Node
	{
	public:
		ResourceLoader(void);
		~ResourceLoader(void);

		cocos2d::Node* getNode(const std::string& name);          // 获取节点
		static void removeNode(const std::string& name);					 // 删除节点
		static bool addNode(Node* p, std::string name);						 // 增加节点
		static void clearAll();                                                                  // 清除所有节点
		virtual void removeNode(cocos2d::Node *p);
		virtual void clear();

	private:
		virtual void loadFile(std::string name) = 0;
		virtual cocos2d::Node* makeA_Node(const std::string& name) { return nullptr; }
		
	protected:
		cocos2d::Node* _root;      // 所有可用资源的根节点
		static std::map<std::string, cocos2d::Node*> _mapNode;
		static std::list<cocos2d::Node *> _onlyFree;
		std::list<cocos2d::Node *> _currNodeList;

	};


	class cocosResourceLoader : public ResourceLoader
	{
	public:
		CREATE_FUNC_STRING(cocosResourceLoader);
	};

	class spriteResourceLoader : public ResourceLoader
	{
	public:
		CREATE_FUNC_STRING(spriteResourceLoader);
	};

	class plistResourceLoader : public ResourceLoader
	{
	public:
		CREATE_FUNC_STRING(plistResourceLoader);

	public:
		virtual cocos2d::Node* makeA_Node(const std::string& name) override;
	};
}

#endif