#include "ofxComposition.h"

ofxComposition::ofxComposition() {
	hasStarted = false;
	titles.loadFont("verdana.ttf", 32);
}

ofxComposition::~ofxComposition() {
	// TODO(soyoung): figure out what to clean up

	// delete everything from videoQueue
	while (!videoQueue.empty()) {
		videoQueue.pop();
	}

	// delete everything from videoWrappers.
	videoWrappers.clear();
	titles.stop();
	titles.clear();
}

void ofxComposition::addVideo(ofxVideoPtr wrapper) {
	videoQueue.push(wrapper);
}

void ofxComposition::start() {
	int currentTime = time(NULL);
	compositionStartTime = currentTime;

	// if titles is empty, do nothing
	if(!titles.empty()) {
		titles.play();
	}

    hasStarted = true;
}

void ofxComposition::update() {
	if(!hasStarted) {
		start();
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
		videoPosition_t position = (*it)->getScreenPosition();

		int halfWidth = ofGetWindowWidth() / 2;
		int thirdHeight = ofGetWindowHeight() / 3;

		int x, y;
		int width = halfWidth;
		int height = thirdHeight;

		switch (position) {
			case hfPosition_t::HF_UPPER_LEFT:
				x = 0;
				y = 0;
				break;
			case hfPosition_t::HF_UPPER_RIGHT:
				x = halfWidth;
				y = 0;
				break;
			case hfPosition_t::HF_CENTER_LEFT:
				x = 0;
				y = thirdHeight;
				break;
			case hfPosition_t::HF_CENTER_RIGHT:
				x = halfWidth;
				y = thirdHeight;
				break;
			case hfPosition_t::HF_LOWER_LEFT:
				x = 0;
				y = thirdHeight * 2;
				break;
			case hfPosition_t::HF_LOWER_RIGHT:
				x = halfWidth;
				y = thirdHeight * 2;
				break;
			case hfPosition_t::HF_UPPER_THIRD:
				x = 0;
				y = 0;
				width = ofGetWindowWidth();
				height = thirdHeight;
				break;
			case hfPosition_t::HF_CENTER_THIRD:
				x = 0;
				y = thirdHeight;
				width = ofGetWindowWidth();
				height = thirdHeight;
				break;
			case hfPosition_t::HF_LOWER_THIRD:
				x = 0;
				y = thirdHeight * 2;
				width = ofGetWindowWidth();
				height = thirdHeight;
				break;
			case hfPosition_t::HF_LEFT_HALF:
				x = 0;
				y = 0;
				width = halfWidth;
				height = ofGetWindowHeight();
				break;
			case hfPosition_t::HF_RIGHT_HALF:
				x = halfWidth;
				y = 0;
				width = halfWidth;
				height = ofGetWindowHeight();
				break;
			case hfPosition_t::HF_FULL_SCREEN:
				x = 0;
				y = 0;
				width = ofGetWindowWidth();
				height = ofGetWindowHeight();
				break;
			default:
				cout << "osc receive::draw -- unknown video position: " << position << endl;;
				break;
		}

		VideoPlayerPtr((*it)->getVideoPlayer())->draw(x,y,width, height);
	}
}

void ofxComposition::drawToFbo(ofFbo* fbo) {
	fbo->begin();
	ofClear(0,0,0,0);
	draw();
	fbo->end();
}

void ofxComposition::drawSubtitles() {
	if(!titles.empty()) {
		titles.draw();
	}
}

bool ofxComposition::isDone() {
	return videoQueue.size() == 0 && videoWrappers.size() == 0;
}

// in our app, we may or may not bind to the videos.
// TODO(soyoung): try to get both working
void ofxComposition::addSubtitles(std::string _text, int _number, int _start_time, int _end_time, int _x, int _y) {
	titles.add(_text, _number, _start_time, _end_time, _x, _y);
}
