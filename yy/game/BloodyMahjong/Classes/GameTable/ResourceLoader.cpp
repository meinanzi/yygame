#include "ResourceLoader.h"
#include "tinyxml2/tinyxml2.h"

namespace XZMJ
{

	using namespace cocos2d;
	using namespace cocostudio;

	std::map<std::string, cocos2d::Node*> ResourceLoader::_mapNode = std::map<std::string, cocos2d::Node*>();
	std::list<cocos2d::Node *> ResourceLoader::_onlyFree = std::list<cocos2d::Node *>();
	ResourceLoader::ResourceLoader(void) 
		: _root(nullptr)
	{
	}


	ResourceLoader::~ResourceLoader(void)
	{
	}

	Node* ResourceLoader::getNode(const string& name)
	{
		if (_mapNode[name] == nullptr)
		{
			_mapNode.erase(name);
			return makeA_Node(name);
		}
		return _mapNode.at(name);
	}

	void ResourceLoader::removeNode(const std::string& name)
	{
		_onlyFree.remove(_mapNode.at(name));
		_mapNode.at(name)->removeFromParent();
		_mapNode.erase(name);
	}

	bool ResourceLoader::addNode(Node* p, std::string name)
	{
		if (name == "")
		{
			return false;
		}

		if(_mapNode.insert(make_pair(name, p)).second)
		{
			_onlyFree.push_back(p);
			return true;
		}
		return false;
	}

	void ResourceLoader::clearAll()
	{
		for (auto iter = _onlyFree.rbegin(); iter != _onlyFree.rend(); ++iter)
		{
			(*iter)->removeFromParent();
		}
		_onlyFree.clear();
		_mapNode.clear();
	}

	void ResourceLoader::removeNode(cocos2d::Node *p)
	{
		auto name = p->getName();
		_currNodeList.remove(p);
		_onlyFree.remove(p);
		_mapNode.erase(name);
		p->removeFromParent();
	}

	void ResourceLoader::clear()
	{
		for (auto iter = _currNodeList.rbegin(); iter != _currNodeList.rend(); ++iter)
		{
			auto p = *iter;
			_onlyFree.remove(p);
			_mapNode.erase(p->getName());
			p->removeFromParent();
		}
		_currNodeList.clear();
	}



	/*cocosResourceLoader******************************************************************************/
	void cocosResourceLoader::loadFile(std::string name)
	{
		_root = CSLoader::createNode(name);
		this->addChild(_root);

		// _mapNode
		deque<Node *> tmpDeque;
		tmpDeque.push_front(_root);
		Node* tmpNode = nullptr;
		while (tmpDeque.size() > 0)
		{
			tmpNode = *tmpDeque.begin();
			auto vChild = tmpNode->getChildren();
			for (auto & child : vChild)
			{
				tmpDeque.push_back(child);
			}
			if (addNode(tmpNode, tmpNode->getName()))
			{
				_currNodeList.push_back(tmpNode);
			}
			/*_mapNode.insert(make_pair(tmpNode->getName(), tmpNode));
			_onlyFree.push_back(tmpNode);*/
			tmpDeque.pop_front();
		}
	}

	/*spriteResourceLoader*****************************************************************************/
	void spriteResourceLoader::loadFile(std::string name2)
	{
		/*name2 = FileUtils::getInstance()->fullPathForFilename(name2);

		tinyxml2::XMLDocument doc;
		
		ssize_t bufferSize;
		auto pBuffer = FileUtils::getInstance()->getFileData(name2.c_str(), "rb", &bufferSize);
		pBuffer[bufferSize] = '\0';
		doc.Parse((const char*)pBuffer);

		auto root = doc.RootElement();
		auto fnt = root->FirstChildElement();
		auto fntPath = fnt->Attribute("path");

		deque<tinyxml2::XMLElement *> xmlDeque;
		xmlDeque.push_back(fnt);
		while (xmlDeque.size() > 0)
		{
			auto p = xmlDeque.front();
			if (p->FirstChildElement() != nullptr)
			{
				auto tmp2 = p->FirstChildElement();
				while(tmp2 != nullptr)
				{
					auto str = tmp2->GetText();
					auto sp = Sprite::create(StringUtils::format("%s%s.png", fntPath, str));
					this->addChild(sp);
					sp->setVisible(false);
					if (addNode(sp, str))
					{
						_currNodeList.push_back(sp);
					}
					xmlDeque.push_back(tmp2);
					tmp2 = tmp2->NextSiblingElement();
				}
			}
			xmlDeque.pop_front();
		}*/
	}

	/**********************************************************************************************************/
	void plistResourceLoader::loadFile(std::string name2)
	{
		if (name2 == "") return;
		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(name2);
	}

	cocos2d::Node* plistResourceLoader::makeA_Node(const std::string& name)
	{
		auto sp = Sprite::createWithSpriteFrameName(name + ".png");
		assert(sp != nullptr);
		this->addChild(sp);
		sp->setVisible(false);
		/*std::string newName = name;
		while (!addNode(sp, newName))
		{
			newName += "_same";
		}
		sp->setName(newName);
		_currNodeList.push_back(sp);*/
		return sp;
	}

	/******************************************************************************************************/
}