#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(255); //white
    
    //kinect.setLogLevel(OF_LOG_NOTICE);
    kinect.init();
    kinect.open();
    imitate(imgPx,kinect);
    
    h = kinect.height;
    w = kinect.width;
    
    nearClip = 4000; // in mm
    farClip = 10000; // in mm
    kinect.setDepthClipping(nearClip,farClip);
    
    colorImage.allocate(w,h);
    grayImage.allocate(w,h);
    grayBg.allocate(w,h);
    grayDiff.allocate(w,h);
    
    grayThreshold = 30;
    learnBg = false;
    
    output.allocate(w,h,OF_IMAGE_COLOR);
    
    if(!movenet.setup("model")) {
        std::exit(EXIT_FAILURE);
    }
    
    //font.load("Montserrat.ttf", 20);
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    if (kinect.isFrameNew()){
        colorImage.setFromPixels(kinect.getPixels());
        grayImage.setFromPixels(kinect.getDepthPixels());
       
        // pass in video input to movenet
        ofPixels pixels(kinect.getPixels());
        pixels.resize(nnWidth, nnHeight);
        movenet.setInput(pixels);
        
        // output rgb video from kinect
        output.setFromPixels(kinect.getPixels());
        output.mirror(false, true);
        
        absdiff(kinect,prevPx,imgDiff);
        imgDiff.update();
        imgDiff.mirror(false, true);
        copy(kinect,prevPx);
        
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(grayThreshold);
        contourFinder.findContours(grayImage, 30, (w*h), 10, true);
                
    }
    
    // run model on current input frame
    movenet.update();
}

//--------------------------------------------------------------
void ofApp::exit(){
    kinect.setCameraTiltAngle(0);
    kinect.close();
    movenet.stopThread();
}

//--------------------------------------------------------------
void ofApp::draw(){
    kinect.draw(0,0,w,h);
    //output.draw(0,0,w,h);
    movenet.draw();
    
    /* // POLYLINE OFFSET INTERACTION
    ofSetColor(0);
    for (int i = 0; i < kinect.getHeight(); i+=8){
        ofPolyline polyline;
        for (int j = 0; j < kinect.getWidth(); j++){
            ofColor col = imgDiff.getPixels().getColor(j,i);
            int brightness = col.getBrightness();
            polyline.addVertex(j, i+ofMap(brightness, 0, 255, 0, -64));
            //sends y coord up if bright
        }
        polyline = polyline.getSmoothed(10);
        polyline.draw();
    }
     */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case OF_KEY_UP:
            if (angle < 30) {
                ++angle;
            }
            kinect.setCameraTiltAngle(angle);
            break;

        case OF_KEY_DOWN:
            if (angle > -30) {
                --angle;
            }
            kinect.setCameraTiltAngle(angle);
            break;
        
        default:
            break;
    }
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
