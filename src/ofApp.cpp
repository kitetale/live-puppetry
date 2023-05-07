#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(255); //white
    kinect.setCameraTiltAngle(0);
    
    //kinect.setLogLevel(OF_LOG_NOTICE);
    kinect.init();
    kinect.open();
    
    h = kinect.height; //480
    w = kinect.width; //640
    
    nearClip = 400; // in mm
    farClip = 4000; // in mm
    kinect.setDepthClipping(nearClip,farClip);
    
    grayThreshold = 30;
    learnBg = false;
    
    minBlobSize = 20;
    maxBlobSize = 250;

    contourFinder.setMinArea(minBlobSize);
    contourFinder.setMaxArea(maxBlobSize);
    
    buffW = 640;
    buffH = 480;
    
    contourThickness = 8;
    filledContourMat.create(buffH, buffW, CV_8UC(1));
    
    inputImg.allocate(w,h);
    inputImg.set(0);
     
    #ifdef USE_VIDEO
        video.load("2.mov");
        w = video.getWidth();
        h = video.getHeight();
        scale = 480.0/w;
        video.play();
    #endif
    
    contour.setup(w, h, scale);
    contour.setMinAreaRadius(minBlobSize);
    contour.setMaxAreaRadius(maxBlobSize);
    
    fbo.allocate(w, h, GL_RGB);
    filledContourImg.allocate(w, h, OF_IMAGE_GRAYSCALE);
    skeletonImg.allocate(w, h, OF_IMAGE_GRAYSCALE);
    skeletonizer.initialize(w,h);
}

//--------------------------------------------------------------
void ofApp::update(){
    #ifdef USE_VIDEO
        video.update();
        if(video.isFrameNew()) {
            
            float time = ofGetElapsedTimef();
            float dt = 1.0 / max(ofGetFrameRate(), 1.f);
            depthImage.setFromPixels(video.getPixels());
            contour.update(dt, depthImage);
            
            fbo.begin();
            ofClear(255, 255, 255);
            contour.draw();
            fbo.end();
            
            ofPixels pix;
            pix.allocate(w, h, OF_IMAGE_GRAYSCALE);
            fbo.readToPixels(pix);
            ofSaveImage(pix,"contour.jpg", OF_IMAGE_QUALITY_BEST);
            
            //filledContourImg.setFromPixels(pix);
            
            //inputImg.setFromPixels(video.getPixels());
            
           /* contourFinder.setTargetColor(ofColor(255,255,255), TRACK_COLOR_RGB);
            contourFinder.setThreshold(80.0);
            contourFinder.setSortBySize(true);
            contourFinder.setFindHoles(true);//bFindHoles
            
            contourFinder.findContours(inputImg);
            */
            
            /*
            vector<vector<cv::Point>> rawContours = contourFinder.getContours();
            
            theContoursi.clear();
            int nRawContours = rawContours.size();
            if (nRawContours > 0) { // something detected
                vector<cv::Point> ithRawContour = rawContours[0]; // just use the first one detected, limit to one body
                ofPolyline ithNewPolyline;
                int nPoints = ithRawContour.size();
                for (int j=0; j<nPoints; j++){
                    cv::Point jthRawPoint = ithRawContour[j];
                    float jxf = (float)jthRawPoint.x;
                    float jyf = (float)jthRawPoint.y;
                    
                    jxf *= scale;
                    jyf *= scale;
                    
                    ithNewPolyline.addVertex(jxf, jyf);
                }
                
                //simplify contours
                ithNewPolyline.setClosed(true);
                if (bSimplifyInputPolylines) {
                    ithNewPolyline.simplify(); }
                if (inputLineResample > 1.0) {
                    ithNewPolyline = ithNewPolyline.getResampledBySpacing(inputLineResample); }
                if (bSmoothHolesToo || (myOfxCvContourFinder.getHole(i) == false)){
                    if (inputLineSmoothing > 0){
                        ithNewPolyline = ithNewPolyline.getSmoothed(inputLineSmoothing);
                    }
                }
                 
                
                vector<cv::Point> ithNewContour;
                int nPts = ithNewPolyline.size();
                for (int j=0; j<nPts; j++){
                    ofVec2f jthNewPoint = ithNewPolyline[j];
                    int jxi = (int)round(jthNewPoint.x);
                    int jyi = (int)round(jthNewPoint.y);
                    ithNewContour.push_back(cv::Point(jxi,jyi));
                }
                theContoursi.push_back(ithNewContour);
                
                reconstituteBlobsFromContours(theContoursi, buffW, buffH);
                */
            /*
            } else {
                skeletonizer.clear();
                filledContourMat.setTo(0);
            }
             */
        }
    #else
        kinect.update();
        if (kinect.isFrameNew()){
            
                    
        }
    #endif
    
    //filledContourImg.load("contour.jpg");
    //toOf(filledContourMat, filledContourImg);
    skeletonizer.computeSkeleton(filledContourImg, contour.contours, 1, contourThickness, filledContourImg.getWidth(),filledContourImg.getHeight());
    /*
    unsigned char* pix = skeletonImg.getPixels().getData();
    for (int i=0; i<w*h; ++i){
        pix[i] = skeletonizer.skeletonBuff[i];
    }
    skeletonImg.update();
    */
}

