#ifndef __HNSDK__Module__
#define __HNSDK__Module__

#include "HNPayCallBack.h"
#include <string>
#include <unordered_map>
#include <assert.h>
#include <algorithm>

namespace HN
{
    
#define create_declare(type)    \
public:                              \
static type* shared##type();
    
#define create_impl(type)           \
static type* gsp##type = nullptr;          \
type* type::shared##type()     \
{       \
    if (gsp##type == nullptr) {         \
        gsp##type = new type();          \
    }                               \
    return gsp##type;                \
}

#define property_read_write(type, name)     \
private:                                    \
    type      m##name;                      \
                                            \
public:                                     \
    void set##name(type _##name) {          \
        m##name = _##name;                  \
    }                                       \
                                            \
    type get##name() {                      \
        return m##name;                     \
    }
    
#define property_read(type, name)     \
private:                                    \
    type      m##name;                      \
public:                             \
    type get##name() {                      \
        return m##name;                     \
    }
    
    
#define property_pass_ref(type, name)  \
private:                        \
type    m##name;                          \
public:                             \
void set##name(const type& _##name)            \
{                                       \
    this->m##name = _##name; \
}                                       \
type get##name()                        \
{                                       \
    return m##name;                        \
}
    
    class Module;
    
    /*
        模块函数指针, 指向模块中函数地址
     */
    typedef std::string (Module::*HNSEL)(const std::string& args);
    typedef std::string (Module::*HNSEL_CallFunc)(const std::string& args, CALLBACK_PRAGMA* callback);
    
    #define HN_SELECTOR(_SELECTOR) (HNSEL)(&_SELECTOR)
    #define HN_CALLFUNC_SELECTOR(_SELECTOR) (HNSEL_CallFunc)(&_SELECTOR)

    
    template<class T>
    class Controller
    {
    public:
        typedef typename std::unordered_map<std::string, T>::iterator ITERATOR;
        typedef typename std::unordered_map<std::string, T>::const_iterator CONST_ITERATOR;
        typedef typename std::unordered_map<std::string, T>::value_type  MAP_VALUE;
        
    public:
        void registerObject(const std::string& key, T t)
        {
            assert(!key.empty());
            if (key.empty()) return;

            assert(t != nullptr);
            if (t == nullptr) return;

            std::string strKey(key);
            std::transform(strKey.begin(), strKey.end(), strKey.begin(), ::tolower);
            _map.insert(MAP_VALUE(strKey, t));
        }
        
        T find(const std::string& key)
        {
            assert(!key.empty());
            if (key.empty()) return nullptr;
            
            std::string strKey(key);
            std::transform(strKey.begin(), strKey.end(), strKey.begin(), ::tolower);
            
            CONST_ITERATOR iter = _map.find(strKey);
            T pObject = nullptr;
            if (iter != _map.end())
            {
                pObject = iter->second;
            }
            return pObject;
        }
        
        ITERATOR begin() { return _map.begin(); }
        
        ITERATOR end() { return _map.end(); }
        
    private:
        std::unordered_map<std::string, T> _map;
    };
    
    class Module
    {
    public:
        /**
         *  构造函数
         */
        Module(const std::string& moduleName);
        /**
         *  析构函数
         *
         *  @return return value description
         */
        virtual ~Module();
        /**
         *  注册函数普通函数接口
         *
         *  @param key 方法key
         *  @param sel 函数
         */
        void resisterMethod(const std::string& key, HNSEL sel);
        /**
         *  注册带回调函数接口
         *
         *  @param key        方法key
         *  @param selcallfun 函数
         */
        void registerAyncMethod(const std::string& key, HNSEL_CallFunc selcallfun);
        /**
         *  查找普通函数接口
         *
         *  @param key 方法key
         *
         *  @return return 函数
         */
        HNSEL finMethod(const std::string& key);
        /**
         *  查找带回调函数接口
         *
         *  @param key 方法key
         *
         *  @return return 函数
         */
        HNSEL_CallFunc finAyncMethod(const std::string& key);
        
        property_read_write(int, Tag);
        
        property_pass_ref(std::string, ModuleName);
        
    public:
        /**
         *  模块处理内核通道
         *
         *  @param method   method description
         *  @param args     args description
         *  @param callback callback description
         *
         *  @return return value description
         */
        std::string execute(const std::string& method, const std::string& args,
                                    CALLBACK_PRAGMA* callback);
        
    private:
        // 带回调函数容器
        Controller<HNSEL_CallFunc>* _ayncMethodMap;
        
        // 带函数容器
        Controller<HNSEL>*          _methodMap;
    };
   
}

#endif        //__HNSDK__Module__