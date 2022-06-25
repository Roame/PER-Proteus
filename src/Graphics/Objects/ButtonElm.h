#ifndef GRAPHICS_OBJECTS_BUTTONELM_H_
#define GRAPHICS_OBJECTS_BUTTONELM_H_

#include "Element.h"
#include "Shapes.h"
#include "Graphics/GraphicsMgr.h"
#include "Graphics/Color.h"
#include "Control/Pipe.h"
#include "Control/Collisions.h"
#include "Control/EdgeDetector.h"

class ButtonElm : public Element {
private:
	Rectangle rect;
	Color cColor, cIdle, cHover, cPress;
	Pipe<CursorMoveEvent> cursorPipe;
	Pipe<MouseButtonEvent> buttonPipe;
	bool isOnBtn = false, isClicking=false, clicked = false;
	EdgeDetector clickDetector, relDetector = EdgeDetector(false);
//	bool toggleable=false; // TODO maybe add in this functionality

public:
	ButtonElm(Rectangle rectangle, Color color)
	: rect(rectangle), cColor(color), cIdle(color){
		GraphicsMgr::getInstance().addPipe(&cursorPipe);
		GraphicsMgr::getInstance().addPipe(&buttonPipe);
	}

	void draw(){
		if(!cursorPipe.isEmpty()){
			CursorMoveEvent latestCursor;
			while(!cursorPipe.isEmpty()){
				latestCursor = cursorPipe.next();
			}
			Vec2 mPos = Vec2((latestCursor.xPos-320)/320*2, -(latestCursor.yPos-240)/240*1.5);
			isOnBtn = Collisions::checkColl(mPos, rect);
		}

		if(!buttonPipe.isEmpty()){
			MouseButtonEvent mBtnEvent;
			while(!buttonPipe.isEmpty()){
				mBtnEvent=buttonPipe.next();
			}
			if(mBtnEvent.button == 0){
				isClicking = mBtnEvent.action;
			}
		}

		if(clickDetector.update(isClicking) && isOnBtn){
			clicked = true;
		}
		if(relDetector.update(isClicking) && isOnBtn){
			clicked = false;
		}

		if(isOnBtn){
			if(clicked){
				cColor = cPress;
			} else {
				cColor = cHover;
			}
		} else {
			cColor = cIdle;
			clicked = false;
		}

		drawShape(&rect, cColor);
	}

	void setIdleColor(Color color){ cIdle = color; }
	void setHoverColor(Color color){ cHover = color; }
	void setPressColor(Color color) { cPress = color; }

	bool isClicked(){ return clicked; }
};


#endif
