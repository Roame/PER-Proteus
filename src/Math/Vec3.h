#ifndef VEC3_H_
#define VEC3_H_

#include <sstream>
#include <iomanip>

struct Vec3 {
public:
	float x, y, z;

	Vec3(float x, float y, float z)
	: x(x), y(y), z(z){}

	Vec3()
	: x(0), y(0), z(0){}

	Vec3 operator+(Vec3 a){
		return Vec3(x+a.x, y+a.y, z+a.z);
	}

	Vec3 operator*(float a){
		return Vec3(a*x, a*y, a*z);
	}

	Vec3 operator-(Vec3 a){
		return *this+a*-1;
	}

	float dot(Vec3 a){
		return x*a.x+y*a.y+z*a.z;
	}

	Vec3 cross(Vec3 a){
		return Vec3(y*a.z-z*a.y, -x*a.z+z*a.x, x*a.y-y*a.x);
	}

	float norm(){
		return pow(pow(x, 2) + pow(y, 2) + pow(z, 2), 0.5);
	}

	Vec3 getUnitVec(){
		return (*this)*(1/norm());
	}

	std::string toString(){
		std::stringstream sstm;
		sstm << std::setprecision(2) << "(" << x << ", " << y << ", " << z << ")";
		return sstm.str();
	}
};

#endif
