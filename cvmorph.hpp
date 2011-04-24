#ifndef CVMORPH_H
#define CVMORPH_H

#include <cstddef>
#include <cv.h>

using namespace cv;

#define QUALITY INTER_LANCZOS4	//http://opencv.willowgarage.com/documentation/cpp/imgproc_geometric_image_transformations.html#resize
void morph(const Mat &img1, const Mat &img2, Mat &out, const Mat grid1, const Mat grid2, const float ratio);

#endif
