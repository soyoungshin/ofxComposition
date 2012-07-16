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
	vidPlayer.stop();
	vidPlayer.close();
	//delete(vidPlayer);
}

void ofxVideoWrapper::setup(string path, int screenPosition, int compositionStartTimecode, int compositionEndTimecode,
							int clipStartTimecode, int clipDuration, int loopType) {
	vidPlayer.loadMovie(path);
	
	if(clipStartTimecode > vidPlayer.getDuration() || clipStartTimecode + clipDuration > vidPlayer.getDuration()) {
		// TODO: figure out how to handle this.
	}
	
	// this is precluded on the fact that the clip will stay in bounds.
	setPositionInSeconds(clipStartTimecode);
	
	
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
	this->frameRate = vidPlayer.getTotalNumFrames() / vidPlayer.getDuration();
}

ofVideoPlayer* ofxVideoWrapper::getVideoPlayer() {
	return &vidPlayer;
}

void ofxVideoWrapper::setPositionInSeconds(int timecode) {
	float idx = min(timecode * frameRate, 1.0f);
	this->vidPlayer.setPosition(idx);
}

// the ofx documentation lies. vidPlayer.getPosition() returns a 0.0-1.0 idx in to the video.
int ofxVideoWrapper::getVideoPositionInSeconds() {
	return vidPlayer.getCurrentFrame() / frameRate;

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
	vidPlayer.play();
}

void ofxVideoWrapper::stop() {
	vidPlayer.stop();
}

void ofxVideoWrapper::setBirthTime(int birthTime) {
	this->birthTime = birthTime;
}

void ofxVideoWrapper::idle() {
	int clipEndTimecode = clipDuration + clipStartTimecode;
	
	if(clipEndTimecode < getVideoPositionInSeconds()) {
		if (loopType == OF_LOOP_NONE) {
			vidPlayer.setPaused(true);
		} else if (loopType == OF_LOOP_PALINDROME) {
			// TODO: decide whether this is worth implementing.
		} else {
			// loop type is normal, or unknown loop type.
			setPositionInSeconds(clipStartTimecode);
		}
	}
	vidPlayer.idleMovie();
}
