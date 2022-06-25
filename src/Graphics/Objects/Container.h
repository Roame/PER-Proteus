#ifndef GRAPHICS_OBJECTS_CONTAINER_H_
#define GRAPHICS_OBJECTS_CONTAINER_H_

#include <vector>

#include "Element.h"

class Container : public Element {
private:
	std::vector<Element*> elements;

public:
	Container(){}
	virtual ~Container() = default;

	void draw(){
		drawContainer();
		for(auto element : elements)
			element->draw();
	}

	virtual void drawContainer();
};



#endif