//--------------------------------------------------------------
void ofApp::reconstituteBlobsFromContours (vector<vector<cv::Point>> &contours, int w, int h){
    // flood fill to reconstitute binary img blobs from contours for skeletonizer
    int nContours = contours.size(); //TODO: maybe can limit to 1 only and remove for loops
    
    filledContourMat.setTo(0);
    for (int i=0; i<nContours; i++){ // Fill the positive contours
        if (contourFinder.getHole(i) == false) {
            cv::drawContours(filledContourMat, contours, i,255, CV_FILLED);
            if (contourThickness > 0){
                cv::drawContours(filledContourMat, contours, i,255, contourThickness);
            }
        }
    }
    for (int i=0; i<nContours; i++){ // Fill the negative contours
        if (contourFinder.getHole(i) == true) {
            cv::drawContours(filledContourMat, contours, i,0, CV_FILLED);
            if (contourThickness > 0){
                cv::drawContours(filledContourMat, contours, i,0, contourThickness);
            }
        }
    }
    
    // Copy over those pixels into filledContourImage, for drawing etc.
    unsigned char* filledContourMatPixels;
    filledContourMatPixels = filledContourMat.data;
   // filledContourImg.setFromPixels(filledContourMatPixels, w,h, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void ofApp::exit(){
    kinect.setCameraTiltAngle(0);
    kinect.close();
    skeletonizer.clear();
}

//--------------------------------------------------------------
void ofApp::draw(){
    #ifdef USE_VIDEO
        ofClear(255);
        filledContourImg.load("contour.jpg");
        //filledContourImg.draw(0,0);
        //skeletonImg.draw(0,0);
        skeletonizer.draw();
    
        ofPushMatrix();
            ofScale(scale);
            //video.draw(0,0);
            
            //fbo.draw(0,0);
            
    
            //skeletonizer.draw();
    
            ofPushStyle();
                ofSetColor(0,255,0);
                ofSetLineWidth(2);
                //contour.draw();
                
                //contourFinder.draw(); //for kinect input, video is already silhouette
            ofPopStyle();
            ofPushStyle();
                ofSetColor(ofColor(0,255,0));
               
            ofPopStyle();
        ofPopMatrix();
        
    //TODO: call thinning algo & draw skeleton
    //TODO: read paper on skeleton construction based on thinning -> assign joint hierarchy
    //TODO: import 3D mesh
    //TODO: map skeleton location to mesh/puppet
    //TODO: update mesh info and draw mesh on screen
    
    #else
        kinect.draw(0,0);
    #endif
    
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
    
    //KINECT DEBUG
    /*
    ofPushStyle();
        ofSetColor(255);
        stringstream reportStream;
        reportStream << "set near clipping " << nearClip << " (press: k l)" << endl
            << "set far clipping " << farClip << " (press: < >)" << endl
            << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl;
        
        ofDrawBitmapString(reportStream.str(), 20, 100);
    ofPopStyle();
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
        case 'k':
            nearClip-=10;
            break;
        case 'l':
            nearClip+=10;
            break;
        case ',':
        case '<':
            farClip-=10;
            break;
        case '.':
        case '>':
            farClip+=10;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key){
        case 'k':
        case 'l':
        case ',':
        case '.':
        case '<':
        case '>':
            kinect.setDepthClipping(nearClip,farClip);
            break;
        default:
            break;
    }
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
