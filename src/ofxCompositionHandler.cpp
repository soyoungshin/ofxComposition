/*
 *  ofxCompositionHandler.cpp
 *  osc receive
 *
 *  Created by Soyoung Shin on 4/30/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxCompositionHandler.h"

ofxCompositionHandler::~ofxCompositionHandler() {
	// this delete seems to barf on exit.
	delete &receiver;
	flush();
}

void ofxCompositionHandler::setup(int port) {
	cout << "listening for osc messages on port " << port << "\n";
	receiver.setup(port);
}

void ofxCompositionHandler::update() {

	// fetch all messages, place them in to a priority queue by their start time in the composition.
	// NOTE: this assumes all the messages for one composition are sent as a bundle, and there is
	//       only one bundle of composition-specific messages at a time.
	//		 this could be made more bomb proof by adding id's for the compositions on the sender side
	if(receiver.hasWaitingMessages()) {

		// Create a composition container, and fill it if the osc sent is to add videos.
		// NOTE: this might be inefficient.
		ofxCompositionPtr comp = ofxCompositionPtr(new ofxComposition());
		bool videosAdded = false;

		ofxOscMessage m;
		while( receiver.hasWaitingMessages() ) {
			receiver.getNextMessage( &m );

			// suuuuper basic validation.
			if ( m.getAddress() == "/addComposition" && m.getNumArgs() == 7) {
				videosAdded = true;
				ofxVideoPtr wrapper = ofxVideoPtr(new ofxVideoWrapper);

				// NOTE(soyoung): does it make sense to instantiate all the videos when the osc is received?
				//					It takes a while to allocate the memory + stuff.
				//					Might be a great use case for 'future'.
				wrapper->setup(m.getArgAsString(0), (hfPosition_t::videoPosition)m.getArgAsInt32(1), m.getArgAsInt32(2),
							   m.getArgAsInt32(3), m.getArgAsInt32(4), m.getArgAsInt32(5),
							   m.getArgAsInt32(6));
				comp->addVideo(wrapper);
			} else if(m.getAddress() == "/addSubtitle") {

				comp->addSubtitles(m.getArgAsString(0), 1,  m.getArgAsInt32(1), m.getArgAsInt32(2), m.getArgAsInt32(3), m.getArgAsInt32(4));
			} else if(m.getAddress() == "/flush") {
				flush();
			} else if(m.getAddress() == "/pop") {
				// pop the current composition off the stack, start playing the next one
				if(m.getNumArgs() > 0) {
					pop(m.getArgAsInt32(0));
				} else {
					pop(1);
				}
			} else {
				// unrecognized message
			}
		}

		if(videosAdded) {
			compositions.push_back(comp);
		}
	}

	// delete composition if it has finished.
	if(compositions.size() > 0 && compositions.front()->isDone()) {
		// TODO(soyoung): rather than deleting this immediately, possibly add it to a delete queue,
		//					or thread the deletion.
		compositions.erase(compositions.begin());
	}

	// update the playing composition.
	if(compositions.size() > 0) {
		compositions.front()->update();
	}
}

void ofxCompositionHandler::draw() {
	if(compositions.size() > 0) {
		compositions.front()->draw();
	}
}

void ofxCompositionHandler::drawToFbo(ofFbo* fbo) {
	if(compositions.size() > 0) {
		compositions.front()->drawToFbo(fbo);
	}
}

void ofxCompositionHandler::drawSubtitles() {
	if(compositions.size() > 0) {
		compositions.front()->drawSubtitles();
	}
}

void ofxCompositionHandler::pop(int numToPop = 1) {
	if(numToPop > 0) {
		int numActuallyPopped =  min(numToPop, (int)compositions.size());
		compositions.erase(compositions.begin(), compositions.begin() + numActuallyPopped);
	}
}

void ofxCompositionHandler::flush() {
	compositions.clear();
}
