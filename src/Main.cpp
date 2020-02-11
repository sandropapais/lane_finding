//! Lane Finding
/*!
Longer description here...
*/

#include <iostream>
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "lane_detector.hpp"

int main(void)
{
    enum mode {modeImages, modeVideo};

    mode setMode = modeVideo;
    
    // Mode 1: Image Processing (debug flag required)
    // Read, process, and show images
    if (setMode == modeImages) 
    {
        // Get all images
        std::vector<std::string> files;
        cv::glob("/home/sandro/Dev/lane_finding/input/*.jpg", files);
        size_t count = files.size(); // number of images in folder     
        
        for (size_t i=0; i<count; i++)
        {              
            // Read image      
            cv::Mat img = cv::imread(files[i]);
            
            // Error handling
            if (img.empty())
            {
                std::cout << "Could not open the image" << std::endl;
                std::cin.get();
                return -1;
            }
                
            // Call image processing function
            lanefinding(img);
            cv::waitKey(0);
                        
        }
    }
    // Mode 2: Video Processing 
    // Read, process, and write video frames
    else if (setMode == modeVideo)
    {
        // Create video read file
        cv::VideoCapture cap("input/solidWhiteRight.mp4");
        
        // Error handling
        if (cap.isOpened() == false)
        {
            std::cout << "Cannot open read video file" << std::endl;
            std::cin.get();
            return -1;
        }
        
        // Initialize variables
        cv::Mat img;
        bool readSuccess = cap.read(img); // read first image
        
        // Create video write file
        cv::VideoWriter writer;
        int codec = cv::VideoWriter::fourcc('M','J','P','G');
        double fps = 25; //cap.get(cv::CAP_PROP_FPS);
        std::string writefilename = "./output/test.avi";
        writer.open(writefilename, codec, fps, img.size(), 1);
        
        // Error handling
        if (!writer.isOpened()) {
            std::cout << "Cannot open write video file" << std::endl;
            std::cin.get();
            return -1;
        }
        
        std::cout << "Processing video..." << std::endl;
        while (readSuccess)
        {
            cv::Mat imgLines;
            imgLines = lanefinding(img);
                        
            writer.write(imgLines);
            
            readSuccess = cap.read(img); // read a new frame from video
        }
        
        
    }
    
    return 0;
}


