#ifndef GRAPHICSMGR_H_
#define GRAPHICSMGR_H_

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <map>

#include "Transform3d.h"
#include "Math/Matrix.h"
#include "Color.h"
#include "FileHandling/Image.h"
#include "FileHandling/TxtEditor.h"
#include "Camera.h"
#include "Control/Pipe.h"

enum EventType{
	KEY_INPUT,
	CHAR_INPUT,
	CURSOR_MOVE,
	MOUSE_BUTTON,
	MOUSE_SCROLL
};

struct KeyInputEvent {
	int key, scancode, action, mods;
};

struct CharInputEvent {
	unsigned codepoint;
};

struct CursorMoveEvent {
	double xPos, yPos;
};

struct MouseButtonEvent {
	int button, action, mods;
};

struct MouseScrollEvent {
	double xOffset, yOffset;
};

struct Drawable {
	bool textured{false};
	unsigned indexCount{};
	GLuint VBO{}, IBO{}, tex{};

	Drawable() = default;
	~Drawable() = default;
};

class GraphicsMgr {
private:
	GLFWwindow *window;
	GLuint program1, program2;
	GLuint gTransformLoc1, gTransformLoc2, gColorLoc;
	std::map<unsigned, Drawable> drawableMap;
	Camera camera;

	// I hate this. I hate all of this
	static std::vector<Pipe<KeyInputEvent>*> keyInputPipes;
	static std::vector<Pipe<CharInputEvent>*> charInputPipes;
	static std::vector<Pipe<CursorMoveEvent>*> cursorMovePipes;
	static std::vector<Pipe<MouseButtonEvent>*> mouseButtonPipes;
	static std::vector<Pipe<MouseScrollEvent>*> mouseScrollPipes;

	GraphicsMgr();

	static void errorCB(int error, const char *description);
	static void keyCB(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void charCB(GLFWwindow *window, unsigned codepoint);
	static void cursorPosCB(GLFWwindow *window, double xPos, double yPos);
	static void mouseButtonCB(GLFWwindow *window, int button, int action, int mods);
	static void scrollCB(GLFWwindow *window, double xOffset, double yOffset);

	void addShader(GLuint program, const char *shaderText, GLenum shaderType);
	void createProgram(GLuint *handle, const char *vs, const char *fs);
	void retrieveUniform(GLuint *handle, GLuint program, const char* name);

public:
	static GraphicsMgr& getInstance(){
		static GraphicsMgr instance;
		return instance;
	}

	struct WindowDims {
		int wPixels, hPixels;
		float wActual, hActual; // Going to do this in mm
	};

	GraphicsMgr(GraphicsMgr const&) = delete;
	void operator=(GraphicsMgr const&) = delete;

	~GraphicsMgr(){}

	void swapBuffers();
	void update();
	void stop();

	Camera* getCamera();

	void drawDrawable(unsigned handle, Matrix transform=Matrix::genIdentity(4), Color color=Color(1,1,1,1));
	void genDrawable(unsigned *handle, std::vector<float> vertices, std::vector<int> indices);
	void genDrawable(unsigned *handle, std::vector<float> vertices, std::vector<int> indices, Image tex);
	void delDrawable(unsigned handle);

	void addPipe(Pipe<KeyInputEvent> *pipe);
	void addPipe(Pipe<CharInputEvent> *pipe);
	void addPipe(Pipe<CursorMoveEvent> *pipe);
	void addPipe(Pipe<MouseButtonEvent> *pipe);
	void addPipe(Pipe<MouseScrollEvent> *pipe);

	bool active();

	WindowDims getWDims();
};

#endif
