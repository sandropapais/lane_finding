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
    Mat image = imread("test_images/solidWhiteRight.jpg");
    
    // Error handling
    if (image.empty())
    {
        cout << "Could not open the image" << endl;
        cin.get();
        return -1;
    }
    
    // Create window and show image
    String windowName = "Raw Image";
    namedWindow(windowName);
    imshow(windowName, image);
    
    // Wait for any key and close window
    waitKey(0);
    destroyWindow(windowName);
    
    return 0;
}