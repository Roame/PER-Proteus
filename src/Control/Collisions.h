#ifndef CONTROL_COLLISIONS_H_
#define CONTROL_COLLISIONS_H_

#include "Math/Vec2.h"
#include "Graphics/Objects/Shapes.h"

class Collisions {
public:
	static bool checkColl(Vec2 location, Rectangle area){
		Vec2 transLoc = (location-area.getPos()).rotate(-area.rot);
		if(transLoc.x >= -area.width/2 &&
				transLoc.x <= area.width/2 &&
				transLoc.y >= -area.height/2 &&
				transLoc.y <= area.height/2)
			return true;
		return false;
	}

};



#endif
