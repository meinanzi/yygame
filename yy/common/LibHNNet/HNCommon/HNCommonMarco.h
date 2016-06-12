#ifndef __HN_HNCommonMarco_H__
#define __HN_HNCommonMarco_H__

#define CHECK_SOCKET_DATA(_type, _size, _text)	\
	CCAssert(sizeof (_type) == _size, _text); \
	if (sizeof (_type) != _size) return;

#define CHECK_SOCKET_DATA_RETURN(_type, _size, _ret, _text)	\
	CCAssert(sizeof (_type) == _size, _text); \
	if (sizeof (_type) != _size) return _ret;

#define HN_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)

#define HN_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)

#define HN_PROPERTY_DECLARATION(varType, funName, varName)		\
	private: varType	_##varName;		\
	public: void set##funName(varType varName);		\
	public: varType get##funName() const;		


#define HN_PROPERTY(varType, funName, varName)		\
	private: varType	_##varName;		\
	public: void set##funName(varType varName) { _##varName = varName; } \
	public: varType get##funName() const { return _##varName; }		


#define HN_PROPERTY_READONLY(varType, funName, varName)		\
	private: varType	_##varName;		\
	public:	varType get##funName() const { return _##varName; }


#define HN_PROPERTY_PASS_BY_REF(varType, funName, varName)		\
	private: varType	_##varName;		\
	public:	void set##funName(const varType& varName) { _##varName = varName; } \
	public:	varType get##funName() const { return _##varName; }		


#define HN_ARRAY_COUNT(_array)		(sizeof(_array) / sizeof(_array[0]))

#define HN_ASSERT(con, _msg_)	assert(con)

#ifdef __GNUC__
#define GNUC_PACKED1
#else
#define GNUC_PACKED1
#endif

#ifdef __GNUC__
#define GNUC_PACKED __attribute__ ((__ms_struct__))
#else
#define GNUC_PACKED
#endif

#ifdef __arm
#define ARM_PACKED    //__packed
#else
#define ARM_PACKED
#endif

#define HN_NULL_RETURN(varName)  \
	if(varName == nullptr) \
{ \
		return;\
}

#endif	//__HN_HNCommonMarco_H__
