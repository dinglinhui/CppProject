/*
 * CILSchedule.h
 *
 *  Created on: Jun 13, 2015
 *      Author: dinglinhui
 */

#ifndef CILSCHEDULE_H_
#define CILSCHEDULE_H_

#include "OSThreadEx.h"

namespace syscil {
using namespace osext;

class CILSchedule: public OSThreadEx {
public:
	CILSchedule();
	virtual ~CILSchedule();

private:
	virtual int Run() override final;
};

} /* namespace syscil */

#endif /* CILSCHEDULE_H_ */
