#ifndef GRAPHICS_OBJECTS_RECTANGLEELM_H_
#define GRAPHICS_OBJECTS_RECTANGLEELM_H_

#include <vector>

#include "Element.h"
#include "Shapes.h"
#include "Graphics/GraphicsMgr.h"
#include "Graphics/Color.h"

class RectangleElm : Element {
private:
	Rectangle rect;
	Color color;

public:
	RectangleElm(Rectangle rectangle, Color color)
	: rect(rectangle), color(color){}

	void draw(){
		drawShape(&rect, color);
	}

};


#endif
