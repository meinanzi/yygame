#include "HNThread.h"
#include <thread>

namespace HN 
{
	HNThread::HNThread(void) : need_quit(false)
	{
	}

	HNThread::~HNThread(void)
	{
	}

	void HNThread::start() 
	{
		auto t = std::thread(std::bind(&HNThread::socketWorkThread, this));
		//t.join();
		t.detach();
	}

	void HNThread::stop() 
	{
		need_quit = true;
	}

	void HNThread::socketWorkThread()
	{
		onExecute();
	}

	void HNThread::onExecute()
	{

	}

}