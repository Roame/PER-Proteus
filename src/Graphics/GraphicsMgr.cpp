#include "GraphicsMgr.h"

std::vector<Pipe<KeyInputEvent>*> GraphicsMgr::keyInputPipes;
std::vector<Pipe<CharInputEvent>*> GraphicsMgr::charInputPipes;
std::vector<Pipe<CursorMoveEvent>*> GraphicsMgr::cursorMovePipes;
std::vector<Pipe<MouseButtonEvent>*> GraphicsMgr::mouseButtonPipes;
std::vector<Pipe<MouseScrollEvent>*> GraphicsMgr::mouseScrollPipes;

GraphicsMgr::GraphicsMgr(){
	if(!glfwInit()) exit(-1);

	window = glfwCreateWindow(640, 480, "Proteus", NULL, NULL);
	if(!window) { glfwTerminate(); exit(-1); }
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
		std::cout << "Failed to init OpenGL context" << std::endl;
		exit(-1);
	}

	glfwSetErrorCallback(errorCB);
	glfwSetKeyCallback(window, keyCB);
	glfwSetCharCallback(window, charCB);
	glfwSetCursorPosCallback(window, cursorPosCB);
	glfwSetMouseButtonCallback(window, mouseButtonCB);
	glfwSetScrollCallback(window, scrollCB);

	createProgram(&program1, "src/Graphics/Basic_VShader.txt", "src/Graphics/Basic_FShader.txt");
	createProgram(&program2, "src/Graphics/Tex_VShader.txt", "src/Graphics/Tex_FShader.txt");
	retrieveUniform(&gTransformLoc1, program1, "gWorld");
	retrieveUniform(&gColorLoc, program1, "gColor");
	retrieveUniform(&gTransformLoc2, program2, "gWorld");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GraphicsMgr::errorCB(int error, const char *description){
	fprintf(stderr, "Error: %s\n", description);
}

void GraphicsMgr::keyCB(GLFWwindow *window, int key, int scancode, int action, int mods){
	for(auto pipe : keyInputPipes){
		pipe->add(KeyInputEvent{key, scancode, action, mods});
	}
}

void GraphicsMgr::charCB(GLFWwindow *window, unsigned codepoint){
	for(auto pipe : charInputPipes){
		pipe->add(CharInputEvent{codepoint});
	}
}

void GraphicsMgr::cursorPosCB(GLFWwindow *window, double xPos, double yPos){
	for(auto pipe : cursorMovePipes){
		pipe->add(CursorMoveEvent{xPos, yPos});
	}
}

void GraphicsMgr::mouseButtonCB(GLFWwindow *window, int button, int action, int mods){
	for(auto pipe : mouseButtonPipes){
		pipe->add(MouseButtonEvent{button, action, mods});
	}
}

void GraphicsMgr::scrollCB(GLFWwindow *window, double xOffset, double yOffset){
	for(auto pipe : mouseScrollPipes){
		pipe->add(MouseScrollEvent{xOffset, yOffset});
	}
}

void GraphicsMgr::addShader(GLuint program, const char *shaderText, GLenum shaderType){
	GLuint shaderObj = glCreateShader(shaderType);
	if(shaderObj == 0){
		fprintf(stderr, "Error creating shader type %d\n", shaderType);
		exit(1);
	}

	const GLchar* p[1];
	p[0] = shaderText;
	GLint lengths[1];
	lengths[0] = strlen(shaderText);
	glShaderSource(shaderObj, 1, p, lengths);
	glCompileShader(shaderObj);
	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if(!success){
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, infoLog);
		exit(1);
	}
	glAttachShader(program, shaderObj);
}

void GraphicsMgr::createProgram(GLuint *handle, const char *vs, const char *fs){
	GLuint program = glCreateProgram();

	if(program == 0){
		fprintf(stderr, "Error creating shader program");
		exit(1);
	}
	*handle = program;

	std::string vsString, fsString;
	vsString = TxtEditor::read(vs);
	fsString = TxtEditor::read(fs);
	if(vsString.length() == 0 || fsString.length() == 0) exit(1);

	addShader(program, vsString.c_str(), GL_VERTEX_SHADER);
	addShader(program, fsString.c_str(), GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar errorLog[1024] = {0};

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", errorLog);
		exit(1);
	}

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", errorLog);
		exit(1);
	}
}

void GraphicsMgr::retrieveUniform(GLuint *handle, GLuint program, const char* name){
	glUseProgram(program);
	*handle = glGetUniformLocation(program, name);
	if(*handle == 0xFFFFFFFF)
		std::cout << "Couldn't locate uniform variable" << std::endl;
}

