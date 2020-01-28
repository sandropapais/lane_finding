//! Lane Finding
/*!
Longer description here...
*/

#include <iostream>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

void lanefinding(Mat img);

int main(void)
{
    enum mode {modeImages, modeVideo};
    mode setMode = modeImages;
    
    if (setMode == modeVideo) 
    {
    
        // Get all images
        vector<String> files;
        glob("/home/sandro/Dev/lane_finding/test_images/*.jpg", files);
        vector<Mat> images;
        size_t count = files.size(); // number of images in folder     
        
        for (size_t i=0; i<count; i++)
        {              
                                                        
            // Read image      
            Mat img = imread(files[i]);
            
            // Error handling
            if (img.empty())
            {
                cout << "Could not open the image" << endl;
                cin.get();
                return -1;
            }
                
            lanefinding(img);
            
            waitKey(0);
            
        }
    }
    else
    {
        // Open video file
        VideoCapture cap("test_video/solidWhiteRight.mp4");
        
        // Error handling
        if (cap.isOpened() == false)
        {
            cout << "Cannot open video file" << endl;
            cin.get();
            return -1;
        }
        
        // Initialize variables
        Mat img;
        bool readSuccess = true;
        
        // skip to 8 seconds
        cap.set(CAP_PROP_POS_MSEC, 8000); 
        
        while (readSuccess)
        {
            readSuccess = cap.read(img); // read a new frame form video
            
            lanefinding(img);
            
            waitKey(0);

        }
        
    }
    
    return 0;
}


