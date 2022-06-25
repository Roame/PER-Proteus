#ifndef GRAPHICS_COLOR_H_
#define GRAPHICS_COLOR_H_

struct Color {
	// Very uninspired atm
	float vals[4];

	Color(float r, float g, float b, float a){
		vals[0] = r;
		vals[1] = g;
		vals[2] = b;
		vals[3] = a;
	}

	Color(){ Color(0,0,0,0); }

	Color& operator=(const Color &c){
		if(this == &c) return *this;
		for(unsigned i = 0; i < 4; i++){
			vals[i] = c.vals[i];
		}
		return *this;
	}

	float operator[](unsigned index) {
		return vals[index];
	}

	Color operator+(Color c){
		return Color(vals[0]+c[0],
					 vals[1]+c[1],
					 vals[2]+c[2],
					 vals[3]+c[3]);
	}

	Color operator*(float a){
		return Color(a*vals[0],
					 a*vals[1],
					 a*vals[2],
					 a*vals[3]);
	}
};

#endif
