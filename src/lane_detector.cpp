#include <iostream>
//#include <opencv2/core/utility.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
 
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
            cv::Canny(imgGray,imgCanny,140,255);
            
            // Create masked image
            cv::Mat imgMasked;
            Mask(imgCanny, imgMasked, w, h);
            
            // Probabilistic line transform
            std::vector<cv::Vec4i> linesP;
            //cv::HoughLinesP(imgMasked, linesP, 1, CV_PI/180, 50, 50, 10);
            cv::HoughLinesP(imgMasked, linesP, 1, CV_PI/180, 20, 20, 30);
            
            // Filter and sort lines
            std::vector<double> slopes;
            double x0, x1, y0, y1;
            double slopeThresh = 0.3;
            std::vector<cv::Vec4i> linesL;
            std::vector<cv::Vec4i> linesR;
            for(size_t i=0; i<linesP.size(); i++)
            {
                x0 = linesP[i][0];
                y0 = linesP[i][1];
                x1 = linesP[i][2];
                y1 = linesP[i][3];
                slopes.push_back((y1-y0)/(x1-x0));
                //std::cout << linesP[i] << std::endl;
                //std::cout << slopes[i] << std::endl;
                if (slopes[i] > slopeThresh)
                {
                    linesL.push_back(linesP[i]);
                    //std::cout << "left line"<< std::endl;
                }
                else if (slopes[i] < -slopeThresh)
                {
                    linesR.push_back(linesP[i]);
                    //std::cout << "right line"<< std::endl;
                }
                else
                {
                    //std::cout << "line discarded"<< std::endl;
                }
            }
            
            // Regression of left line
            cv::Vec4i l;
            std::vector<cv::Point> pointsL; 
            cv::Vec4d lineLfit;
            double left_m;
            cv::Point left_b[1][1];

            for(size_t i=0; i<linesL.size(); i++)
            {                
                l = linesL[i];
                pointsL.push_back(cv::Point(l[0], l[1]));
                pointsL.push_back(cv::Point(l[2], l[3]));
            }            
            cv::fitLine(pointsL, lineLfit, cv::DIST_L2, 0, 0.01, 0.01);
            left_m = lineLfit[1]/lineLfit[0];
            left_b[0][0] = cv::Point(lineLfit[2], lineLfit[3]);
            
            // Regression of right line
            std::vector<cv::Point> pointsR;
            cv::Vec4d lineRfit;
            double right_m;
            cv::Point right_b[1][1];
            
            for(size_t i=0; i<linesR.size(); i++)
            {                
                l = linesR[i];
                pointsR.push_back(cv::Point(l[0], l[1]));
                pointsR.push_back(cv::Point(l[2], l[3]));
            }
            cv::fitLine(pointsR, lineRfit, cv::DIST_L2, 0, 0.01, 0.01);            
            right_m = lineRfit[1]/lineRfit[0];
            right_b[0][0] = cv::Point(lineRfit[2],lineRfit[3]);
            
            // Create left and right line from regression
            std::cout << left_m << "," << left_b[0][0] << std::endl;
            std::cout << right_m << "," << right_b[0][0] << std::endl;
            
            // Draw lines
            cv::Mat imgLines;
            cv::cvtColor(imgMasked, imgLines, cv::COLOR_GRAY2RGB);
            
            imgLines = img;
            for(size_t i=0; i<linesP.size(); i++)
            {
                l = linesP[i];
                line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(0,0,255), 3, cv::LINE_AA);
            }            
            for(size_t i=0; i<linesL.size(); i++)
            {
                l = linesL[i];
                line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(255,0,0), 3, cv::LINE_AA);
            }
            for(size_t i=0; i<linesR.size(); i++)
            {
                l = linesR[i];
                line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(255,0,0), 3, cv::LINE_AA);
            }
            //std::cout << lineRfit << std::endl;
            //l = lineRfit[0];
            //line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(0,255,0), 3, cv::LINE_AA);
            //l = lineRfit[0];
            //line(imgLines, cv::Point(l[0],l[1]), cv::Point(l[2],l[3]), cv::Scalar(0,255,0), 3, cv::LINE_AA);
            
        
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