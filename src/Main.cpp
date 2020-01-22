//! Lane Finding
/*!
Longer description here...
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    // Read image
    Mat img = imread("test_images/solidWhiteRight.jpg");
    
    // Error handling
    if (img.empty())
    {
        cout << "Could not open the image" << endl;
        return -1;
    }
    
    
    // Set region of interest
    int w = img.size().width;
    int h = img.size().height;
    Rect roi;
    roi.x = w/8;        // top left corner
    roi.y = h/2;
    roi.width = 3*w/4;    // dimensions
    roi.height = h/2;
    
    
    Mat imgMask = Mat::zeros(h, w, CV_8UC1);
    //Mat vertices = (Mat_<double>(4,2) << 0, 0, w, 0, w, h, 0,h);
    //cout << vertices << endl; 

    /*
    int vertices[4][2] = {
        0, 0, 
        w, 0, 
        w, h, 
        0, h
    };
    for (int i=0; i<4; i++)
        for (int j=0; j<2; j++){
            cout << vertices[i][j] << endl;
        }
    */
   

    //int ignore_mask_color = (2^8)-1;
    //void fillPoly(Mat& imgMask, const Point**)
    //namedWindow("Mask");
    //imshow("Mask", imgMask);
    
    
    // Crop the original image to the defined ROI
    Mat imgCrop = img(roi);
    
    // Create windows and show image    
    namedWindow("Raw Image");
    imshow("Raw Image", img);
    namedWindow("Cropped Image");
    imshow("Cropped Image", imgCrop);
    
    //cout << img << endl;
    
    waitKey(0);
    
    return 0;
}