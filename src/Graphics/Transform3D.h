#ifndef GRAPHICS_TRANSFORM3D_H_
#define GRAPHICS_TRANSFORM3D_H_

#include "Math/Matrix.h"

struct Transform3D {
	float 	yaw, pitch, roll,
			transX, transY, transZ,
			scaleX, scaleY, scaleZ;

	Transform3D(const Transform3D& t){ this->copy(t); }
	Transform3D()
	: 	yaw(0), pitch(0), roll(0),
		transX(0), transY(0), transZ(0),
		scaleX(1), scaleY(1), scaleZ(1){}
	Transform3D& operator=(const Transform3D& t){
		if(this==&t) return *this;
		this->copy(t);
		return *this;
	}

	void setRotation(float yaw, float pitch, float roll){
		this->yaw = yaw;
		this->pitch = pitch;
		this->roll = roll;
	}

	void setTranslation(float transX, float transY, float transZ){
		this->transX = transX;
		this->transY = transY;
		this->transZ = transZ;
	}
	void setDilation(float scaleX, float scaleY, float scaleZ){
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->scaleZ = scaleZ;
	}

	Matrix getMatrix(){
		Matrix out = Matrix::gen3DDilation(scaleX, scaleY, scaleZ);
		out = Matrix::gen3DRotationYPR(yaw, pitch, roll)*out;
		out = Matrix::gen3DTranslation(transX, transY, transZ)*out;
		return out;
	}

private:
	void copy(const Transform3D& t){
		setRotation(t.yaw, t.pitch, t.roll);
		setTranslation(t.transX, t.transY, t.transZ);
		setDilation(t.scaleX, t.scaleY, t.scaleZ);
	}
};

#endif
