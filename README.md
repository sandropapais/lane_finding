# lane_finding

This repository contains a lane line detection code using OpenCV and C++.

## Functionality

The primary functions of the algorithm are to
* perform convolution to denoise image,
* use color transforms, gradients, and edge detection techniques to create a binary image,
* use probabalistic Hough line transforms to extract lane line candidates,
* filter and sort lane line candidates to form a left and right lane line,
* basic video/image processing, reading, and writing capabilties. 

An example image of the results is shown below and a video can be found at lane_finding/output/test.avi.

![picture alt](https://i.ibb.co/WH7JCDj/lane-finding-capture.png "Title is optional")

## Future Work

The algorithm has not been tested on a diverse data set yet and several gaps could be addressed such as
* image distortion calibration by collecting chessboard data with car camera,
* several algorithm parameters were manually tuned which could be automated,
* finding curvature of the lanes for turn detection using a perspective transform.



