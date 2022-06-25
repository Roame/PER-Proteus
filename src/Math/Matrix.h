#ifndef MATRIX_H_
#define MATRIX_H_

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>

class Matrix {

public:
	int rows, cols;
	std::vector<float> data;

	Matrix(int rows, int columns);
	Matrix(): Matrix(0,0){}
	~Matrix(){}
	// TODO initialization with arrays

	void setVal(int row, int column, float val);
	float getVal(int row, int column);
	float* getData();

	std::string toString();

	Matrix operator+(Matrix a);
	Matrix operator*(Matrix a);

	static Matrix genIdentity(int n);
	static Matrix genRand(int rows, int columns, float min, float max);
	static Matrix gen3DRotationX(float radians);
	static Matrix gen3DRotationY(float radians);
	static Matrix gen3DRotationZ(float radians);
	static Matrix gen3DRotationYPR(float yaw, float pitch, float roll);
	static Matrix gen3DTranslation(float x, float y, float z);
	static Matrix gen3DDilation(float x, float y, float z);
	static Matrix genProjection(float ar, float FOV, float focalLength, float nearZ, float farZ);
	static Matrix genOrtho(float width, float height, float nearZ, float farZ);
};
#endif
