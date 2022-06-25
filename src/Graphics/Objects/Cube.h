#ifndef GRAPHICS_OBJECTS_CUBE_H_
#define GRAPHICS_OBJECTS_CUBE_H_

#include <vector>

#include "Graphics/GraphicsMgr.h"
#include "Graphics/Color.h"
#include "Graphics/Transform3D.h"

class Cube {
private:
	GraphicsMgr* g;

	unsigned d;
	std::vector<float> vertices = {
		-0.5, -0.5, -0.5,
		-0.5, -0.5,  0.5,
		-0.5,  0.5, -0.5,
		-0.5,  0.5,  0.5,
		 0.5, -0.5, -0.5,
		 0.5, -0.5,  0.5,
		 0.5,  0.5, -0.5,
		 0.5,  0.5,  0.5,
	};
	std::vector<int> indices = {
		0,2,4,
		2,4,6,

		1,3,5,
		3,5,7,

		2,3,6,
		3,6,7,

		0,1,4,
		1,4,5,

		4,5,6,
		5,6,7,

		0,1,2,
		1,2,3,
	};

	Color color;

public:
	Transform3D transform;
	Cube(){
		g = &GraphicsMgr::getInstance();
		g->genDrawable(&d, vertices, indices);
		color = Color(1,1,1,1);
	}
	~Cube(){}

	void draw(){
		g->drawDrawable(d, transform.getMatrix(), color);
	}
};


#endif
