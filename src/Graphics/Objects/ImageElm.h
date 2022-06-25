#ifndef GRAPHICS_OBJECTS_IMAGEELM_H_
#define GRAPHICS_OBJECTS_IMAGEELM_H_

#include <vector>

#include "Element.h"
#include "Shapes.h"
#include "FileHandling/PNGLoader.h"
#include "Graphics/GraphicsMgr.h"

class ImageElm : public Element {
private:
	unsigned d;
public:
	ImageElm(Rectangle rect, const char* filename){
		float w= rect.width, h = rect.height;
		Image img = PNGLoader::decode(filename, true);
		std::vector<float> vertices{
			-w/2, -h/2, 0,	0, 0,
			-w/2,  h/2, 0,	0, 1,
			 w/2, -h/2, 0,	1, 0,
			 w/2,  h/2, 0,	1, 1
		};
		std::vector<int> indices{
			0,1,2,
			1,2,3
		};

		GraphicsMgr::getInstance().genDrawable(&d, vertices, indices, img);
	}
	~ImageElm(){
		GraphicsMgr::getInstance().delDrawable(d);
	}

	void draw(){
		GraphicsMgr::getInstance().drawDrawable(d);
	}

};

#endif
