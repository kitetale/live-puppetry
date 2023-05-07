#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "Thinning.h"
#include "Contour.h"

using namespace ofxCv;
using namespace cv;

#define USE_VIDEO

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
        void reconstituteBlobsFromContours(vector<vector<cv::Point>> &contours, int w, int h);

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
    
    ofVideoPlayer video;
    ofxKinect kinect;
    
    int w,h;
    float scale;
    int nearClip,farClip;
    int angle;
    
    
    float minContourSize, maxContourSize;
    int depthDilate; // num of dilates applied to depth img
    int depthErode; // num of erode applied to depth img
    int depthBlur; // amount of blur applied to depth img
    
    ofImage depthImage, depthOriginal, bgOriginal;
    ofxCvGrayscaleImage grayDiff, grayDepthImage, bgImage;
    ofImage grayThreshNear;
    ofImage grayThreshFar;
    int grayThreshold;
    
    bool learnBg;
    
    // for adjusting cam view area, if needed
    Mat depthCroppingMask;
    float depthLeftMask, depthRightMask;
    float depthTopMask, depthBottomMask;
    
    ContourFinder contourFinder;
    Contour contour;
    int minBlobSize;
    int maxBlobSize;
    int contourThickness;
    int buffW, buffH;
    
    Mat filledContourMat;
    ofImage filledContourImg, skeletonImg;
    ofFbo fbo;
    vector<vector<cv::Point>> theContoursi;
    
    ofxCvColorImage inputImg;
    
    Thinning skeletonizer;
};
