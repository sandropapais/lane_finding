#include <iostream>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "lane_detector.hpp"

cv::Mat lanefinding(cv::Mat &img){
    
    // Debug flag for verbosity and additional details in output image
    enum flag {debugOn, debugOff};
    flag debugFlag = debugOff;
    
    // Get image dimensions
    int w = img.size().width;
    int h = img.size().height;
    
    // Denoise image
    cv::Mat imgDenoised;
    cv::GaussianBlur(img, imgDenoised, cv::Size(3, 3), 0, 0);           
    
    // Gray scale image
    cv::Mat imgGray;
    cvtColor(imgDenoised,imgGray,cv::COLOR_RGB2GRAY,0);
    
    // Canny edge detection
    cv::Mat imgCanny;
    cv::Canny(imgGray,imgCanny,140,255);
    
    // Create masked image
    cv::Mat imgMasked;
    Mask(imgCanny, imgMasked, w, h);
    
    // Probabilistic line transform
    std::vector<cv::Vec4i> linesP;
    cv::HoughLinesP(imgMasked, linesP, 1, CV_PI/180, 20, 20, 30);
    
    // Filter and sort lines
    std::vector<cv::Vec4i> linesL;
    std::vector<cv::Vec4i> linesR;
    double slopeThresh = 0.3;
    sortLines(linesP, linesL, linesR, slopeThresh);
    
    // Regression of lane lines
    cv::Point leftlanept0, leftlanept1, rightlanept0, rightlanept1;
    regressLines(linesL, leftlanept0, leftlanept1, h);
    regressLines(linesR, rightlanept0, rightlanept1, h);
    
    // Draw lines
    cv::Mat imgLines = img;
    cv::Vec4i l;
    if (debugFlag == debugOn){
        for(size_t i=0; i<linesP.size(); i++) // Discarded detected lines in red
        {
            l = linesP[i]; 
            line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(0,0,255), 3, cv::LINE_AA);
        }            
        for(size_t i=0; i<linesL.size(); i++) // Filtered left lines in blue
        {
            l = linesL[i]; 
            line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(255,0,0), 3, cv::LINE_AA);
        }
        for(size_t i=0; i<linesR.size(); i++) // Filtered right lines in blue
        {
            l = linesR[i]; 
            line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(255,0,0), 3, cv::LINE_AA);
        }
    } // Lane lines in green
    line(imgLines, leftlanept0, leftlanept1, cv::Scalar(0,255,0), 3, cv::LINE_AA);
    line(imgLines, rightlanept0, rightlanept1, cv::Scalar(0,255,0), 3, cv::LINE_AA);
        
    // Create windows and show image
    cv::namedWindow("Raw");
    imshow("Raw", img);
    cv::namedWindow("Edge Detection");
    imshow("Edge Detection", imgCanny);
    cv::namedWindow("Masked");
    imshow("Masked", imgMasked);
    cv::namedWindow("Lines");
    imshow("Lines", imgLines);
    
    // Output stream results and wait for key stroke (debug mode)
    if (debugFlag == debugOn) { 
        std::cout << "-----" << std::endl;
        std::cout << "Number of lines detected: " << linesP.size() << std::endl;
        std::cout << "Number of filtered left lines: " << linesL.size() << std::endl;
        std::cout << "Number of filtered right lines: " << linesR.size() << std::endl;
        std::cout << "Number of lines rejected: " << linesP.size()-linesL.size()-linesR.size() << std::endl;
        cv::waitKey(0);
    }
    
    // Output processed image
    return imgLines;
}

void Mask(cv::Mat img, cv::Mat &imgMasked, int w, int h){
    
    // Initialize mask
    cv::Mat imgMask = cv::Mat::zeros(h, w, CV_8UC1);

    // Define edge points of polygon for image mask
    cv::Point vertices[1][3]; 
    vertices[0][0] = cv::Point(0, h);
    vertices[0][1] = cv::Point(w/2, 0.55*h);
    vertices[0][2] = cv::Point(w, h);
    
    // Create image mask
    int lineType = cv::LINE_8;
    const cv::Point* ppt[1] = {vertices[0]};
    int npt[] = {3};
    cv::fillPoly(imgMask, ppt, npt, 1, cv::Scalar(255,255,255), lineType);
    
    // Create masked image
    cv::bitwise_and(img, img, imgMasked, imgMask);
}

void sortLines(std::vector<cv::Vec4i> linesP, std::vector<cv::Vec4i> &linesL, std::vector<cv::Vec4i> &linesR, double slopeThresh){
    
    std::vector<double> slopes;
    double x0, x1, y0, y1;
    
    // Loop through detected lines
    for(size_t i=0; i<linesP.size(); i++)
    {
        // Get line end points
        x0 = linesP[i][0];
        y0 = linesP[i][1];
        x1 = linesP[i][2];
        y1 = linesP[i][3];
        
        // Calculate line slopes
        slopes.push_back((y1-y0)/(x1-x0));
        
        // Sort slopes into left or right if criteria is met
        if (slopes[i] > slopeThresh)
        {
            linesL.push_back(linesP[i]);
        }
        else if (slopes[i] < -slopeThresh)
        {
            linesR.push_back(linesP[i]);
        }
    }
}

void regressLines(std::vector<cv::Vec4i> lines, cv::Point &lanept0, cv::Point &lanept1, int h){
    
    cv::Vec4i l;
    std::vector<cv::Point> points; 
    cv::Vec4d linefit;
    double m;
    cv::Point b[1][1];

    // Get points from end points of lines
    for(size_t i=0; i<lines.size(); i++)
    {                
        l = lines[i];
        points.push_back(cv::Point(l[0], l[1]));
        points.push_back(cv::Point(l[2], l[3]));
    }           
    
    // Linear regression of points 
    cv::fitLine(points, linefit, cv::DIST_L2, 0, 0.01, 0.01);
    m = linefit[1]/linefit[0];
    b[0][0] = cv::Point(linefit[2], linefit[3]);
    
    // Calculate regression line end points
    int x0 = (h-b[0][0].y)/m + b[0][0].x;
    int x1 = (0.6*h-b[0][0].y)/m + b[0][0].x;
    lanept0 = cv::Point(x0,h);
    lanept1 = cv::Point(x1,0.6*h);
    
}
