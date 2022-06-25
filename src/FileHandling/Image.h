#ifndef FILEHANDLING_IMAGE_H_
#define FILEHANDLING_IMAGE_H_

#include <iostream>
#include <vector>

struct Image {
	unsigned width, height;
	std::vector<unsigned char> data;
	enum ColorSpace {
		RGBA,
		GRAY
	} cSpace{RGBA};

	Image(unsigned width, unsigned height, std::vector<unsigned char> data)
	: width(width), height(height), data(data){}
	Image(unsigned width, unsigned height, ColorSpace cSpace)
	: width(width), height(height), cSpace(cSpace){}
	Image()
	: width(0), height(0){}

	Image resizeWidth(unsigned nWidth){
		if(nWidth>width){
			std::cout << "Not yet implemented" << std::endl; //TODO implement this
			return Image();
		} else if(nWidth == width){
			return *this;
		}

		float ar = (float)width/height;
		unsigned nHeight = nWidth/ar;
		float windowLen = (float)width/nWidth;
		Image output{ nWidth, nHeight, std::vector<unsigned char>(nWidth*nHeight*4) };

		//TODO I'm being a bit lazy here, but also it's faster computationally so...
		for(unsigned i = 0; i < nWidth; i++){
			for(unsigned j = 0; j < nHeight; j++){
				int x = windowLen*(i + 0.5);
				int y = windowLen*(j + 0.5);
				output.setPixel(i, j, getPixelRGBA(x, y));
			}
		}
		return output;
	}

	unsigned getNumPixels(){
		return width*height;
	}

	void setPixel(int x, int y, std::vector<float> color) {
		switch(cSpace){
			case GRAY:{
				unsigned location = x+y*width;
				float sum = 0;
				for(int i = 0; i < 3; i++){
					sum += color[i];
				}
				unsigned char val = (sum/3)*color[3]; // Kind of a strange formula atm
				data[location] = val;
			} break;
			case RGBA:{
				unsigned location = (x+y*width)*4;
				for(int i = 0; i < 4; i++){
					data[location+i] =(int)(color[i]*255);
				}
			} break;
			default:
				break;
		}
	}

	void setPixel(int x, int y, unsigned char val) {
		switch(cSpace){
			case GRAY:{
				unsigned location = x+y*width;
				data[location] = val;
			} break;
			case RGBA:{
				unsigned location = (x+y*width)*4;
				for(int i = 0; i < 4; i++){
					data[location+i] = val;
				}
			} break;
			default:
				break;
		}
	}

	unsigned char getPixelGray(int x, int y){
		int location = x+y*width;
		return data[location];
	}

	std::vector<float> getPixelRGBA(int x, int y){
		int location = (x+y*width)*4;
		return std::vector<float>{(float)data[location]/255, (float)data[location+1]/255, (float)data[location+2]/255, (float)data[location+3]/255};
	}
};

#endif
