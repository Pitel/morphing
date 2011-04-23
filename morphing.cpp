#include <cstdlib>
#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "cvmorph.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
	const char *imagename = argc > 1 ? argv[1] : "Lenna.png";
	cout << "Loading " << imagename << endl;
	Mat img = Mat_<float>(imread(imagename));
	if (!img.data) {
		return EXIT_FAILURE;
	}
	
	Mat out(img.rows, img.cols, DataType<float>::type);
	Mat grid1(2, 2, DataType<Point2f>::type);
	Mat grid2(2, 2, DataType<Point2f>::type);
	
	grid1.at<Point2f>(0, 0) = Point2f(150, 150);
	grid1.at<Point2f>(0, 1) = Point2f(250, 100);
	grid1.at<Point2f>(1, 0) = Point2f(100, 300);
	grid1.at<Point2f>(1, 1) = Point2f(300, 250);
	
	grid2.at<Point2f>(0, 0) = Point2f(100, 100);
	grid2.at<Point2f>(0, 1) = Point2f(300, 100);
	grid2.at<Point2f>(1, 0) = Point2f(100, 300);
	grid2.at<Point2f>(1, 1) = Point2f(300, 300);
	
	morph(img, img, out, grid1, grid2, 0.5);
	imshow ("input", Mat_<uchar>(img));
	imshow ("output", Mat_<uchar>(out));
	
	waitKey();
	return EXIT_SUCCESS;
}
