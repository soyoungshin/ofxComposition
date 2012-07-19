/*
 *  ofxCompositionHandler.h
 *  osc receive
 *
 *  Created by Soyoung Shin on 4/30/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFXCOMPOSITIONHANDLER_H
#define _OFXCOMPOSITIONHANDLER_H

#include "ofxComposition.h"
#include <vector>

// compositon handler
typedef ofPtr<ofxComposition> ofxCompositionPtr;
typedef vector<ofxCompositionPtr> compositionVec;

class ofxCompositionHandler {
public:
	ofxCompositionHandler() {};
	~ofxCompositionHandler();
	void setup(int port);
	void update();
	void draw();
	void drawToFbo(ofFbo* fbo);
	void pop(int numToPop);
	void flush();
private:
	ofxOscReceiver receiver;
	compositionVec compositions;
};

#endif