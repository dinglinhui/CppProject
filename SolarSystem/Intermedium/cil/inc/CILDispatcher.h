/*
 * CILDispatcher.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef CILDISPATCHER_H_
#define CILDISPATCHER_H_

#include "OSThreadEx.h"
namespace CIL {
class CILDispatcher: public OSExt::OSThreadEx {
public:
	CILDispatcher();
	virtual ~CILDispatcher();

private:
	virtual int Run() override final;
};

} /* namespace OSExt */

#endif /* CILDISPATCHER_H_ */
