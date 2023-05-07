// 5.7.2023
// Thinning.cpp
// ref: https://github.com/golanlevin/GPP2016_Display/blob/master/src/Skeletonizer.cpp

#include "Thinning.h"

//--------------------------------------------------------------
void Thinning::initialize(int w, int h){
    buffW = w;
    buffH = h;
    nCurrentPosCon = 0;
    
    roiMinX = 0;
    roiMaxX = buffW -1;
    roiMinY = 0;
    roiMaxY = buffH -1;
    
    skeletonBuff = new unsigned char[buffW * buffH];
    tmpBuff = new unsigned char[buffW * buffH];
    inputBuff = skeletonBuff;
    
    skeletonImg.allocate(buffW, buffH, OF_IMAGE_GRAYSCALE);
    inputImg.allocate(buffW, buffH, OF_IMAGE_GRAYSCALE);
    tmpImg.allocate(buffW, buffH, OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void Thinning::draw(){
    //ofSetHexColor(0xffffff);
    ofBackground(255);
    ofClear(255);
    skeletonImg.draw(0, 0); // displayW,displayH);
    /*if (nCurrentPosCon > 0){
        ofSetHexColor(0x666666);
        ofNoFill();
        ofPushMatrix();
        ofTranslate(displayX,displayY);
        ofDrawRectangle(roiMinX, roiMinY, roiMaxX-roiMinX, roiMaxY-roiMinY);
        ofPopMatrix();
    }*/
}

//--------------------------------------------------------------
void Thinning::clear(){
    int nPixels = buffW * buffH;
    memset(skeletonBuff, (unsigned char)0, nPixels);
    roiMinX = 0;
    roiMaxX = 1;
    roiMinY = 0;
    roiMaxY = 1;
}

//--------------------------------------------------------------
void Thinning::computeSkeleton(ofImage filledContourMat,
                               vector<ofPolyline> contours,
                               int nCurrentPosContour, int contourThickness,
                               int w, int h){
    nCurrentPosCon = nCurrentPosContour;
    buffW = w;
    buffH = h;
    
    // Copy the blob pixels from filledContourMat into skeletonBuff
    int nPixels = w * h;
    /*memcpy(skeletonBuff, filledContourMat.getPixels().getData(), nPixels);
    memset(tmpBuff, (unsigned char)0, nPixels);
    skeletonBuff = filledContourMat.getPixels().getData();
    inputBuff = skeletonBuff;
     */
    inputImg.setFromPixels(filledContourMat.getPixels());
    
    //skeletonImg.setFromPixels(filledContourMat.getPixels().getData(), w, h, OF_IMAGE_GRAYSCALE);
    //skeletonImg.setFromPixels(filledContourMat.getPixels());
    
    if (nCurrentPosCon == 0){
        // If there are no bodies present, don't bother searching.
        //memset(skeletonBuff, (unsigned char)255, nPixels);
        
    } else {
        // Compute the bounds of the ROI, which is a rect containing all contours.
        // The ROI speeds up the skeletonization.
        roiMinX = INT_MAX;
        roiMaxX = INT_MIN;
        roiMinY = INT_MAX;
        roiMaxY = INT_MIN;
        int nContours = contours.size();
        for (int i=0; i<nContours; i++){ // Fill the positive contours
            auto ithContour = contours[i];
            int nPoints = ithContour.size();
            for (int j=0; j<nPoints; j++){
                ofPoint jthPoint = ithContour[j];
                int jx = jthPoint.x;
                int jy = jthPoint.y;
                roiMinX = MIN(roiMinX, jx);
                roiMaxX = MAX(roiMaxX, jx);
                roiMinY = MIN(roiMinY, jy);
                roiMaxY = MAX(roiMaxY, jy);
            }
        }
        // Adding a small search margin to the ROI prevents an unpleasant artifact.
        int skeletonSearchMargin = contourThickness + 1;
        roiMinX -= skeletonSearchMargin;
        roiMinY -= skeletonSearchMargin;
        roiMaxX += skeletonSearchMargin;
        roiMaxY += skeletonSearchMargin;
        
        // Clamp the skeleton search ROI to the bounds of the image.
        roiMinX = MIN(w-1, MAX(roiMinX, 0));
        roiMinY = MIN(h-1, MAX(roiMinY, 0));
        roiMaxX = MIN(w-1, MAX(roiMaxX, 0));
        roiMaxY = MIN(h-1, MAX(roiMaxY, 0));
        
        skeletonize();
    }
    
    //skeletonImg.setFromPixels(skeletonBuff, w,h, OF_IMAGE_GRAYSCALE);
    //skeletonImg.setFromPixels(inputBuff, w,h, OF_IMAGE_GRAYSCALE);
    skeletonImg.setFromPixels(inputImg.getPixels());
}


//--------------------------------------------------------------
void Thinning::skeletonize() {
    unsigned char table[]  = \
        {0,0,0,1,0,0,1,3,0,0,3,1,1,0,1,3,0,0,0,0,0,0,0,0,2,0,2,0,3,0,3,3, \
        0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,3,0,2,2, \
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, \
        2,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,3,0,0,0,0,0,0,0,3,0,0,0,3,0,2,0, \
        0,0,3,1,0,0,1,3,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1, \
        3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, \
        2,3,1,3,0,0,1,3,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, \
        2,3,0,1,0,0,0,1,0,0,0,0,0,0,0,0,3,3,0,1,0,0,0,0,2,2,0,0,2,0,0,0};
        /* magic from ref */
        
    int sw = buffW;
    int sh = buffH;
    int sn = sw*sh;
    //unsigned char* pixels = inputBuff;
    ofPixels pixels = inputImg.getPixels();
    
    // Black the outermost edges of the input buffer, to prevent an ugly artifact.
    for (int i=0; i<sw; i++){
        pixels[sn-sw+i] = pixels[i] = 0; }
    for (int i=0; i<sh; i++){
        pixels[i*sw + (sw-1)] = pixels[i*sw] = 0; }
    
    long then = ofGetElapsedTimeMicros();
    
    // Do the iterative, thinning-based skeletonization.
    int pass = 0;
    int pixelsRemoved = 0;
    do {
        pixelsRemoved = thin(pass++, table);
    } while (pixelsRemoved > 0);
    
    // Compute how much time that took.
    long now = ofGetElapsedTimeMicros();
    float elapsed = (now - then);
    float A = 0.95; float B = 1.0-A;
    skeletonizationDuration = A*skeletonizationDuration + B*elapsed;
    
}

//--------------------------------------------------------------
int Thinning::thin (int pass, unsigned char *table) {
    const int sw = buffW;
    const int sh = buffH;
    const int xMin = roiMinX;
    const int xMax = roiMaxX;
    const int yMin = roiMinY;
    const int yMax = roiMaxY;
    
    //memcpy (tmpBuff, inputBuff, buffW*buffH);
    tmpImg.setFromPixels(inputImg.getPixels());
    /*
    unsigned char* pixels = inputBuff;
    unsigned char* pixels2 = tmpBuff;
     */
    ofPixels pixels = inputImg.getPixels();
    ofPixels pixels2 = tmpImg.getPixels();
    
    int offset;
    int pixelsRemoved = 0;
    const int rowOffset = sw;
    const int rowOffsetp1 = rowOffset+1;
    unsigned char index;
    unsigned char *ptr;
    unsigned char v;
    
    const bool b_oddPass = (pass & 1);
    const unsigned char testb = (b_oddPass) ? 2:1;
    
    int y,x;
    int yswpxmin = yMin*sw + xMin;
    int offsetmRowoffsetp1;
    offset = yswpxmin;
    
    for (y=yMin; y<yMax; y++) {
        offset = yswpxmin;
        offsetmRowoffsetp1 = offset - rowOffsetp1;
        yswpxmin += sw;
        
        for (x=xMin; x<xMax; x++,offsetmRowoffsetp1++) {
            if ((v = pixels2[offset])) { // yes, assignment.
                ptr = &pixels2[offsetmRowoffsetp1];
                index = 0;
                if (*ptr++){ index |= 1;  }
                if (*ptr++){ index |= 2;  }
                if (*ptr)  { index |= 4;  } ptr+=rowOffset;
                if (*ptr)  { index |= 8;  } ptr+=rowOffset;
                if (*ptr--){ index |= 16; }
                if (*ptr--){ index |= 32; }
                if (*ptr)  { index |= 64; } ptr-=rowOffset;
                if (*ptr)  { index |= 128;}
                
                if ((table[index])&testb){
                    v = 0;
                    pixelsRemoved++;
                }
            }
            pixels[offset++] = v;
        }
    }
    
    return pixelsRemoved;
}
