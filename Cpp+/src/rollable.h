/*
 * rollable.h
 *
 *  Created on: Jan 29, 2016
 *      Author: dinglinhui
 */

#ifndef ROLLABLE_H_
#define ROLLABLE_H_

class Rollable {
public:
	Rollable();
	virtual ~Rollable();
	virtual void roll() = 0;
};

#endif /* ROLLABLE_H_ */
