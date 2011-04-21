#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

/**
 * velikost bloku bude 8x8 pixelu
 */
static const unsigned short N = 8;

typedef struct{
	float x, y;
} souradnice;
/**
 * demonstrace komprese obrazu pomoci DCT
 */
static void demo (const Mat &input){

	int pocet=5;
	souradnice grid1[6][6];
	souradnice grid2[6][6];
	float width = input.size().width;
	float height = input.size().height;
	for(int i=0; i<=pocet; i++){
		for(int j=0; j<=pocet; j++){
			grid1[i][j].x = (width/((float)pocet))*i;
			grid1[i][j].y = (height/((float)pocet))*j;
			grid2[i][j].x = (width/((float)pocet))*i;
			grid2[i][j].y = (height/((float)pocet))*j;
		}
	}
	grid2[1][1].x = 50;
	grid2[1][1].y = 50;

	Point2f body[4];
	body[0] = Point2f(0,0);
	body[1] = Point2f(500,0);
	body[2] = Point2f(500,500);
	body[3] = Point2f(0,500);

	Point2f body2[4];
	body2[0] = Point2f(100,0);
	body2[1] = Point2f(400,0);
	body2[2] = Point2f(200,300);
	body2[3] = Point2f(0,400);

	Mat trans = getPerspectiveTransform(body, body2);
	cout << "AAAAAAAAAA" << trans.rows << ", " << trans.cols << endl;

	Mat output(input.rows, input.cols, DataType<float>::type);
	/*trans.at<float>(0,0) = 0.5;
	trans.at<float>(1,0) = 0.0;
	trans.at<float>(2,0) = 0.0;
	trans.at<float>(0,1) = -0.37;
	trans.at<float>(1,1) = 0.5;
	trans.at<float>(2,1) = 297.0;
	trans.at<float>(0,2) = 0.0;
	trans.at<float>(1,2) = 0.0;
	trans.at<float>(2,2) = 1.0;*/
	warpPerspective(input, output, trans, output.size(), WARP_INVERSE_MAP);
	// cely postup zobrazit
	imshow ("input",      Mat_<uchar> (input) );
	imshow ("output",      Mat_<uchar> (output) );
	//imshow ("transform",  abs (coeff/15) );
	//imshow ("quantized",  abs (quant/15) );
	//imshow ("decoded",    Mat_<uchar> (decod) );
	//imshow ("difference", Mat_<uchar> ( abs (input-decod) ) );
}

int main (int argc, char **argv)
{
	const char *imagename = argc > 1
		? argv[1] : "Lenna.png";

	cout << "Loading " << imagename << endl;

	Mat img = Mat_<float> ( imread (imagename) );
	if (!img.data)
		return -1;

	demo (img);

	waitKey ();

	return 0;
}
