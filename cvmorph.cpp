#include <cstddef>
//#include <iostream>
#include <cv.h>
//#include <highgui.h>
#include "cvmorph.hpp"

using namespace std;
using namespace cv;

Point tween(const Point p1, const Point p2, const float r) {
	return Point(p1.x - (p1.x - p2.x) * r, p1.y - (p1.y - p2.y) * r);
}

Mat warp(const Mat &img, const Mat grid1, const Mat grid2, const float ratio) {
	Mat mask(img.size(), img.type());
	Mat tmp(img.size(), img.type());
	Mat out(img.size(), img.type());
	
	for (unsigned short y = 0; y < grid1.cols - 1; y++) {
		for (unsigned short x = 0; x < grid1.rows - 1; x++) {
			//clog << x << ' ' << y << ": " << grid1.at<Point>(x, y).x << ' ' << grid1.at<Point>(x, y).y << endl;
			Point tile1[4];
			tile1[0] = grid1.at<Point>(x     , y    );
			tile1[1] = grid1.at<Point>(x + 1 , y    );
			tile1[2] = grid1.at<Point>(x + 1 , y + 1);
			tile1[3] = grid1.at<Point>(x     , y + 1);
			Point2f tile1f[4] = {tile1[0], tile1[1], tile1[2], tile1[3]};
			
			Point tile2[4];
			tile2[0] = tween(tile1[0], grid2.at<Point>(x    , y    ), ratio);
			tile2[1] = tween(tile1[1], grid2.at<Point>(x + 1, y    ), ratio);
			tile2[2] = tween(tile1[2], grid2.at<Point>(x + 1, y + 1), ratio);
			tile2[3] = tween(tile1[3], grid2.at<Point>(x    , y + 1), ratio);
			Point2f tile2f[4] = {tile2[0], tile2[1], tile2[2], tile2[3]};
			
			// Perspektivni korekce
			warpPerspective(img, tmp, getPerspectiveTransform(tile1f, tile2f), tmp.size());
			
			// Maskovani
			mask = Mat::zeros(mask.size(), mask.type());
			fillConvexPoly(mask, tile2, 4, CV_RGB(255, 255, 255), CV_AA, 0);
			out += mask & tmp;
			
			/*
			addWeighted(tmp, 1, mask, 0.5, 0, tmp);
			imshow("debug", tmp);
			waitKey();
			*/
		}
	}
	
	return out;
}

void morph(const Mat &img1, const Mat &img2, Mat &out, const Mat grid1, const Mat grid2, const float ratio) {
	out = Mat::zeros(out.size(), out.type());
	
	Mat w1 = warp(img1, grid1, grid2, ratio);
	Mat w2 = warp(img2, grid2, grid1, 1 - ratio);
	
	// Blending
	addWeighted(w1, 1 - ratio, w2, ratio, 0, out);
}
