#ifndef _HN_MessageHead_H_
#define _HN_MessageHead_H_

#include "HNBaseType.h"
#include "HNCommon/HNCommonMarco.h"
#include "HNComStruct.h"

namespace HN 
{
#pragma  pack(1)
	// network packet header
	typedef struct tagNetMessageHead
	{		
		UINT						uMessageSize;			// 数据包大小
		UINT						bMainID;				// 处理主类型
		UINT						bAssistantID;			// 辅助处理类型 ID
		UINT						bHandleCode;			// 数据包处理代码
		UINT						bReserve;				// 保留字段
	}  NetMessageHead;

#pragma pack()
}

#endif // !_HN_NetMessageHead_H_
