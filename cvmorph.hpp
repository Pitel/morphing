#include <cv.h>

using namespace cv;

#ifndef CVMORPH_H
#define CVMORPH_H

#define QUALITY INTER_LANCZOS4
void morph(const Mat &img1, const Mat &img2, Mat &out);

#endif
