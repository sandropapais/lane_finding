//! Lane Finding
/*!
Longer description here...
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void Mask(Mat img, Mat *imgMasked, int w, int h);

int main(void)
{
    
    // Read image
    Mat img = imread("test_images/solidWhiteRight.jpg",IMREAD_COLOR);
    
    // Error handling
    if (img.empty())
    {
        cout << "Could not open the image" << endl;
        return -1;
    }
        
    // Get image dimensions
    int w = img.size().width;
    int h = img.size().height;
        
    // Create masked image
    Mat imgMasked;
    Mask(img, &imgMasked, w, h);
    
    // Create windows and show image    
    namedWindow("Raw Image");
    imshow("Raw Image", img);
    namedWindow("Masked Image");
    imshow("Masked Image", imgMasked);
    
    waitKey(0);
    
    return 0;
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