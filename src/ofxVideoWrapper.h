/*
 *  ofxVideoWrapper.h
 *
 *  Created by Soyoung Shin on 4/18/2012.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFXVIDEOWRAPPER_H
#define _OFXVIDEOWRAPPER_H

#include "hfGstVideoPlayer.h"

typedef ofPtr<hfGstVideoPlayer> VideoPlayerPtr;
typedef vector<VideoPlayerPtr> VideoPlayers;

// hf == henry facade
// add positionings as necessary.
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
	VideoPlayerPtr getVideoPlayer();
	void setPositionInSeconds(int timecode);
	void idle();
	bool isSetup();
	int getScreenPosition();
	float getVideoPositionInSeconds();
	int getCompositionStartTimecode();
	int getCompositionEndTimecode();
	void play();
	void stop();
	void setBirthTime(int birthTime);

private:
	VideoPlayerPtr vidPlayer;
	int screenPosition, compositionStartTimecode, compositionEndTimecode, clipStartTimecode, clipDuration, loopType, birthTime;
	bool bSetup;
};

#endif
