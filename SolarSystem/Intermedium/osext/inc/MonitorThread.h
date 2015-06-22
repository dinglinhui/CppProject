/*
 * MonitorThread.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef MONITORTHREAD_HPP_
#define MONITORTHREAD_HPP_

#include "OSThread.h"

namespace osext {
class MonitorThread: public OSThread {
public:
	MonitorThread();
	virtual ~MonitorThread();

private:
	virtual int Run() override;
};

} /* namespace osext */

#endif /* MONITORTHREAD_HPP_ */
