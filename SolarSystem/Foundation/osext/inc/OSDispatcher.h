/*
 * OSDispatcher.h
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#ifndef OSDISPATCHER_H_
#define OSDISPATCHER_H_

#include "OSThread.h"

namespace OSExt {
class OSDispatcher: public OSThread {
public:
	OSDispatcher();
	virtual ~OSDispatcher();

private:
	int Create(int nTskOpt = 0);

	virtual OSRet Start() override;
	virtual OSRet Stop() override;
	virtual OSRet Pause() override;
	virtual OSRet Continue() override;

	OSThread *GetThread(int nPrio);
};

} /* namespace OSExt */

#endif /* OSDISPATCHER_H_ */