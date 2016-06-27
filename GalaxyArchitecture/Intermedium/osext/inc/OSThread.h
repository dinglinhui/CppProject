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
#include <future>
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
	OSThread(int nPrio, int nStackSize = 512, WORD nTskOpt = 0);
	OSThread(std::string name, int prio);
	virtual ~OSThread();

	OSThread(OSThread &&) = delete;
	OSThread(const OSThread &) = delete;
	void operator=(OSThread &&) = delete;
	void operator=(const OSThread &) = delete;

	virtual OSRet start();
	virtual OSRet stop();
	virtual OSRet suspend();
	virtual OSRet resume();
	//
	std::thread::id getThreadID() {
		return thread_->get_id();
	}
	//
	std::string getThreadName() {
		return name_;
	}
	//
	TStat getThreadStatus() {
		return status_;
	}
	void setThreadStatus(TStat status) {
		status_ = status;
	}
	//
	int getPrio() const {
		return prio_;
	}
	void setPrio(int prio) {
		prio_ = prio;
	}
	//
	int getOption() const {
		return option_;
	}
	void setOption(int opt) {
		option_ = opt;
	}
	//
	OSHeartbeat& getHeartbeat() {
		return heartbeat_;
	}

protected:
	virtual OSRet run() = 0;

	virtual OSRet OSInitHook(void);

	static OSRet staticThreadFunction(void *param);

private:
	std::thread *thread_;
	std::string name_;
	TStat status_;
//	static bool flag_;
//	static std::mutex mutex_;
//	static std::condition_variable cond_;
	int prio_;
	int option_;
	OSHeartbeat heartbeat_;
};
} /* namespace osext */

#endif /* OSThread_H_ */
