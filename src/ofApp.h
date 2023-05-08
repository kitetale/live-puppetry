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
    //ofSpherePrimitive s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13,s14,s15,s16,s17,s18,s19,s20;
    vector<ofSpherePrimitive> handSpheres;
    ofLight light;
    
    string curFileInfo;
    
    ofEasyCam cam;
    float cameraOrbit;
    
    ofxOscReceiver osc;
    
    //hand : https://developers.google.com/mediapipe/solutions/vision/hand_landmarker
    float oscx = 0.0; float oscy = 0.0; //wrist
    float x1,y1,x2,y2,x3,y3,x4,y4; //thumb
    float x5,y5,x6,y6,x7,y7,x8,y8; //index
    float x9,y9,x10,y10,x11,y11,x12,y12; //middle
    float x13,y13,x14,y14,x15,y15,x16,y16; //ring
    float x17,y17,x18,y18,x19,y19,x20,y20; //pinky
    vector<float> handXs, handYs;
    
    float w, h;
    
    ofxFloatSlider rad, deform, deformFreq, extrude;
    ofxFloatSlider camXPos, camYPos, camZPos;
    ofParameter<bool> showHand = {"Show Hand", true};
    ofxPanel gui;
        
};