void GraphicsMgr::swapBuffers(){ glfwSwapBuffers(window); }

void GraphicsMgr::update(){
	glfwPollEvents();
}

void GraphicsMgr::stop(){ glfwTerminate(); }

Camera* GraphicsMgr::getCamera() { return &camera; }

void GraphicsMgr::drawDrawable(unsigned handle, Matrix transform/*=Matrix::genIdentity(4)*/, Color color/*=Color(1,1,1,1)*/){
	Drawable d = drawableMap[handle];
	if(!d.textured){
		glUseProgram(program1);

		Matrix camMat=camera.getMatrix();

		transform = camMat*transform;
		glUniformMatrix4fv(gTransformLoc1, 1, GL_TRUE, transform.getData());
		glUniform4fv(gColorLoc, 1, color.vals);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.IBO);

		glDrawElements(GL_TRIANGLES, d.indexCount, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
	} else {
		glUseProgram(program2);

		Matrix camMat=camera.getMatrix();

		transform = camMat*transform;
		glUniformMatrix4fv(gTransformLoc2, 1, GL_TRUE,transform.getData());

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, d.VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

		glEnableVertexAttribArray(1);
		glBindTexture(GL_TEXTURE_2D, d.tex);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.IBO);

		glDrawElements(GL_TRIANGLES, d.indexCount, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

void GraphicsMgr::genDrawable(unsigned *handle, std::vector<float> vertices, std::vector<int> indices){
	unsigned key;
	if(drawableMap.size() == 0){
		key = 0;
	} else {
		key = drawableMap.rbegin()->first+1;
	}

	Drawable out;
	out.textured = false;
	glGenBuffers(1, &out.VBO);
	glGenBuffers(1, &out.IBO);

	out.indexCount = indices.size();
	glBindBuffer(GL_ARRAY_BUFFER, out.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	*handle = key;
	drawableMap.insert(std::pair<unsigned, Drawable>(key, out));
}

void GraphicsMgr::genDrawable(unsigned *handle, std::vector<float> vertices, std::vector<int> indices, Image tex){
	unsigned key;
	if(drawableMap.size() == 0){
		key = 0;
	} else {
		key = drawableMap.rbegin()->first+1;
	}

	Drawable out;
	out.textured = true;
	glGenBuffers(1, &out.VBO);
	glGenBuffers(1, &out.IBO);
	glGenTextures(1, &out.tex);

	out.indexCount = indices.size();
	glBindBuffer(GL_ARRAY_BUFFER, out.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
	glBindTexture(GL_TEXTURE_2D, out.tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.data.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	*handle = key;
	drawableMap.insert(std::pair<unsigned, Drawable>(key, out));
}

void GraphicsMgr::delDrawable(unsigned handle){
	Drawable d = drawableMap[handle];
	glDeleteBuffers(1, &d.VBO);
	glDeleteBuffers(1, &d.IBO);
	if(d.textured) glDeleteTextures(1, &d.tex);

	drawableMap.erase(handle); //TODO doesn't recycle keys btw
}

void GraphicsMgr::addPipe(Pipe<KeyInputEvent> *pipe){
	keyInputPipes.push_back(pipe);
}
void GraphicsMgr::addPipe(Pipe<CharInputEvent> *pipe){
	charInputPipes.push_back(pipe);
}
void GraphicsMgr::addPipe(Pipe<CursorMoveEvent> *pipe){
	cursorMovePipes.push_back(pipe);
}
void GraphicsMgr::addPipe(Pipe<MouseButtonEvent> *pipe){
	mouseButtonPipes.push_back(pipe);
}
void GraphicsMgr::addPipe(Pipe<MouseScrollEvent> *pipe){
	mouseScrollPipes.push_back(pipe);
}

bool GraphicsMgr::active(){ return !glfwWindowShouldClose(window); }

GraphicsMgr::WindowDims GraphicsMgr::getWDims() {
	WindowDims dims;
	glfwGetFramebufferSize(window, &dims.wPixels, &dims.hPixels);

	GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *vidmode = glfwGetVideoMode(primaryMonitor);

	int wScreen, hScreen; // For some reason these are ints, despite being measured in mm
	int wWinScreen, hWinScreen; // My naming is terrible
	glfwGetWindowSize(window, &wWinScreen, &hWinScreen);
	glfwGetMonitorPhysicalSize(primaryMonitor, &wScreen, &hScreen);

	dims.wActual = wScreen * ((float)wWinScreen/vidmode->width);
	dims.hActual = hScreen * ((float)hWinScreen/vidmode->height);

	return dims;
}

