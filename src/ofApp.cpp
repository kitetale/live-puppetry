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
    /*model.loadModel("hand.obj", 20);
    curFileInfo = ".obj";
    
    model.setRotation(0, 0, 0, 0, 0);
    model.setScale(1,1,1);
     */
    light.setPosition(0, 0, 500);
    
    cameraOrbit = 0;
    cam.setDistance(750);
    
    osc.setup(PORT);
    
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
            
            
            /*
            //thumb xy
            x1 = m.getArgAsFloat(7); y1 = m.getArgAsFloat(8);
            x2 = m.getArgAsFloat(10); y2 = m.getArgAsFloat(11);
            x3 = m.getArgAsFloat(13); y3 = m.getArgAsFloat(14);
            x4 = m.getArgAsFloat(16); y4 = m.getArgAsFloat(17);
            //index xy
            x5 = m.getArgAsFloat(19); y5 = m.getArgAsFloat(20);
            x6 = m.getArgAsFloat(22); y6 = m.getArgAsFloat(23);
            x7 = m.getArgAsFloat(25); y7 = m.getArgAsFloat(26);
            x8 = m.getArgAsFloat(28); y8 = m.getArgAsFloat(29);
            //middle xy
            x9  = m.getArgAsFloat(31); y9  = m.getArgAsFloat(32);
            x10 = m.getArgAsFloat(34); y10 = m.getArgAsFloat(35);
            x11 = m.getArgAsFloat(37); y11 = m.getArgAsFloat(38);
            x12 = m.getArgAsFloat(40); y12 = m.getArgAsFloat(41);
            //ring xy
            x13 = m.getArgAsFloat(43); y13 = m.getArgAsFloat(44);
            x14 = m.getArgAsFloat(46); y14 = m.getArgAsFloat(47);
            x15 = m.getArgAsFloat(49); y15 = m.getArgAsFloat(50);
            x16 = m.getArgAsFloat(52); y16 = m.getArgAsFloat(53);
            //pinky xy
            x17 = m.getArgAsFloat(55); y17 = m.getArgAsFloat(56);
            x18 = m.getArgAsFloat(58); y18 = m.getArgAsFloat(59);
            x19 = m.getArgAsFloat(61); y19 = m.getArgAsFloat(62);
            x20 = m.getArgAsFloat(64); y20 = m.getArgAsFloat(65);
            */
            
        }
        
    }
    /*
    vector<glm::vec3> vertices0 = sphere.getMesh().getVertices();
    vector<glm::vec3> &vertices = sphere.getMesh().getVertices();
    for (int i=0; i<vertices.size(); i++) {
      ofPoint v = vertices0[i];
      v.normalize();
      float sx = sin( v.x * deformFreq );
      float sy = sin( v.y * deformFreq );
      float sz = sin( v.z * deformFreq );
      v.x += sy * sz * deform;
      v.y += sx * sz * deform;
      v.z += sx * sy * deform;
      v *= rad;
      vertices[i] = v;
    }*/
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofEnableDepthTest();
    
    light.enable();

    cam.begin();
/*
    ofColor(255,255);
    ofPushMatrix();
    ofTranslate(-oscx*ofGetWidth()/w,-oscy*ofGetHeight()/h);
    model.drawFaces();
    ofPopMatrix();
   
  */
    
    //osc tester
    //ofDrawCircle(oscx*ofGetWidth()/w, oscy*ofGetHeight()/h, 50);
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
