// 5.7.2023
// Thinning.h
// ref: https://github.com/golanlevin/GPP2016_Display/blob/master/src/Skeletonizer.h

#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "Contour.h"

using namespace ofxCv;
using namespace cv;

class Thinning {
    public:
        void initialize(int w, int h);
        void computeSkeleton(ofImage filledContourMat,
                             vector<ofPolyline> contours,
                             int nCurrentPosContour, int contourThickness,
                             int w, int h); //vector<vector<cv::Point>> &contours
        void skeletonize();
        void clear();
        void draw();
    
        inline int thin (int pass, unsigned char *table);
    
        ofImage inputImg, tmpImg;
        unsigned char* inputBuff;
        unsigned char* tmpBuff;
        int nCurrentPosCon; // number of positive contours
        
        float skeletonizationDuration;
        
        int buffW, buffH;
        int roiMinX, roiMaxX;
        int roiMinY, roiMaxY;
        
        unsigned char* skeletonBuff;
        ofImage skeletonImg;
};
