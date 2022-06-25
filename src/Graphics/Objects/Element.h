#ifndef GRAPHICS_OBJECTS_ELEMENT_H_
#define GRAPHICS_OBJECTS_ELEMENT_H_

class Element {
private:
public:
	Element(){}
	virtual ~Element() = default;

	virtual void draw(){};
};



#endif
