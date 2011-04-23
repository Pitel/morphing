//#include <iostream>
#include <cv.h>
#include "cvmorph.hpp"

using namespace std;
using namespace cv;

void morph(const Mat &img1, const Mat &img2, Mat &out, const Mat grid1, const Mat grid2, const float morp) {
	Point tile1[4];
	tile1[0] = grid1.at<Point>(0, 0);
	tile1[1] = grid1.at<Point>(0, 1);
	tile1[2] = grid1.at<Point>(1, 1);
	tile1[3] = grid1.at<Point>(1, 0);
	Point2f tile1f[4] = {tile1[0], tile1[1], tile1[2], tile1[3]};
	
	Point tile2[4];
	tile2[0] = grid2.at<Point>(0, 0);
	tile2[1] = grid2.at<Point>(0, 1);
	tile2[2] = grid2.at<Point>(1, 1);
	tile2[3] = grid2.at<Point>(1, 0);
	Point2f tile2f[4] = {tile2[0], tile2[1], tile2[2], tile2[3]};
	
	warpPerspective(img1, out, getPerspectiveTransform(tile1f, tile2f), out.size(), QUALITY, BORDER_TRANSPARENT);
	
	Mat mask(out.size(), out.type());
	fillConvexPoly(mask, tile1, 4, CV_RGB(255, 255, 255), CV_AA, 0);
	out &= mask;
}
