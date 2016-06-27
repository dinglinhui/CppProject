/*
 * square.h
 *
 *  Created on: Jan 29, 2016
 *      Author: dinglinhui
 */

#ifndef SQUARE_H_
#define SQUARE_H_
#include "shape.h"

class Square: public Shape {
public:
	Square();
	virtual ~Square();
	void draw() const override;
};

#endif /* SQUARE_H_ */
