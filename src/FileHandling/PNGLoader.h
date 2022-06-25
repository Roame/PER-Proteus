#ifndef PNGLOADER_H_
#define PNGLOADER_H_

// shoutout to lodepng for saving my sanity

#include <FileHandling/Image.h>
#include "lodepng.h"

class PNGLoader {
public:
	static Image decode(const char* filename, bool flipVertical=false){
		std::vector<unsigned char> image;
		unsigned width, height;
		unsigned error = lodepng::decode(image, width, height, filename);
		if(error){
			std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
			exit(1);
		} else {
			if(flipVertical){
				std::vector<unsigned char> newImage(image.size());
				for(unsigned j = 0; j < height; j++){
					for(unsigned i = 0; i < width; i++){
						unsigned index = i+j*width;
						unsigned nIndex = i+(height-j-1)*width;
						for(int k = 0; k < 4; k++){
							newImage[4*nIndex+k] = image[4*index+k];
						}
					}
				}
				image = newImage;
			}
			Image output = {width, height, image};
			return output;
		}
	}

	static void encode(const char* filename, Image* image){
		unsigned error = lodepng::encode(filename, image->data, image->width, image->height);
		if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
	}
};

#endif
