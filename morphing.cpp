#include <cstdlib>
#include <cstddef> 
#include <iostream>
#include <sstream>

#include <cv.h>
#include <highgui.h>

#include "cvmorph.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
	const char *imagename1 = argc > 1 ? argv[1] : "Lenna.png";
	const char *imagename2 = argc > 2 ? argv[2] : "Lenna.png";
	Mat img1 = imread(imagename1);
	Mat img2 = imread(imagename2);
	if (!img1.data || !img2.data) {
		return EXIT_FAILURE;
	}
	
	Mat out(img1.size(), img1.type());
	
	const unsigned short gridsize = 4;
	Mat grid1(gridsize, gridsize, DataType<Point>::type);
	
	for (unsigned short y = 0; y < gridsize; y++) {
		for (unsigned short x = 0; x < gridsize; x++) {
			grid1.at<Point>(x, y) = Point((img1.cols / (gridsize - 1)) * x, (img1.rows / (gridsize - 1)) * y);
			//cout << x << ' ' << y << ": " << (img.cols / (gridsize - 1)) * x << ' ' << (img.rows / (gridsize - 1)) * y << endl;
		}
	}
	
	Mat grid2 = grid1.clone();
	grid2.at<Point>(1, 1).y = img2.rows / 2;
	grid2.at<Point>(2, 1).x = img2.cols / 2;
	grid2.at<Point>(1, 2).x = img2.cols / 2;
	grid2.at<Point>(2, 2).y = img2.rows / 2;
	
	for (float ratio = 0; ratio <= 1; ratio += 0.25) {
		morph(img1, img2, out, grid1, grid2, ratio);
		stringstream ss;
		ss << ratio;
		imshow(ss.str(), out);
	}
	
	waitKey();
	return EXIT_SUCCESS;
}
