#pragma once

#include "ofConstants.h"
#include "ofEventUtils.h"
#include "ofTimer.h"
#include "ofFpsCounter.h"
#include <set>

//-------------------------- mouse/key query
bool ofGetMousePressed(int button=-1); //by default any button
bool ofGetKeyPressed(int key=-1); //by default any key

int	ofGetMouseX();
int	ofGetMouseY();

int	ofGetPreviousMouseX();
int	ofGetPreviousMouseY();


//-----------------------------------------------
// event arguments, this are used in oF to pass
// the data when notifying events

class ofEventArgs{};

class ofKeyEventArgs : public ofEventArgs {
public:
	enum Type{
		Pressed,
		Released,
	};

	ofKeyEventArgs()
  	:type(Pressed)
  	,key(0)
	,keycode(0)
	,scancode(0)
	,codepoint(0){}

	ofKeyEventArgs(Type type, int key, int keycode, int scancode, unsigned int codepoint)
	:type(type)
	,key(key)
	,keycode(keycode)
	,scancode(scancode)
	,codepoint(codepoint){

	}

	ofKeyEventArgs(Type type, int key)
	:type(type)
	,key(key)
	,keycode(0)
	,scancode(0)
	,codepoint(0){

	}

	Type type;
	/// \brief For special keys, one of OF_KEY_* (@see ofConstants.h). For all other keys, the Unicode code point you'd expect if this key combo (including modifier keys that may be down) was pressed in a text editor (same as codepoint). 
	int key; 
	/// \brief The keycode returned by the windowing system, independent of any modifier keys or keyboard layout settings. For ofAppGLFWWindow this value is one of GLFW_KEY_* (@see glfw3.h) - typically, ASCII representation of the symbol on the physical key, so A key always returns 0x41 even if shift, alt, ctrl are down. 
	int keycode;
	/// \brief The raw scan code returned by the keyboard, OS and hardware specific. 
	int scancode;
	/// \brief The Unicode code point you'd expect if this key combo (including modifier keys) was pressed in a text editor, or -1 for non-printable characters. 
	unsigned int codepoint;
};

class ofResizeEventArgs : public ofEventArgs {
public:
	ofResizeEventArgs()
  	:width(0)
	,height(0){}

	ofResizeEventArgs(int width, int height)
	:width(width)
	,height(height){}

	int width;
	int height;
};

class ofMessage : public ofEventArgs{
	public:
		ofMessage( string msg ){
			message = msg;
		}
		string message;
};
		

class ofCoreEvents {
  public:
	ofCoreEvents();
	ofEvent<ofEventArgs> 		setup;
	ofEvent<ofEventArgs> 		update;
	ofEvent<ofEventArgs> 		draw;
	ofEvent<ofEventArgs> 		exit;

	ofEvent<ofResizeEventArgs> 	windowResized;

	ofEvent<ofKeyEventArgs> 	keyPressed;
	ofEvent<ofKeyEventArgs> 	keyReleased;

	ofEvent<ofMessage>			messageEvent;

	void disable();
	void enable();

	void setFrameRate(int _targetRate);
	float getFrameRate() const;
	float getTargetFrameRate() const;
	double getLastFrameTime() const;
	uint64_t getFrameNum() const;

	bool getMousePressed(int button=-1) const;
	bool getKeyPressed(int key=-1) const;
	int getMouseX() const;
	int getMouseY() const;
	int getPreviousMouseX() const;
	int getPreviousMouseY() const;

	//  event notification only for internal OF use
	void notifySetup();
	void notifyUpdate();
	void notifyDraw();

	void notifyKeyPressed(int key, int keycode=-1, int scancode=-1, int codepoint=-1);
	void notifyKeyReleased(int key, int keycode=-1, int scancode=-1, int codepoint=-1);
	void notifyKeyEvent(const ofKeyEventArgs & keyEvent);

	void notifyExit();
	void notifyWindowResized(int width, int height);

private:
	float targetRate;
	bool bFrameRateSet;
	ofTimer timer;
	ofFpsCounter fps;

