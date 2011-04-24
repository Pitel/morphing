#include <cstdlib>
#include <cstddef> 
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
	
	const unsigned short gridsize = 4;
	Mat grid1(gridsize, gridsize, DataType<Point>::type);
	
	for (unsigned short y = 0; y < gridsize; y++) {
		for (unsigned short x = 0; x < gridsize; x++) {
			grid1.at<Point>(x, y) = Point((img.cols / (gridsize - 1)) * x, (img.rows / (gridsize - 1)) * y);
			//cout << x << ' ' << y << ": " << (img.cols / (gridsize - 1)) * x << ' ' << (img.rows / (gridsize - 1)) * y << endl;
		}
	}
	
	Mat grid2 = grid1.clone();
	grid2.at<Point>(1, 1).y = img.rows / 2;
	grid2.at<Point>(2, 1).x = img.cols / 2;
	grid2.at<Point>(1, 2).x = img.cols / 2;
	grid2.at<Point>(2, 2).y = img.rows / 2;
	
	morph(img, img, out, grid1, grid2, 0.5);
	imshow("input", img);
	imshow("output", out);
	
	waitKey();
	return EXIT_SUCCESS;
}
