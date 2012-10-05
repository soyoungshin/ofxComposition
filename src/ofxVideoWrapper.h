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
#include "hfPosition.h"


#ifdef TARGET_WIN32
#include "ofxGstStandaloneVideoPlayer.h"
typedef ofxGstStandaloneVideoPlayer VideoWrapperPlayer;

#define USING_GST_PLAYER
#else
typedef ofVideoPlayer VideoWrapperPlayer;
#endif

typedef ofPtr<VideoWrapperPlayer> VideoPlayerPtr;
typedef vector<VideoPlayerPtr> VideoPlayers;

// hf == henry facade
// add positionings as necessary.


//using namespace std;

class ofxVideoWrapper
{
public:
	ofxVideoWrapper();
	~ofxVideoWrapper();

	void setup(string path, hfPosition_t::videoPosition screenPosition, int compositionStartTimecode, int compositionEndTimecode,
			   int clipStartTimecode, int clipDuration, int loopType);
	VideoPlayerPtr getVideoPlayer();
	void setPositionInSeconds(int timecode);
	void idle();
	bool isSetup();
	hfPosition_t::videoPosition getScreenPosition();
	float getVideoPositionInSeconds();
	int getCompositionStartTimecode();
	int getCompositionEndTimecode();
	void play();
	void stop();
private:
	VideoPlayerPtr vidPlayer;
	hfPosition_t::videoPosition screenPosition;
	int compositionStartTimecode, compositionEndTimecode, clipStartTimecode, clipDuration, loopType;

	bool bSetup;
};

#endif
