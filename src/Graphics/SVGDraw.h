#ifndef GRAPHICS_SVGDRAW_H_
#define GRAPHICS_SVGDRAW_H_

#include "FileHandling/Image.h"
#include "Graphics/Objects/Shapes.h"

class SVGDraw {
private:

public:
	Image circle(Circle c, unsigned width){
		Image output{width, width, Image::GRAY};
		return output;
	}
};



#endif
