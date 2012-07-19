/*
 *  ofxVideoWrapper.cpp
 *
 *  Created by Soyoung Shin on 4/18/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxVideoWrapper.h"

ofxVideoWrapper::ofxVideoWrapper() {
	bSetup = false;
}

ofxVideoWrapper::~ofxVideoWrapper() {
}

void ofxVideoWrapper::setup(string path, int screenPosition, 
	int compositionStartTimecode, int compositionEndTimecode,
	int clipStartTimecode, int clipDuration, int loopType) {

	vidPlayer = ofPtr<hfGstVideoPlayer>(new hfGstVideoPlayer);
	vidPlayer->setPixelFormat(OF_PIXELS_RGB);

	std::string file = ofFilePath::getAbsolutePath(path, true);
	std::replace(file.begin(), file.end(), '\\', '/');

	// prepend the protocol
	file = "file:///" + file;

	if(!vidPlayer->loadMovie(file)) {
		ofLogError("setup", "movie loading failed: \n" + file);
	}

	if(clipStartTimecode > vidPlayer->getDuration() || clipStartTimecode + clipDuration > vidPlayer->getDuration()) {
		// times supplied for for clip start/stop are out of bounds.
		// TODO: figure out how to handle this out of bounds error.
	}
	
	// this is precluded on the fact that the clip will stay in bounds.
	setPositionInSeconds(clipStartTimecode);
	vidPlayer->setVolume(0);
	
	if(loopType != OF_LOOP_NONE && loopType != OF_LOOP_NORMAL && loopType != OF_LOOP_PALINDROME) {
		cout << "warning: loop type unknown. loop type: " << loopType << endl;
	}
	
	// TODO: validate these arguments. ensure are positive, etc.	
	this->screenPosition = screenPosition;
	this->compositionStartTimecode = compositionStartTimecode;
	this->compositionEndTimecode = compositionEndTimecode;
	this->clipStartTimecode = clipStartTimecode;
	this->clipDuration = clipDuration;
	this->loopType = loopType;
	this->bSetup = true;
}

// Caller has to immediately wrap returned value in an ofPtr.
VideoPlayerPtr ofxVideoWrapper::getVideoPlayer() {
	return vidPlayer;
}

void ofxVideoWrapper::setPositionInSeconds(int timecode) {
	// convert time in seconds to 0.0f - 1.0f.
	float idx = min(timecode / vidPlayer->getDuration(), 1.0f);
	vidPlayer->setPosition(idx);
}

// the ofx documentation lies. 
// vidPlayer.getPosition() returns a 0.0-1.0 idx in to the video.
float ofxVideoWrapper::getVideoPositionInSeconds() {
	return vidPlayer->getPosition() * vidPlayer->getDuration();
}

int ofxVideoWrapper::getCompositionStartTimecode() {
	return compositionStartTimecode;
}

int ofxVideoWrapper::getCompositionEndTimecode() {
	return compositionEndTimecode;
}

bool ofxVideoWrapper::isSetup() {
	return bSetup;
}

int ofxVideoWrapper::getScreenPosition() {
	return screenPosition;
}

void ofxVideoWrapper::play() {
	vidPlayer->play();
}

void ofxVideoWrapper::stop() {
	vidPlayer->stop();
}

void ofxVideoWrapper::setBirthTime(int birthTime) {
	this->birthTime = birthTime;
}

void ofxVideoWrapper::idle() {
	int clipEndTimecode = clipDuration + clipStartTimecode;
	if(clipEndTimecode < getVideoPositionInSeconds()) {
		if (loopType == OF_LOOP_NONE) {
			vidPlayer->setPaused(true);
		} else if (loopType == OF_LOOP_PALINDROME) {
			// TODO: decide whether this is worth implementing.
		} else {
			// loop type is normal, or unknown loop type.
			setPositionInSeconds(clipStartTimecode);
		}
	}
	vidPlayer->update();
}
