#include "ofxComposition.h"

ofxComposition::ofxComposition() {
	hasStarted = false;
}

ofxComposition::~ofxComposition() {
	// TODO(soyoung): figure out what to clean up
	
	// delete everything from videoQueue
	while (!videoQueue.empty()) {
		videoQueue.pop();
	}

	// delete everything from videoWrappers.
	videoWrappers.clear();
	
	ofSetFrameRate(10);
	
}

void ofxComposition::addVideo(ofxVideoPtr wrapper) {
	videoQueue.push(wrapper);
}

void ofxComposition::start() {
	int currentTime = time(NULL);
	compositionStartTime = currentTime;
	
	vector<ofxVideoPtr>::iterator it;
	for ( it=videoWrappers.begin() ; it < videoWrappers.end(); ) {
		(*it)->setBirthTime(currentTime);
	}
}

void ofxComposition::update() {
	if(!hasStarted) {
		start();
		hasStarted = true;
	}
	
	int currentTime = time(NULL);

	// dequeue current videos from priority queue
	int compositionElapsed = currentTime - compositionStartTime;
	
	while(!videoQueue.empty()) {
		ofxVideoPtr p = videoQueue.top();
		if(p->getCompositionStartTimecode() <= compositionElapsed) {
			p->play();
			videoWrappers.push_back(p);
			videoQueue.pop();
		} else {
			break;
		}
		
	}
	
	// prune old videos from current-play vector
	vector<ofxVideoPtr>::iterator it;
	for ( it=videoWrappers.begin() ; it < videoWrappers.end(); ) {
		if((*it)->getCompositionEndTimecode() <= compositionElapsed) {
			(*it)->stop();
			it = videoWrappers.erase(it);
		} else {
			it++;
		}
	}
	
	// idle current videos
	for ( it=videoWrappers.begin() ; it < videoWrappers.end(); it++ ) {
		(*it)->idle();
	}	
}

void ofxComposition::draw() {
	vector<ofxVideoPtr>::iterator it;
	for ( it=videoWrappers.begin() ; it < videoWrappers.end(); it++ ) {
		int position = (*it)->getScreenPosition();
		

		
		int halfWidth = ofGetWindowWidth() / 2;
		int thirdHeight = ofGetWindowHeight() / 3;
		
		int x, y;
		int width = halfWidth;
		int height = thirdHeight;
		
		switch (position) {
			case HF_UPPER_LEFT:
				x = 0;
				y = 0;
				break;
			case HF_UPPER_RIGHT:
				x = halfWidth;
				y = 0;
				break;
			case HF_CENTER_LEFT:
				x = 0;
				y = thirdHeight;
				break;
			case HF_CENTER_RIGHT:
				x = halfWidth;
				y = thirdHeight;
				break;
			case HF_LOWER_LEFT:
				x = 0;
				y = thirdHeight * 2;
				break;
			case HF_LOWER_RIGHT:
				x = halfWidth;
				y = thirdHeight * 2;
				break;
			case HF_FULL_SCREEN:
				x = 0;
				y = 0;
				width = ofGetWindowWidth();
				height = ofGetWindowHeight();
				break;
			default:
				cout << "osc receive::draw -- unknown video position: " << position << endl;;
				break;
		}
		
		(*(*it)->getVideoPlayer()).draw(x, y, width, height);
	}
}

bool ofxComposition::isDone() {
	return videoQueue.size() == 0 && videoWrappers.size() == 0;
}