#ifndef __HN_HNUIDelegateQueue_H__
#define __HN_HNUIDelegateQueue_H__

#include "HNCommonMarco.h"
#include <unordered_map>
#include <assert.h>

namespace HN
{
	template <class T>
	class HNUIDelegateQueue
	{
		typedef typename std::unordered_map<T, T> DelegateQueue;
		typedef typename std::unordered_map<T, T>::iterator ITERATOR;

	public:
		HNUIDelegateQueue()
		{
			_uiQueue = new DelegateQueue();
		}

		~HNUIDelegateQueue()
		{
			HN_SAFE_DELETE(_uiQueue);
		}

	public:
		void addObserver(T delegate)
		{
			assert(nullptr != delegate);
			{
				auto Value = std::make_pair(delegate, delegate);
				_uiQueue->insert(Value);
			}
		}

		void removeObserver(T delegate)
		{
			assert(nullptr != delegate);
			{
				auto iter = _uiQueue->find(delegate);
				if (iter != _uiQueue->end())
				{
					_uiQueue->erase(iter);
				}
			}
		}

		ITERATOR begin()
		{
			return _uiQueue->begin();
		}

		ITERATOR end()
		{
			return _uiQueue->end();
		}

	private:
		DelegateQueue * _uiQueue;
	};
}

#endif	//__HN_HNUIDelegateQueue_H__

