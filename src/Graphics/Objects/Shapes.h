#ifndef GRAPHICS_OBJECTS_SHAPES_H_
#define GRAPHICS_OBJECTS_SHAPES_H_

#include <iostream>
#include <vector>

#include "Graphics/GraphicsMgr.h"
#include "Math/Vec2.h"

struct Shape {
	float x, y, rot;

	Shape(float x, float y, float rot)
		:x(x), y(y), rot(rot){}
	Shape():Shape(0,0,0){}
	virtual ~Shape(){}

	Vec2 getPos(){ return Vec2(x, y); }

};

struct Rectangle : public Shape {
	float width, height;
	Rectangle(float x, float y, float rot, float width, float height)
		: Shape(x,y,rot), width(width), height(height){}
};

struct Circle : public Shape {
	float radius;
};


template<typename T>
bool isInstanceOf(Shape *ptr){
	if(dynamic_cast<T*>(ptr) != 0) return true;
	return false;
}

static unsigned genGeometry(Shape *shape){
	if(isInstanceOf<Rectangle>(shape)){
		Rectangle* rect = dynamic_cast<Rectangle*>(shape);
		float w = rect->width, h = rect->height;
		std::vector<float> vertices = {
			-w/2, -h/2, 0,
			-w/2,  h/2, 0,
			 w/2, -h/2, 0,
			 w/2,  h/2, 0
		};
		std::vector<int> indices = {
			0,1,2,
			1,2,3
		};
		unsigned d;
		GraphicsMgr::getInstance().genDrawable(&d, vertices, indices);

		return d;
	}

//	if(isInstanceOf<Circle>(shape)){}

	return -1;
}

static void drawShape(Shape *shape, Color color=Color(1,1,1,1)){
	unsigned d = genGeometry(shape);
	Matrix transform = Matrix::gen3DRotationZ(shape->rot);
	transform = Matrix::gen3DTranslation(shape->x, shape->y, 0) * transform;
	GraphicsMgr::getInstance().drawDrawable(d, transform, color);
	GraphicsMgr::getInstance().delDrawable(d);
}




#endif
