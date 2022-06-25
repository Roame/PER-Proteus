#ifndef VEC2_H_
#define VEC2_H_

#include <sstream>
#include <iomanip>
#include <math.h>

struct Vec2 {
public:
	float x, y;

	Vec2(float x, float y)
	: x(x), y(y){}

	Vec2()
	: Vec2(0, 0){}

	Vec2(const Vec2& v){
		x = v.x;
		y = v.y;
	}

	Vec2 operator+(Vec2 a){
		return Vec2(x+a.x, y+a.y);
	}

	Vec2 operator*(float a){
		return Vec2(a*x, a*y);
	}

	Vec2 operator-(Vec2 a){
		return *this+a*-1;
	}

	void operator+=(Vec2 a){
		x = this->x+a.x;
		y = this->y+a.y;
	}

	float dot(Vec2 a){
		return x*a.x+y*a.y;
	}

	float cross(Vec2 a){
		return x*a.y-y*a.x;
	}

	Vec2 rotate(float rads){
		float outX = x*cos(rads)-y*sin(rads);
		float outY = x*sin(rads)+y*cos(rads);
		return Vec2(outX, outY);
	}

	float norm(){
		return pow(pow(x, 2) + pow(y, 2), 0.5);
	}

	Vec2 getUnitVec(){
		if(norm() == 0){
			return Vec2();
		}
		return (*this)*(1/norm());
	}

	std::string toString(){
		std::stringstream sstm;
		sstm << std::setprecision(2) << "(" << x << ", " << y << ")";
		return sstm.str();
	}
};

#endif
