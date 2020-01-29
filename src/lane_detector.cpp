#include <iostream>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
 
void Mask(cv::Mat img, cv::Mat &imgMasked, int w, int h);
void lanefinding(cv::Mat &img);


void lanefinding(cv::Mat &img){
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
            cv::Canny(imgGray,imgCanny,50,200);
            
            // Create masked image
            cv::Mat imgMasked;
            Mask(imgCanny, imgMasked, w, h);
            
            // Probabilistic line transform
            std::vector<cv::Vec4i> linesP;
            cv::HoughLinesP(imgMasked, linesP, 1, CV_PI/180, 50, 50, 10);
            
            // Draw lines
            cv::Mat imgLines;
            cv::cvtColor(imgMasked, imgLines, cv::COLOR_GRAY2RGB);
            for(size_t i=0; i<linesP.size(); i++)
            {
                cv::Vec4i l = linesP[i];
                line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(0,0,255), 3, cv::LINE_AA);
            }
        
            // Create windows and show image    
            cv::namedWindow("Raw");
            imshow("Raw", img);
            cv::namedWindow("Edge Detection");
            imshow("Edge Detection", imgCanny);
            cv::namedWindow("Masked");
            imshow("Masked", imgMasked);
            cv::namedWindow("Lines");
            imshow("Lines", imgLines);
}

void Mask(cv::Mat img, cv::Mat &imgMasked, int w, int h){
    
    // Initialize mask
    cv::Mat imgMask = cv::Mat::zeros(h, w, CV_8UC1);

    // Define edge points of polygon for image mask
    cv::Point vertices[1][3]; 
    vertices[0][0] = cv::Point(0, h);
    vertices[0][1] = cv::Point(w/2, h/2);
    vertices[0][2] = cv::Point(w, h);
    
    // Create image mask
    int lineType = cv::LINE_8;
    const cv::Point* ppt[1] = {vertices[0]};
    int npt[] = {3};
    cv::fillPoly(imgMask, ppt, npt, 1, cv::
    Scalar(255,255,255), lineType);
    
    // Create masked image
    cv::bitwise_and(img, img, imgMasked, imgMask);
}