# lane_finding

This repository contains a lane line detection code using OpenCV and C++.

## Functionality

The primary functions of the algorithm are to
* perform convolution to denoise image,
* use Canny edge detection and probabalistic Hough line transforms to extract lane line candidates,
* filter and sort lane line candidates to form a left and right lane line, and
* basic video/image processing, reading, and writing capabilties. 

## Future Work

The algorithm has not been tested on a diverse data set yet and several gaps could be addressed such as
* image distortion calibration by collecting chessboard data with car camera,
* several algorithm parameters were manually tuned which could be automated, and
* finding curvature of the lanes for turn detection using a perspective transform.
