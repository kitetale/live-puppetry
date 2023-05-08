#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(150);
    
    ofSetVerticalSync(true);
    gui.setup();
    gui.add(rad.setup("rad", 250, 0, 500));
    gui.add(deform.setup("deform", 0.3, 0, 1.5));
    gui.add(deformFreq.setup("deformFreq", 3, 0, 10));
    gui.add(extrude.setup("extrude", 1, 0, 1 ));
    gui.add(camXPos.setup("cam x pos", 500, 0, 1000));
    gui.add(camYPos.setup("cam y pos",395, -500, 500));
    gui.add(camZPos.setup("cam z pos", 0, -500, 500));
    gui.add(showHand);
    light.setPosition(0, 0, 500);
    
    cameraOrbit = 0;
    cam.setDistance(750);
    
    osc.setup(PORT);
    
    head.setRadius(100);
    legL.setRadius(20);
    legR.setRadius(20);
    eyeL.setRadius(7);
    eyeR.setRadius(7);
    nose.setRadius(60);
    
    sphere.setRadius(20);
    handSpheres.push_back(sphere); //wrist
    handXs.push_back(0);
    handYs.push_back(0);
    for (int i=0; i<20; ++i){
        ofSpherePrimitive s;
        s.setRadius(10);
        handSpheres.push_back(s); //hand
        handXs.push_back(0);
        handYs.push_back(0);
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    while(osc.hasWaitingMessages()){
        ofxOscMessage m;
        osc.getNextMessage(&m);
        
        //pose comes to poses/arr
        //hand comes to hands/arr
        if(m.getAddress() == "/hands/arr"){
            //https://developers.google.com/mediapipe/solutions/vision/hand_landmarker
            //w, h, numHands, score, [(x,y,score),..]
            w = m.getArgAsFloat(0);
            h = m.getArgAsFloat(1);
            //writst xy
            oscx = m.getArgAsFloat(4);
            oscy = m.getArgAsFloat(5);
            
            for (int i=0; i<21; ++i){
                handXs[i] = m.getArgAsFloat(4+i*3);
                handYs[i] = m.getArgAsFloat(5+i*3);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    light.enable();
    cam.begin();
    
    head.setPosition(0,0,0);
    legL.setPosition(35,-20,0);
    legR.setPosition(-35,-20,0);
    eyeL.setPosition(20, 0, 80);
    eyeR.setPosition(-20, 0, 80);
    nose.setPosition(0,-70,40);
    
    ofPushMatrix();
        ofTranslate(-100,-100);
        ofPushStyle();
        ofSetColor(30, 30, 50);
            ofPushMatrix();
                ofScale(1.3,1,0.7);
                head.draw();
            ofPopMatrix();
        
            ofPushMatrix();
                ofScale(0.4,4,1);
                legL.draw();
                legR.draw();
            ofPopMatrix();
        ofPopStyle();
        ofPushStyle();
        ofSetColor(0,128,128);
            eyeL.draw();
            eyeR.draw();
            ofPushMatrix();
                ofRotateDeg(-30, 10, 0, 0);
                ofScale(1.7,0.8,0.3);
                nose.draw();
            ofPopMatrix();
        ofPopStyle();
    
    ofPopMatrix();
    
    
    
    //debug
    if (showHand){
        ofPushMatrix();
        ofPushStyle();
        ofSetColor(255, 255, 0);
        ofTranslate(-oscx*ofGetWidth()/w+camXPos,-oscy*ofGetHeight()/h+camYPos);
        sphere.draw();
        ofPopStyle();
        ofPopMatrix();
        
        for (int i=1; i<21; ++i){
            ofPushMatrix();
            ofTranslate(-handXs[i]*ofGetWidth()/w+camXPos, -handYs[i]*ofGetHeight()/h+camYPos);
            handSpheres[i].draw();
            ofPopMatrix();
        }
    }
    
    cam.end();
    light.disable();
    ofDisableDepthTest();
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
