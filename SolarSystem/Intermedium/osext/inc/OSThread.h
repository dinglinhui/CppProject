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
#include "OSHeartbeat.h"

namespace osext {

class OSHeartbeat;
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
	//
	std::thread::id GetThreadID() {
		return m_pThread->get_id();
	}
	//
	int GetPrio() const {
		return m_nPrio;
	}
	void SetPrio(int prio) {
		m_nPrio = prio;
	}
	//
	int GetOption() const {
		return m_nOption;
	}
	void SetOption(int opt) {
		m_nOption = opt;
	}
	//
	OSHeartbeat& GetHeartbeat() {
		return m_tHeartbeat;
	}

protected:
	virtual OSRet Run() = 0;
	virtual OSRet OSInitHook(void);

	static OSRet ThreadFunction(void *param);

private:
	std::thread *m_pThread;
//	static bool m_bFlag;
//	static std::mutex mutex_;
//	static std::condition_variable cond_;
	int m_nPrio;
	int m_nOption;
	OSHeartbeat m_tHeartbeat;
};
} /* namespace osext */

#endif /* OSThread_H_ */
