#include "HNModule.h"

namespace HN
{
    Module::Module(const std::string& moduleName)
        : mModuleName(moduleName)
        , _methodMap(nullptr)
        , _ayncMethodMap(nullptr)
    {
        
    }
    
    Module::~Module()
    {
        
    }
    
    void Module::resisterMethod(const std::string& key, HNSEL sel)
    {
        if (_methodMap == nullptr)
        {
            _methodMap = new Controller<HNSEL>();
        }
        _methodMap->registerObject(key, sel);
    }
    
    void Module::registerAyncMethod(const std::string& key, HNSEL_CallFunc selcallfun)
    {
        if (_ayncMethodMap == nullptr)
        {
            _ayncMethodMap = new Controller<HNSEL_CallFunc>();
        }
        _ayncMethodMap->registerObject(key, selcallfun);
    }
    
    HNSEL Module::finMethod(const std::string& key)
    {
        if (_methodMap == nullptr) return nullptr;
        
        HNSEL pMethod = _methodMap->find(key);
        return pMethod;
    }
    
    HNSEL_CallFunc Module::finAyncMethod(const std::string& key)
    {
        if (_ayncMethodMap == nullptr) return nullptr;
        
        HNSEL_CallFunc pMethod = _ayncMethodMap->find(key);
        return pMethod;
    }
    
    std::string Module::execute(const std::string& method, const std::string& args,
                                CALLBACK_PRAGMA* callback)
    {
        HNSEL_CallFunc pCallFuncMethod = finAyncMethod(method);
        
        if (pCallFuncMethod != nullptr)
        {
            return (this->*pCallFuncMethod)(args, callback);
        }
        
        HNSEL pMethod = finMethod(method);
        if (pMethod != nullptr)
        {
            return (this->*pMethod)(args);
        }
        
        return "-1";
    }
}