/*
 * OSThreadEx.h
 *
 *  Created on: Feb 4, 2015
 *      Author: dinglinhui
 */

#ifndef OSTHREADEX_H_
#define OSTHREADEX_H_
#include "OSMessageBase.h"
#include "OSThread.h"

namespace OSExt {

class OSThreadEx: public OSThread, public OSMessageBase {
public:
	OSThreadEx();
	virtual ~OSThreadEx();

private:
	virtual int Run() override;
};

} /* namespace OSExt */

#endif /* OSTHREADEX_H_ */
