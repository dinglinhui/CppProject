/*
 * MonitorThread.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef MONITORTHREAD_HPP_
#define MONITORTHREAD_HPP_

#include "OSThread.h"

namespace KD {
class MonitorThread: public OSExt::OSThread {
public:
	MonitorThread();
	virtual ~MonitorThread();

private:
	virtual int Run() override;
};

} /* namespace OSExt */

#endif /* MONITORTHREAD_HPP_ */
