/*
 * CILAbstractFactory.h
 *
 *  Created on: Jun 28, 2015
 *      Author: dinglinhui
 */

#ifndef CILABSTRACTFACTORY_H_
#define CILABSTRACTFACTORY_H_

namespace syscil {

class CILAbstractFactory {
public:
	virtual ~CILAbstractFactory();

	virtual void * Create(const char * class_name) = 0;
};

} /* namespace syscil */

#endif /* CILABSTRACTFACTORY_H_ */