	int	currentMouseX, currentMouseY;
	int	previousMouseX, previousMouseY;
	bool bPreMouseNotSet;
	set<int> pressedMouseButtons;
	set<int> pressedKeys;
};

void ofSendMessage(ofMessage msg);
void ofSendMessage(string messageString);

ofCoreEvents & ofEvents();

template<class ListenerClass>
void ofRegisterMouseEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().mouseDragged,listener,&ListenerClass::mouseDragged,prio);
	ofAddListener(ofEvents().mouseMoved,listener,&ListenerClass::mouseMoved,prio);
	ofAddListener(ofEvents().mousePressed,listener,&ListenerClass::mousePressed,prio);
	ofAddListener(ofEvents().mouseReleased,listener,&ListenerClass::mouseReleased,prio);
	ofAddListener(ofEvents().mouseScrolled,listener,&ListenerClass::mouseScrolled,prio);
	ofAddListener(ofEvents().mouseEntered,listener,&ListenerClass::mouseEntered,prio);
	ofAddListener(ofEvents().mouseExited,listener,&ListenerClass::mouseExited,prio);
}

template<class ListenerClass>
void ofRegisterKeyEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().keyPressed, listener, &ListenerClass::keyPressed,prio);
	ofAddListener(ofEvents().keyReleased, listener, &ListenerClass::keyReleased,prio);
}

template<class ListenerClass>
void ofRegisterTouchEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().touchDoubleTap, listener, &ListenerClass::touchDoubleTap,prio);
	ofAddListener(ofEvents().touchDown, listener, &ListenerClass::touchDown,prio);
	ofAddListener(ofEvents().touchMoved, listener, &ListenerClass::touchMoved,prio);
	ofAddListener(ofEvents().touchUp, listener, &ListenerClass::touchUp,prio);
	ofAddListener(ofEvents().touchCancelled, listener, &ListenerClass::touchCancelled,prio);
}

template<class ListenerClass>
void ofRegisterGetMessages(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage,prio);
}

template<class ListenerClass>
void ofRegisterDragEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofAddListener(ofEvents().fileDragEvent, listener, &ListenerClass::dragEvent,prio);
}

template<class ListenerClass>
void ofUnregisterMouseEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().mouseDragged,listener,&ListenerClass::mouseDragged,prio);
	ofRemoveListener(ofEvents().mouseMoved,listener,&ListenerClass::mouseMoved,prio);
	ofRemoveListener(ofEvents().mousePressed,listener,&ListenerClass::mousePressed,prio);
	ofRemoveListener(ofEvents().mouseReleased,listener,&ListenerClass::mouseReleased,prio);
	ofRemoveListener(ofEvents().mouseScrolled,listener,&ListenerClass::mouseScrolled,prio);
	ofRemoveListener(ofEvents().mouseEntered,listener,&ListenerClass::mouseEntered,prio);
	ofRemoveListener(ofEvents().mouseExited,listener,&ListenerClass::mouseExited,prio);
}

template<class ListenerClass>
void ofUnregisterKeyEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().keyPressed, listener, &ListenerClass::keyPressed,prio);
	ofRemoveListener(ofEvents().keyReleased, listener, &ListenerClass::keyReleased,prio);
}

template<class ListenerClass>
void ofUnregisterTouchEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().touchDoubleTap, listener, &ListenerClass::touchDoubleTap,prio);
	ofRemoveListener(ofEvents().touchDown, listener, &ListenerClass::touchDown,prio);
	ofRemoveListener(ofEvents().touchMoved, listener, &ListenerClass::touchMoved,prio);
	ofRemoveListener(ofEvents().touchUp, listener, &ListenerClass::touchUp,prio);
	ofRemoveListener(ofEvents().touchCancelled, listener, &ListenerClass::touchCancelled,prio);
}

template<class ListenerClass>
void ofUnregisterGetMessages(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().messageEvent, listener, &ListenerClass::gotMessage,prio);
}

template<class ListenerClass>
void ofUnregisterDragEvents(ListenerClass * listener, int prio=OF_EVENT_ORDER_AFTER_APP){
	ofRemoveListener(ofEvents().fileDragEvent, listener, &ListenerClass::dragEvent,prio);
}
