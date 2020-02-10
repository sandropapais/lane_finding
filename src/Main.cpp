//! Lane Finding
/*!
Longer description here...
*/

#include <iostream>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

//using namespace cv;

cv::Mat lanefinding(cv::Mat &img);

int main(void)
{
    enum mode {modeImages, modeVideo};
    mode setMode = modeImages;
    
    if (setMode == modeVideo) 
    {
    
        // Get all images
        std::vector<std::string> files;
        cv::glob("/home/sandro/Dev/lane_finding/test_images/*.jpg", files);
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
                
            lanefinding(img);
            
            cv::waitKey(0);
            
        }
    }
    else
    {
        // Create video read file
        cv::VideoCapture cap("test_video/solidWhiteRight.mp4");
        
        // Error handling
        if (cap.isOpened() == false)
        {
            std::cout << "Cannot open video file" << std::endl;
            std::cin.get();
            return -1;
        }
        
        // Initialize variables
        cv::Mat img;
        //bool readSuccess = true;
        bool readSuccess = cap.read(img); // read first image
        
        // skip to 8 seconds
        //cap.set(cv::CAP_PROP_POS_MSEC, 8000); 
        
        // Create video write file
        cv::VideoWriter writer;
        int codec = cv::VideoWriter::fourcc('M','J','P','G');
        double fps = 10; //cap.get(cv::CAP_PROP_FPS);
        std::string writefilename = "./test.mp4";
        writer.open(writefilename, codec, fps, img.size(), 1);
        
        // Error handling
        if (!writer.isOpened()) {
            std::cout << "Cannot open video file" << std::endl;
            std::cin.get();
            return -1;
        }
        
        while (readSuccess)
        {
            
            
            cv::Mat imgLines;
            imgLines = lanefinding(img);
            
            //cv::waitKey(0); // for debugging
            
            writer.write(imgLines);
            
            readSuccess = cap.read(img); // read a new frame form video

        }
        
    }
    
    return 0;
}


