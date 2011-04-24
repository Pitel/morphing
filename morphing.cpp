#include <cstdlib>
#include <iostream>

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
	
	//FIXME Kdyz zamenim <= za <, tak SIGABRT nenastane. Ale je mi fakt divny, proc to failne uz u prvni dlazdice, ktera by mela byt na tech nerovnostech nezavisla. :/
	for (unsigned short y = 0; y <= gridsize; y++) {
		for (unsigned short x = 0; x <= gridsize; x++) {
			grid1.at<Point>(x, y) = Point((img.cols / gridsize) * x, (img.rows / gridsize) * y);
			cout << x << ' ' << y << ": " << (img.cols / gridsize) * x << ' ' << (img.rows / gridsize) * y << endl;
		}
	}
	
	Mat grid2 = grid1.clone();
	
	morph(img, img, out, grid1, grid2, 0.5);
	imshow("input", img);
	imshow("output", out);
	
	waitKey();
	return EXIT_SUCCESS;
}
