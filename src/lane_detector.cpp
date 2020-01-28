#include <iostream>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
 
using namespace cv;
using namespace std;

void Mask(Mat img, Mat *imgMasked, int w, int h);
void lanefinding(Mat img);


void lanefinding(Mat img){
            // Get image dimensions
            int w = img.size().width;
            int h = img.size().height;
            
            // Gray scale image
            Mat imgGray;
            cvtColor(img,imgGray,COLOR_RGB2GRAY,0);
            
            // Canny edge detection
            Mat imgCanny;
            Canny(imgGray,imgCanny,50,200);
            
            // Create masked image
            Mat imgMasked;
            Mask(imgCanny, &imgMasked, w, h);
            
            // Probabilistic line transform
            vector<Vec4i> linesP;
            HoughLinesP(imgMasked, linesP, 1, CV_PI/180, 50, 50, 10);
            
            // Draw lines
            Mat imgLines;
            cvtColor(imgMasked, imgLines, COLOR_GRAY2RGB);
            for(size_t i=0; i<linesP.size(); i++)
            {
                Vec4i l = linesP[i];
                line(imgLines, Point(l[0],l[1]), Point(l[2],l[3]), Scalar(0,0,255), 3, LINE_AA);
            }
        
            // Create windows and show image    
            namedWindow("Raw");
            imshow("Raw", img);
            namedWindow("Edge Detection");
            imshow("Edge Detection", imgCanny);
            namedWindow("Masked");
            imshow("Masked", imgMasked);
            namedWindow("Lines");
            imshow("Lines", imgLines);
}

void Mask(Mat img, Mat *imgMasked, int w, int h){
    
    // Initialize mask
    Mat imgMask = Mat::zeros(h, w, CV_8UC1);

    // Define edge points of polygon for image mask
    Point vertices[1][3]; 
    vertices[0][0] = Point(0, h);
    vertices[0][1] = Point(w/2, h/2);
    vertices[0][2] = Point(w, h);
    
    // Create image mask
    int lineType = LINE_8;
    const Point* ppt[1] = {vertices[0]};
    int npt[] = {3};
    fillPoly(imgMask, ppt, npt, 1, Scalar(255,255,255), lineType);
    
    // Create masked image
    bitwise_and(img, img, *imgMasked, imgMask);
}