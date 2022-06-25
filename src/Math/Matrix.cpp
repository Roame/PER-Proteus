#include "Matrix.h"

Matrix::Matrix(int rows, int columns)
: rows(rows), cols(columns) {
	data.resize(rows*columns, 0);
}

void Matrix::setVal(int row, int column, float val){
	int index = row*cols+column;
	data[index] = val;
}

float Matrix::getVal(int row, int column){
	int index = row*cols+column;
	return data[index];
}

float* Matrix::getData(){
	return data.data();
}

std::string Matrix::toString(){
	std::stringstream sstm;
	sstm << "(" << rows << ", " << cols << ") matrix:\n";
	for(int i = 0; i < rows; i++){
		for(int k = 0; k < cols; k++){
			sstm << getVal(i, k) << " ";
		}
		sstm << "\n";
	}
	return sstm.str();
}

Matrix Matrix::operator+(Matrix a){
	if(a.rows != rows || a.cols != cols){
		std::cout << "Error: Matrix addition - Matrices do not have the same dimensions"; // TODO proper error throwing
		exit(1);
	}

	Matrix output(rows, cols);

	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			output.setVal(i, j, getVal(i,j)+a.getVal(i,j));
		}
	}
	return output;
}

Matrix Matrix::operator*(Matrix a){
	int rows1 = rows, rows2 = a.rows,
		cols1 = cols, cols2 = a.cols;

	if(cols1 != rows2){
		std::cout << "Inner dimensions of matrices don't match"; // TODO proper error throwing
		exit(1);
	}

	Matrix output(rows1, cols2);

	for(int i = 0; i < rows1; i++){
		for(int j = 0; j < cols2; j++){
			float sum = 0;
			for(int k = 0; k < cols1; k++){
				sum += getVal(i, k)*a.getVal(k, j);
			}
			output.setVal(i, j, sum);
		}
	}
	return output;
}

Matrix Matrix::genIdentity(int n){
	Matrix output(n, n);
	for(int i = 0; i < n; i++){
		output.setVal(i, i, 1);
	}
	return output;
}

Matrix Matrix::genRand(int rows, int columns, float min, float max){
	static bool init = false;
	if(!init){
		init = true;
		srand((unsigned) time(NULL));
	}

	Matrix output(rows, columns);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			float value = ((float)(rand()%1001))/1000*(max-min)+min; // TODO: not sure how I feel about the limited # of decimal places this produces
			output.setVal(i, j, value);
		}
	}
	return output;
}

Matrix Matrix::gen3DRotationX(float rads){
	Matrix mat(4, 4);
	mat.setVal(3,3, 1);
	mat.setVal(0,0, 1);
	mat.setVal(1,1, cos(rads));
	mat.setVal(1,2, -sin(rads));
	mat.setVal(2,1, sin(rads));
	mat.setVal(2,2, cos(rads));
	return mat;
}

Matrix Matrix::gen3DRotationY(float rads){
	Matrix mat(4, 4);
	mat.setVal(3,3, 1);
	mat.setVal(1,1, 1);
	mat.setVal(0,0, cos(rads));
	mat.setVal(0,2, sin(rads));
	mat.setVal(2,0, -sin(rads));
	mat.setVal(2,2, cos(rads));
	return mat;
}

Matrix Matrix::gen3DRotationZ(float rads){
	Matrix mat(4, 4);
	mat.setVal(3,3, 1);
	mat.setVal(2,2, 1);
	mat.setVal(0,0, cos(rads));
	mat.setVal(0,1, -sin(rads));
	mat.setVal(1,0, sin(rads));
	mat.setVal(1,1, cos(rads));
	return mat;
}

Matrix Matrix::gen3DRotationYPR(float yaw, float pitch, float roll){
	Matrix output = Matrix::gen3DRotationY(yaw)*
			Matrix::gen3DRotationX(pitch)*
			Matrix::gen3DRotationZ(roll);
	return output;
}

Matrix Matrix::gen3DTranslation(float x, float y, float z){
	Matrix mat = Matrix::genIdentity(4);
	mat.setVal(0,3, x);
	mat.setVal(1,3, y);
	mat.setVal(2,3, z);
	return mat;
}

Matrix Matrix::gen3DDilation(float x, float y, float z){
	Matrix mat = Matrix::genIdentity(4);
	mat.setVal(0,0, x);
	mat.setVal(1,1, y);
	mat.setVal(2,2, z);
	return mat;
}

Matrix Matrix::genProjection(float ar, float FOV, float fLength, float nearZ, float farZ){
	Matrix output(4,4);
	output.setVal(0,0, fLength/(ar*tan(FOV/2)));
	output.setVal(1,1, fLength/(ar*tan(FOV/2)));
	output.setVal(2,2, -(nearZ+farZ)/(nearZ-farZ));
	output.setVal(3,2, 1);
	output.setVal(2,3, (2*farZ*nearZ)/(nearZ-farZ));
	return output;
}

Matrix Matrix::genOrtho(float width, float height, float nearZ, float farZ){
	Matrix output = Matrix::genIdentity(4);
	output.setVal(0,0, 2/width);
	output.setVal(1,1, 2/height);
	output.setVal(2,2, 2/(farZ-nearZ));
	output.setVal(2,3, -2*nearZ/(farZ-nearZ)-1);
	return output;
}
