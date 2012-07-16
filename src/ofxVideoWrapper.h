/*
 *  ofxVideoWrapper.h
 *
 *  Created by Soyoung Shin on 4/18/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFXVIDEOWRAPPER_H
#define _OFXVIDEOWRAPPER_H

#include "ofVideoPlayer.h"

// hf == henry facade
enum e_videoPosition {
	HF_UPPER_LEFT, 
	HF_UPPER_RIGHT, 
	HF_CENTER_LEFT,
	HF_CENTER_RIGHT,
	HF_LOWER_LEFT,
	HF_LOWER_RIGHT,
	HF_FULL_SCREEN
};

using namespace std;


class ofxVideoWrapper
{
public:
	ofxVideoWrapper();
	~ofxVideoWrapper();
	
	void setup(string path, int screenPosition, int compositionStartTimecode, int compositionEndTimecode,
			   int clipStartTimecode, int clipDuration, int loopType);
	ofVideoPlayer* getVideoPlayer();
	void setPositionInSeconds(int timecode);
	void idle();
	bool isSetup();
	int getScreenPosition();
	int getVideoPositionInSeconds();
	int getCompositionStartTimecode();
	int getCompositionEndTimecode();
	void play();
	void stop();
	void setBirthTime(int birthTime);


private:
	ofVideoPlayer vidPlayer;
	int screenPosition, compositionStartTimecode, compositionEndTimecode, clipStartTimecode, clipDuration, loopType, birthTime;
	float frameRate;
	bool bSetup;
};
#endif
