#ifndef __NH_HNThread_H__
#define __NH_HNThread_H__

#include <functional>
#include <thread>

namespace HN 
{
	typedef std::function<void(void* pragma)> HNThreadFunc;

	class HNThread
	{
	public:
		HNThread(void);
		virtual ~HNThread(void);

	public:
		void start();
		void stop();
		virtual void onExecute();

	protected:	
		bool need_quit;

	private:
		void socketWorkThread();

	private:
		std::thread* _thread;
		HNThreadFunc _func;
	};

};

#endif		//__NH_HNThread_H__