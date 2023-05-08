#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(150);
    
    ofSetVerticalSync(true);
    gui.setup();
    gui.add(camXPos.setup("cam x pos", 500, 0, 1000));
    gui.add(camYPos.setup("cam y pos",-290, -500, 500));
    gui.add(camZPos.setup("cam z pos", 0, -500, 500));
    gui.add(showHand);
    
    light.setPosition(500, 500, 500);
    light2.setPosition(-500,600,400);
    light3.setPosition(0,300,-700);
    light4.setPosition(0,-2000,-100);
    
    cameraOrbit = 0;
    cam.setDistance(750);
    
    osc.setup(PORT);
    /*
    head.setRadius(100);
    legL.setRadius(20);
    legR.setRadius(20);
    eyeL.setRadius(7);
    eyeR.setRadius(7);
    nose.setRadius(60);
    */
    
    //https://sketchfab.com/3d-models/bird-orange-0d31748606c2499fb652c0c1052b7cfa
    model.loadModel("bird.dae", 20); //model by Wen Yeh
    curFileInfo = ".dae";
    
    sphere.setRadius(20);
    handSpheres.push_back(sphere); //wrist
    handXs.push_back(0);
    handYs.push_back(0);
    for (int i=0; i<21; ++i){
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
        handOnScreen = true;
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
            
            for (int i=0; i<20; ++i){
                handXs[i] = m.getArgAsFloat(4+i*3);
                handYs[i] = m.getArgAsFloat(5+i*3);
            }
        }
    
    
        //hand open/close status based on middle finger location
        ofVec2f middleTip(handXs[12],handYs[12]);
        ofVec2f wrist(handXs[0],handYs[0]);
        ofVec2f middle(handXs[9],handYs[9]);
        float tipWristDist = wrist.distance(middleTip);
        float middleWristDist = wrist.distance(middle);
        if (tipWristDist > middleWristDist) handClosed = false;
        else handClosed = true;
        closedAmount = ofMap(tipWristDist, 0, middleWristDist*2, 0, 1);
        
        // get zScale based on scale of hand on screen
        zScale = ofMap(middleWristDist, 0, ofGetHeight()/2, 0, 1.2);
        
        //hand spread status based on alignment of thumb & pinky to middle
        ofVec2f thumbMid(handXs[3],handYs[3]);
        ofVec2f thumbRoot(handXs[2],handYs[2]);
        ofVec2f pinkyTip(handXs[20],handYs[20]);
        ofVec2f pinkyRoot(handXs[17],handYs[17]);
        
        ofVec2f middleDir = middleTip-wrist;
        ofVec2f thumbDir = thumbMid-thumbRoot;
        float thumbAlign = abs(middleDir.dot(thumbDir));
        if (thumbAlign>0.7) thumbSpread = false;
        else thumbSpread = true;
        thumbSpreadAmount = ofMap(thumbAlign, 0, 1, 1, 0);
        
        ofVec2f pinkyDir = pinkyTip-pinkyRoot;
        float pinkyAlign = abs(middleDir.dot(pinkyDir));
        if (pinkyAlign>0.7) pinkySpread = false;
        else pinkySpread = true;
        pinkySpreadAmount = ofMap(pinkyAlign, 0, 1, 1, 0);
        
        //get orientation degree of hand
        float middleNormalAlign = middleDir.dot(ofVec2f(0,1));
        float cosTheta = middleNormalAlign/(middleDir.length());
        rotDeg = acos(cosTheta)/PI*180;
        if (middleNormalAlign>0) rotDeg+=180;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    ofEnableDepthTest();
    light.enable();
    light2.enable();
    light3.enable();
    light4.enable();
    cam.begin();
    /*
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
    */
    /*
    if (!handOnScreen){
        ofPushMatrix();
        ofRotateXDeg(180);
        ofTranslate(0,200);
        //model.drawFaces();
        ofPopMatrix();
    } else {
     */
        ofPushMatrix();
        ofScale(zScale,zScale,zScale);
        //ofRotateZDeg(-rotDeg);
        float scaleAmountX = ofMap(closedAmount,0,1,3.7,0.5);
        float scaleAmountY = ofMap(closedAmount,0,1,0.2,2);
        ofScale(scaleAmountX, scaleAmountY, scaleAmountX);
        
        ofRotateXDeg(180);
    
        ofTranslate(-oscx*ofGetWidth()/w+camXPos,oscy*ofGetHeight()/h+camYPos);
        
        
        model.drawFaces();
        ofPopMatrix();
    //}
    
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
    
    handOnScreen = false;
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
