#include "ofEvents.h"


static ofEventArgs voidEventArgs;


//--------------------------------------
void ofSetFrameRate(int targetRate){
	//ofEvents().setFrameRate(targetRate);
}

//--------------------------------------
float ofGetFrameRate(){
	//return ofEvents().getFrameRate();
	return 0.f;
}

//--------------------------------------
float ofGetTargetFrameRate(){
	//return ofEvents().getTargetFrameRate();
	return 0;
}

//--------------------------------------
double ofGetLastFrameTime(){
	//return ofEvents().getLastFrameTime();
	return 0;
}

//--------------------------------------
uint64_t ofGetFrameNum(){
	//return ofEvents().getFrameNum();
	return 0.f;
}

//--------------------------------------
bool ofGetMousePressed(int button){ //by default any button
	//return ofEvents().getMousePressed(button);
	return false;
}

//--------------------------------------
bool ofGetKeyPressed(int key){
	//return ofEvents().getKeyPressed(key);
	return false;
}

//--------------------------------------
int ofGetMouseX(){
	//return ofEvents().getMouseX();
	return 0;
}

//--------------------------------------
int ofGetMouseY(){
	//return ofEvents().getMouseY();
	return 0;
}

//--------------------------------------
int ofGetPreviousMouseX(){
	//return ofEvents().getPreviousMouseX();
	return 0;
}

//--------------------------------------
int ofGetPreviousMouseY(){
	//return ofEvents().getPreviousMouseY();
	return 0;
}

ofCoreEvents::ofCoreEvents()
:targetRate(0)
,bFrameRateSet(false)
,fps(60)
,currentMouseX(0)
,currentMouseY(0)
,previousMouseX(0)
,previousMouseY(0)
,bPreMouseNotSet(false){

}

//------------------------------------------
void ofCoreEvents::disable(){
	setup.disable();
	draw.disable();
	update.disable();
	exit.disable();
	keyPressed.disable();
	keyReleased.disable();
	messageEvent.disable();
}

//------------------------------------------
void ofCoreEvents::enable(){
	setup.enable();
	draw.enable();
	update.enable();
	exit.enable();
	keyPressed.enable();
	keyReleased.enable();
	messageEvent.enable();
}

//--------------------------------------
void ofCoreEvents::setFrameRate(int _targetRate){
	// given this FPS, what is the amount of millis per frame
	// that should elapse?

	// --- > f / s

	if (_targetRate == 0){
		bFrameRateSet = false;
	}else{
		bFrameRateSet	= true;
		targetRate		= _targetRate;
		uint64_t nanosPerFrame = 1000000000.0 / (double)targetRate;
		timer.setPeriodicEvent(nanosPerFrame);
	}
}

//--------------------------------------
float ofCoreEvents::getFrameRate() const{
	return fps.getFps();
}

//--------------------------------------
float ofCoreEvents::getTargetFrameRate() const{
	return targetRate;
}

//--------------------------------------
double ofCoreEvents::getLastFrameTime() const{
	return fps.getLastFrameSecs();
}

//--------------------------------------
uint64_t ofCoreEvents::getFrameNum() const{
	return fps.getNumFrames();
}

//--------------------------------------
bool ofCoreEvents::getMousePressed(int button) const{ //by default any button
	if(button==-1) return pressedMouseButtons.size();
	return pressedMouseButtons.find(button)!=pressedMouseButtons.end();
}

//--------------------------------------
bool ofCoreEvents::getKeyPressed(int key) const{
	if(key==-1) return pressedKeys.size();
	return pressedKeys.find(key)!=pressedKeys.end();
}

//--------------------------------------
int ofCoreEvents::getMouseX() const{
	return currentMouseX;
}

//--------------------------------------
int ofCoreEvents::getMouseY() const{
	return currentMouseY;
}

//--------------------------------------
int ofCoreEvents::getPreviousMouseX() const{
	return previousMouseX;
}

//--------------------------------------
int ofCoreEvents::getPreviousMouseY() const{
	return previousMouseY;
}

//------------------------------------------
void ofCoreEvents::notifySetup(){
	ofNotifyEvent( setup, voidEventArgs );
}


//------------------------------------------
void ofCoreEvents::notifyUpdate(){
	ofNotifyEvent( update, voidEventArgs );
}

