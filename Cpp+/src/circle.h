/*
 * circle.h
 *
 *  Created on: Jan 29, 2016
 *      Author: dinglinhui
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "rollable.h"
#include "shape.h"

class Circle: virtual public Shape, virtual public Rollable {
public:
	Circle();
	virtual ~Circle();
	void draw() const override;
	void roll() override;
};

#endif /* CIRCLE_H_ */
