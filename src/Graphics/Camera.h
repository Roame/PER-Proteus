#ifndef GRAPHICS_CAMERA_H_
#define GRAPHICS_CAMERA_H_

#include "Math/Matrix.h"
#include "Transform3d.h"
#include "Math/Vec3.h"

class Camera {
private:
	Matrix projection;

public:
	float yaw, pitch, roll;
	Vec3 pos;

	Camera()
	: yaw(0), pitch(0), roll(0){
		projection = Matrix::genOrtho(2, 2, 0, 100);
	}
	~Camera(){}

	void setRotation(float yaw, float pitch, float roll){
		this->yaw = yaw;
		this->pitch = pitch;
		this->roll = roll;
	}

	void setProjection(Matrix projection){ this->projection = projection; }

	Matrix getMatrix(){
		Matrix out = Matrix::genIdentity(4);
		out = Matrix::gen3DTranslation(-pos.x, -pos.y, pos.z)*out;
		out = Matrix::gen3DRotationZ(-roll)*
				Matrix::gen3DRotationX(-pitch)*
				Matrix::gen3DRotationY(-yaw)*out;
		out = projection*out;
		return out;
	}

};



#endif
