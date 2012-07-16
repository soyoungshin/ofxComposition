/*
 *  ofxComposition.h
 *
 *  Created by Soyoung Shin on 4/18/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFXCOMPOSITION_H
#define _OFXCOMPOSITION_H
#define COMPOSITION_PORT 12345

#include "ofxVideoWrapper.h"
#include "ofxOsc.h"

// compositions
typedef ofPtr<ofxVideoWrapper> ofxVideoPtr;
typedef vector<ofxVideoPtr> videoWrappersVec;

class CompareVideoWrappers {
public:
    bool operator()(ofxVideoPtr& lhs, ofxVideoPtr& rhs)
    {
		// comparator such that we have a min heap
		if (lhs->getCompositionStartTimecode() > rhs->getCompositionStartTimecode()) {
			return true;
		} else {
			return false;
		}
    }
};

class ofxComposition {

public:
	ofxComposition();
	~ofxComposition();
	void addVideo(ofxVideoPtr wrapper);
	void start();
	void update();
	void draw();
	bool isDone();
private:
	// current video list
	videoWrappersVec videoWrappers;
	
	// enqueued videos
	// NOTE: this isn't the most efficient storage type.
	priority_queue<ofxVideoPtr, vector<ofxVideoPtr>, CompareVideoWrappers> videoQueue;
	
	// epoch time at which this composition started.
	int compositionStartTime;
	bool hasStarted;
};

#endif