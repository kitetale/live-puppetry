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
    
    #ifdef USE_VIDEO
        video.load("production ID 3873059_2.mp4");
        //video.load("shadow_videos/4.mov");
        video.play();
    #endif
    
    //font.load("Montserrat.ttf", 20);
}

//--------------------------------------------------------------
void ofApp::update(){
    #ifdef USE_VIDEO
        video.update();
        if(video.isFrameNew()) {
            ofPixels pixels(video.getPixels());
            pixels.resize(nnWidth, nnHeight);
            // pass in video input to movenet
            movenet.setInput(pixels);
        }
    #else
        kinect.update();
        if (kinect.isFrameNew()){
            ofPixels pix(kinect.getPixels());
            pix.resize(nnWidth, nnHeight);
            colorImage.setFromPixels(pix);
            ofPixels depthPix(kinect.getDepthPixels());
            depthPix.resize(nnWidth, nnHeight);
            grayImage.setFromPixels(depthPix);
           
            // pass in video input to movenet
            movenet.setInput(pix);
            
            // output rgb video from kinect
            output.setFromPixels(pix);
            output.mirror(false, true);
            
            absdiff(kinect,prevPx,imgDiff);
            imgDiff.update();
            imgDiff.mirror(false, true);
            copy(kinect,prevPx);
            
            grayDiff.absDiff(grayBg, grayImage);
            grayDiff.threshold(grayThreshold);
            contourFinder.findContours(grayImage, 30, (w*h), 10, true);
                    
        }
    #endif
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
    #ifdef USE_VIDEO
        video.draw(0,0);
    #else
        //kinect.draw(0,0,nnWidth,nnHeight);
    #endif
    output.draw(0,0,nnWidth,nnHeight);
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
