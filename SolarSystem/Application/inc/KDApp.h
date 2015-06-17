/*
 * KDApp.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef KDAPP_H_
#define KDAPP_H_

#include "OSDispatcherEx.h"

namespace KD {
class KDApp: public OSExt::OSDispatcherEx {
public:
	virtual ~KDApp();
	KDApp * GetAppInstance();

protected:
	virtual int OSInitHook(void) override final;
	virtual int OnHandleMessage(OSExt::Message *msg) override final;

private:
	KDApp();
	int Initialize(int *ret = nullptr);
};

} /* namespace KDING */

#endif /* KDAPP_H_ */
