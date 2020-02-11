void Mask(cv::Mat img, cv::Mat &imgMasked, int w, int h);
void sortLines(std::vector<cv::Vec4i> linesP,   // input
            std::vector<cv::Vec4i> &linesL,     // outputs
            std::vector<cv::Vec4i> &linesR, 
            double slopeThresh);                // parameter
void regressLines(std::vector<cv::Vec4i> lines, cv::Point &lanept0, cv::Point &lanept1, int h);
cv::Mat lanefinding(cv::Mat &img);