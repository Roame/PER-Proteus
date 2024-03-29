#include <iostream>
#include <sstream>

#include "Graphics/GraphicsMgr.h"
#include "Graphics/Color.h"
#include "FileHandling/PNGLoader.h"
#include "FileHandling/TxtEditor.h"
#include "Graphics/Camera.h"
#include "Graphics/Objects/RectangleElm.h"
#include "Graphics/Objects/ButtonElm.h"
#include "Graphics/Objects/ImageElm.h"
#include "Graphics/Objects/Shapes.h"
#include "Control/Pipe.h"
#include "Math/Spline2.h"

int main() {
	// Hello!

	GraphicsMgr& g = GraphicsMgr::getInstance();
	Camera* cam = g.getCamera();
	cam->pos.z = 1; // Cam is automatically pointing in the -z-direction btw
	cam->setProjection(Matrix::genOrtho(4, 3, 0, 100));

	GraphicsMgr::WindowDims dims = g.getWDims();
//	Element elm = RectangleElm(Rectangle(0,0,0,1,1), Color(1,1,1,1));



	while(g.active()){
		glClear(GL_COLOR_BUFFER_BIT);
//		g.draw

		g.swapBuffers();
		g.update();
	}
	g.stop();

	return 0;
}


