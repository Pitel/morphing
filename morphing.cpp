#include <cstdlib>
//#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "cvmorph.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
	const char *imagename = argc > 1 ? argv[1] : "Lenna.png";
	Mat img = imread(imagename);
	if (!img.data) {
		return EXIT_FAILURE;
	}
	
	Mat out(img.size(), img.type());
	Mat grid1(2, 2, DataType<Point>::type);
	Mat grid2(2, 2, DataType<Point>::type);
	
	grid1.at<Point>(0, 0) = Point(150, 150);
	grid1.at<Point>(0, 1) = Point(250, 100);
	grid1.at<Point>(1, 0) = Point(100, 300);
	grid1.at<Point>(1, 1) = Point(300, 250);
	
	grid2.at<Point>(0, 0) = Point(100, 100);
	grid2.at<Point>(0, 1) = Point(300, 100);
	grid2.at<Point>(1, 0) = Point(100, 300);
	grid2.at<Point>(1, 1) = Point(300, 300);
	
	morph(img, img, out, grid1, grid2, 0.5);
	imshow("input", img);
	imshow("output", out);
	
	waitKey();
	return EXIT_SUCCESS;
}
