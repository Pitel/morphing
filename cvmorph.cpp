#include <cstddef>
//#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "cvmorph.hpp"

using namespace std;
using namespace cv;

void morph(const Mat &img1, const Mat &img2, Mat &out, const Mat grid1, const Mat grid2, const float ratio) {
	for (unsigned short y = 0; y < grid1.cols - 1; y++) {
		for (unsigned short x = 0; x < grid1.rows - 1; x++) {
			//clog << x << ' ' << y << ": " << grid1.at<Point>(x, y).x << ' ' << grid1.at<Point>(x, y).y << endl;
			Point tile1[4];
			tile1[0] = grid1.at<Point>(x     , y);
			tile1[1] = grid1.at<Point>(x + 1 , y);
			tile1[2] = grid1.at<Point>(x + 1 , y + 1);
			tile1[3] = grid1.at<Point>(x     , y + 1);
			Point2f tile1f[4] = {tile1[0], tile1[1], tile1[2], tile1[3]};
			
			Point tile2[4];
			tile2[0] = grid2.at<Point>(x    , y);
			tile2[1] = grid2.at<Point>(x + 1, y);
			tile2[2] = grid2.at<Point>(x + 1, y + 1);
			tile2[3] = grid2.at<Point>(x    , y + 1);
			Point2f tile2f[4] = {tile2[0], tile2[1], tile2[2], tile2[3]};
			
			// Perspektivni korekce
			warpPerspective(img1, out, getPerspectiveTransform(tile1f, tile2f), out.size(), QUALITY);
			
			// Maskovani
			Mat mask(out.size(), out.type());
			fillConvexPoly(mask, tile1, 4, CV_RGB(255, 255, 255), CV_AA, 0);
			out &= mask;
			
			imshow("debug", out);
			waitKey();
			
			// Blending
			//addWeighted(out, 1 - ratio, mask, ratio, 0, out);
		}
	}
}