//------------------------------------------
void ofCoreEvents::notifyDraw(){
	ofNotifyEvent( draw, voidEventArgs );

	if (bFrameRateSet){
		timer.waitNext();
	}
	
	if(fps.getNumFrames()==0){
		if(bFrameRateSet) fps = ofFpsCounter(targetRate);
	}else{
		/*if(ofIsVerticalSyncEnabled()){
			float rate = ofGetRefreshRate();
			int intervals = round(lastFrameTime*rate/1000000.);//+vsyncedIntervalsRemainder;
			//vsyncedIntervalsRemainder = lastFrameTime*rate/1000000.+vsyncedIntervalsRemainder - intervals;
			lastFrameTime = intervals*1000000/rate;
		}*/
	}
	fps.newFrame();
	
}

//------------------------------------------
void ofCoreEvents::notifyKeyPressed(int key, int keycode, int scancode, int codepoint){
	// FIXME: modifiers are being reported twice, for generic and for left/right
	// add operators to the arguments class so it can be checked for both
    if(key == OF_KEY_RIGHT_CONTROL || key == OF_KEY_LEFT_CONTROL){
        pressedKeys.insert(OF_KEY_CONTROL);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_CONTROL);
        ofNotifyEvent( keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_RIGHT_SHIFT || key == OF_KEY_LEFT_SHIFT){
        pressedKeys.insert(OF_KEY_SHIFT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_SHIFT);
        ofNotifyEvent( keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_ALT || key == OF_KEY_RIGHT_ALT){
        pressedKeys.insert(OF_KEY_ALT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_ALT);
        ofNotifyEvent( keyPressed, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_SUPER || key == OF_KEY_RIGHT_SUPER){
        pressedKeys.insert(OF_KEY_SUPER);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,OF_KEY_SUPER);
        ofNotifyEvent( keyPressed, keyEventArgs );
    }
            
	pressedKeys.insert(key);
    ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Pressed,key,keycode,scancode,codepoint);
	ofNotifyEvent( keyPressed, keyEventArgs );
}

//------------------------------------------
void ofCoreEvents::notifyKeyReleased(int key, int keycode, int scancode, int codepoint){
	// FIXME: modifiers are being reported twice, for generic and for left/right
	// add operators to the arguments class so it can be checked for both
    if(key == OF_KEY_RIGHT_CONTROL || key == OF_KEY_LEFT_CONTROL){
        pressedKeys.erase(OF_KEY_CONTROL);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_CONTROL);
    	ofNotifyEvent( keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_RIGHT_SHIFT || key == OF_KEY_LEFT_SHIFT){
        pressedKeys.erase(OF_KEY_SHIFT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_SHIFT);
    	ofNotifyEvent( keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_ALT || key == OF_KEY_RIGHT_ALT){
        pressedKeys.erase(OF_KEY_ALT);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_ALT);
    	ofNotifyEvent( keyReleased, keyEventArgs );
    }
    else if(key == OF_KEY_LEFT_SUPER || key == OF_KEY_RIGHT_SUPER){
        pressedKeys.erase(OF_KEY_SUPER);
        ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,OF_KEY_SUPER);
    	ofNotifyEvent( keyReleased, keyEventArgs );
    }
    
	pressedKeys.erase(key);
    ofKeyEventArgs keyEventArgs(ofKeyEventArgs::Released,key,keycode,scancode,codepoint);
	ofNotifyEvent( keyReleased, keyEventArgs );
}


//------------------------------------------
void ofCoreEvents::notifyKeyEvent(const ofKeyEventArgs & keyEvent){
	switch(keyEvent.type){
		case ofKeyEventArgs::Pressed:
			notifyKeyPressed(keyEvent.key);
			break;
		case ofKeyEventArgs::Released:
			notifyKeyReleased(keyEvent.key);
			break;
		
	}
}

//------------------------------------------
void ofCoreEvents::notifyExit(){
	ofNotifyEvent( exit, voidEventArgs );
}

//------------------------------------------
void ofCoreEvents::notifyWindowResized(int width, int height){
	ofResizeEventArgs resizeEventArgs(width,height);
	ofNotifyEvent( windowResized, resizeEventArgs );
}

//------------------------------------------
void ofSendMessage(ofMessage msg){
	//ofNotifyEvent(ofEvents().messageEvent, msg);
}

//------------------------------------------
void ofSendMessage(string messageString){
	ofMessage msg(messageString);
	ofSendMessage(msg);
}
