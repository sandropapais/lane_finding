//! Lane Finding
/*!
Longer description here...
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

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
    
    // Initialize and declare matrices
    Mat imgMask = Mat::zeros(h, w, CV_8UC1);
    Mat imgMasked;

    // Create mask
    int lineType = LINE_8;
    Point vertices[1][3]; 
    vertices[0][0] = Point(0, h);
    vertices[0][1] = Point(w/2, h/2);
    vertices[0][2] = Point(w, h);
    const Point* ppt[1] = {vertices[0]};
    int npt[] = {3};
    fillPoly(imgMask, ppt, npt, 1, Scalar(255,255,255), lineType);
    
    // Mask image
    bitwise_and(img, img, imgMasked, imgMask);
    
    // Create windows and show image    
    namedWindow("Raw Image");
    imshow("Raw Image", img);
    namedWindow("Mask");
    imshow("Mask", imgMask);
    namedWindow("Masked Image");
    imshow("Masked Image", imgMasked);
    
    waitKey(0);
    
    return 0;
}