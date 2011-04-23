#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "cvmorph.hpp"

using namespace std;
using namespace cv;

void morph(const Mat &img1, const Mat &img2, Mat &out, const Mat grid1, const Mat grid2, const float morp) {
	Point2f tile1[4];
	tile1[0] = grid1.at<Point2f>(0, 0);
	tile1[1] = grid1.at<Point2f>(0, 1);
	tile1[2] = grid1.at<Point2f>(1, 0);
	tile1[3] = grid1.at<Point2f>(1, 1);
	
	Point2f tile2[4];
	tile2[0] = grid2.at<Point2f>(0, 0);
	tile2[1] = grid2.at<Point2f>(0, 1);
	tile2[2] = grid2.at<Point2f>(1, 0);
	tile2[3] = grid2.at<Point2f>(1, 1);
	
	warpPerspective(img1, out, getPerspectiveTransform(tile1, tile2), out.size(), QUALITY, BORDER_TRANSPARENT);
}
