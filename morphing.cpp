#include <cstdlib>
#include <iostream>

#include <cv.h>
#include <highgui.h>

#include "cvmorph.hpp"

using namespace std;
using namespace cv;

int main (int argc, char **argv) {
	const char *imagename = argc > 1 ? argv[1] : "Lenna.png";
	cout << "Loading " << imagename << endl;
	Mat img = Mat_<float>(imread (imagename));
	if (!img.data) {
		return EXIT_FAILURE;
	}
	morph(img);
	waitKey();
	return EXIT_SUCCESS;
}
