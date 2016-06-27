#ifndef SHAPR_H_
#define SHAPR_H_

class Shape {
public:
	Shape();
	virtual ~Shape();
    virtual void draw() const = 0;
};
#endif
