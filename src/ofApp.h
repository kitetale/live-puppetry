#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"

#define PORT 9527

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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
		
    ofxAssimpModelLoader model;
    
    ofSpherePrimitive sphere;
    vector<ofSpherePrimitive> handSpheres;
    //ofSpherePrimitive head, legL, legR, eyeL, eyeR, nose;
    
    bool handClosed;
    bool thumbSpread;
    bool pinkySpread;
    float closedAmount; //closed 0~1 open
    float thumbSpreadAmount; //spread 0~1 closed
    float pinkySpreadAmount; //spread 0~1 closed
    
    float zScale;
    
    float rotDeg;
    
    ofLight light, light2, light3, light4;
    
    string curFileInfo;
    
    ofEasyCam cam;
    float cameraOrbit;
    
    ofxOscReceiver osc;
    
    //hand : https://developers.google.com/mediapipe/solutions/vision/hand_landmarker
    float oscx = 0.0; float oscy = 0.0; //wrist
    vector<float> handXs, handYs;
    bool handOnScreen;
    
    float w, h;
    
    ofxFloatSlider camXPos, camYPos, camZPos;
    ofParameter<bool> showHand = {"Show Hand", false};
    ofxPanel gui;
        
};
