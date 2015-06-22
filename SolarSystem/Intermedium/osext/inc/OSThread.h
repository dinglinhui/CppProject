/*
 * OSThread.h
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#ifndef OSThread_H_
#define OSThread_H_
// condition_variable example
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <string>
#include <vector>
#include <functional>
#include <future>
#include "OSDefs.h"

namespace osext {
class OSThread {
public:
	OSThread();
	virtual ~OSThread();

	OSThread(OSThread &&) = delete;
	OSThread(const OSThread &) = delete;
	void operator=(OSThread &&) = delete;
	void operator=(const OSThread &) = delete;

	virtual OSRet Start();
	virtual OSRet Stop();
	virtual OSRet Pause();
	virtual OSRet Continue();

protected:
	virtual int Run() = 0;
	virtual int OSInitHook(void);
	static int ThreadFunction(void *param);

private:
	std::thread *m_pThread = nullptr;
};
} /* namespace osext */

#endif /* OSThread_H_ */
