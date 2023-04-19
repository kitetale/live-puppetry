#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxMovenet.h"

using namespace ofxCv;
using namespace cv;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    ofxKinect kinect;
    ofxCvColorImage colorImage;
    ofImage imgDiff;
    ofPixels prevPx;
    ofPixels imgPx;
    ofxCvGrayscaleImage grayImage, grayBg, grayDiff;
    ofxCvContourFinder contourFinder;
    int grayThreshold;
    bool learnBg;
    ofImage output; //ofImage for saving to os
    
    ofTrueTypeFont font;
    
    int h,w;
    int nearClip,farClip;
    int angle;

    
    // TensorFlow ML model for Skeleton
    ofxMovenet movenet;
    // neural net input size
    std::size_t nnWidth = 640; //640
    std::size_t nnHeight = 480; //480
};
