#ifndef MATH_SPLINE2_H_
#define MATH_SPLINE2_H_

#include <vector>

#include "Math/Vec2.h"

struct SplinePoint {
	float x, y, h1{}, h2{};

	enum HType {
		NONE,
		CUSP,
//		SMOOTH, // TODO implement smooth behavior
		DEFINED
	} h1Type, h2Type;

	SplinePoint(float x, float y)
	: x(x), y(y), h1Type(CUSP), h2Type(CUSP){}
	SplinePoint(float x, float y, float h1, HType h2Type)
	: x(x), y(y), h1(h1), h1Type(DEFINED), h2Type(h2Type) {}
	SplinePoint(float x, float y, HType h1Type, float h2)
	: x(x), y(y), h2(h2), h1Type(h1Type), h2Type(DEFINED){}
	SplinePoint(float x, float y, float h)
	: x(x), y(y), h1(h), h2(h), h1Type(DEFINED), h2Type(DEFINED){}
	SplinePoint(float x, float y, float h1, float h2)
	: x(x), y(y), h1(h1), h2(h2), h1Type(DEFINED), h2Type(DEFINED){}
};

class Spline2 {
private:
	std::vector<SplinePoint> points;

	struct Curve2 {
		std::vector<float> xCoeffs, yCoeffs;

		Vec2 genPoint(float t){
			Vec2 out;
			for(unsigned i = 0; i < xCoeffs.size(); i++){
				out.x += xCoeffs[i]*pow(t,i);
				out.y += yCoeffs[i]*pow(t,i);
			}
			return out;
		}
	};
	std::vector<Curve2> curves;

	void genCoeffs(){
		curves.clear();
		if(points.size() < 2) return;
		for(unsigned i = 0; i < points.size()-1; i++){
			SplinePoint p1 = points[i], p2 = points[i+1];
			Curve2 curve;
			if(p1.h2Type==SplinePoint::DEFINED && p2.h1Type == SplinePoint::DEFINED){
				std::vector<float> xCoeffs(4), yCoeffs(4);
				xCoeffs[0] = p1.x;
				xCoeffs[1] = cos(p1.h2);
				xCoeffs[2] = -3*p1.x - 2*cos(p1.h2) + 3*p2.x - cos(p2.h1);
				xCoeffs[3] =  2*p1.x +   cos(p1.h2) - 2*p2.x + cos(p2.h1);

				yCoeffs[0] = p1.y;
				yCoeffs[1] = sin(p1.h2);
				yCoeffs[2] = -3*p1.y - 2*sin(p1.h2) + 3*p2.y - sin(p2.h1);
				yCoeffs[3] =  2*p1.y +   sin(p1.h2) - 2*p2.y + sin(p2.h1);

				curve = {xCoeffs, yCoeffs};
			} else if(p1.h2Type == SplinePoint::DEFINED && p2.h1Type == SplinePoint::CUSP){
				std::vector<float> xCoeffs(3), yCoeffs(3);
				xCoeffs[0] = p1.x;
				xCoeffs[1] = cos(p1.h2);
				xCoeffs[2] = p2.x-p1.x-cos(p1.h2);

				yCoeffs[0] = p1.y;
				yCoeffs[1] = sin(p1.h2);
				yCoeffs[2] = p2.y-p1.y-sin(p1.h2);

				curve = {xCoeffs, yCoeffs};
			} else if(p1.h2Type == SplinePoint::CUSP && p2.h1Type == SplinePoint::DEFINED){
				std::vector<float> xCoeffs(3), yCoeffs(3);
				xCoeffs[0] = p1.x;
				xCoeffs[1] = -2*p1.x + 2*p2.x - cos(p2.h1);
				xCoeffs[2] = p1.x - p2.x + cos(p2.h1);

				yCoeffs[0] = p1.y;
				yCoeffs[1] = -2*p1.y + 2*p2.y - sin(p2.h1);
				yCoeffs[2] = p1.y - p2.y + sin(p2.h1);

				curve = {xCoeffs, yCoeffs};
			} else if(p1.h2Type == SplinePoint::CUSP && p2.h1Type == SplinePoint::CUSP){
				std::vector<float> xCoeffs(2), yCoeffs(2);
				xCoeffs[0] = p1.x;
				xCoeffs[1] = p2.x-p1.x;

				yCoeffs[0] = p1.y;
				yCoeffs[1] = p2.y-p1.y;

				curve = {xCoeffs, yCoeffs};
			}

			curves.push_back(curve);
		}
	}

public:
	Spline2(){}
	~Spline2(){}

	void addPoint(SplinePoint point){
		points.push_back(point);
		genCoeffs();
	}

	Vec2 genPoint(float t){
		if(points.size() == 0)
			return Vec2();
		if(points.size() == 1)
			return Vec2(points[0].x, points[0].y);
		if(t < 0 || t > 1)
			return Vec2();

		if(points.size() >= 2){
			float scaledT = t*(points.size()-1);
			unsigned p1Index = static_cast<unsigned>(scaledT);
			if(p1Index == points.size()-1){
				p1Index--;
			}
			float newT = scaledT-p1Index;
			return curves[p1Index].genPoint(newT);
		}

		return Vec2(); // Pretty sure this is useless, but eclipse is fussy
	}
};



#endif
