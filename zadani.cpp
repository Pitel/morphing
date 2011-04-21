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

/**
 * dopredna 2D DCT
 */
static void fdct2D (const Mat& src, Mat& dst) {
	for (unsigned short v = 0; v < N; v++) {
		for (unsigned short u = 0; u < N; u++) {
			double g = 0;
			double au = sqrt(2.0 / N);
			if (u == 0) au = sqrt(1.0 / N);
			double av = sqrt(2.0 / N);
			if (v == 0) av = sqrt(1.0 / N);
			for (unsigned short y = 0; y < N; y++) {
				for (unsigned short x = 0; x < N; x++) {
					g += au * av * src.at<double>(y, x) * cos((M_PI / N) * (x + .5) * u) * cos((M_PI / N) * (y + .5) * v);
				}
			dst.at<double>(v, u) = g;
			}
		}
	}
}

/**
 * zpetna 2D DCT
 */
static void idct2D (const Mat& src, Mat& dst) {
	for (unsigned short y = 0; y < N; y++) {
		for (unsigned short x = 0; x < N; x++) {
			double f = 0;
			for (unsigned short v = 0; v < N; v++) {
				for (unsigned short u = 0; u < N; u++) {
					double au = sqrt(2.0 / N);
					if (u == 0) au = sqrt(1.0 / N);
					double av = sqrt(2.0 / N);
					if (v == 0) av = sqrt(1.0 / N);
					f += au * av * src.at<double>(v, u) * cos((M_PI / N) * (x + .5) * u) * cos((M_PI / N) * (y + .5) * v);
				}
			dst.at<double>(y, x) = f;
			}
		}
	}
}

/**
 * kvantizace koeficientu
 */
static void quantize (const Mat& src, Mat& dst) {
	const unsigned short Q[N][N] = {
		{16, 11, 10, 16,  24,  40,  51,  61},
		{12, 12, 14, 19,  26,  58,  60,  55},
		{14, 13, 16, 24,  40,  57,  69,  56},
		{14, 17, 22, 29,  51,  87,  80,  62},
		{18, 22, 37, 56,  68, 109, 103,  77},
		{24, 35, 55, 64,  81, 104, 113,  92},
		{49, 64, 78, 87, 103, 121, 120, 101},
		{72, 92, 95, 98, 112, 100, 103,  99}};
	for (unsigned short i = 0; i < N; i++) {
		for (unsigned short j = 0; j < N; j++) {
			//dst.at<double>(i, j) = src.at<double>(2, 2);
			dst.at<double>(i, j) = round(src.at<double>(i, j) / Q[i][j]) * Q[i][j];
		}
	}
}

/**
 * demonstrace komprese obrazu pomoci DCT
 */
static void demo (const Mat &input)
{
	// sem se ulozi koeficienty DCT
	Mat coeff (input.rows, input.cols, DataType<double>::type);

	// sem se ulozi koeficienty po provedeni kvantizace
	Mat quant (input.rows, input.cols, DataType<double>::type);

	// a tady bude dekodovany obrazek
	Mat decod (input.rows, input.cols, DataType<double>::type);

	// zpracujeme obraz po blocich
	for (int y = 0; y < input.rows; y+=N)
		for (int x = 0; x < input.cols; x+=N)
		{
			// aktualni zpracovavany blok bude tento vyrez celeho obrazu
			Rect rect (x, y, N, N);

			// pomocne promenne, ktere reprezentuji prislusny vyrez
			Mat inputBlock (input, rect);
			Mat coeffBlock (coeff, rect);
			Mat quantBlock (quant, rect);
			Mat decodBlock (decod, rect);

			// nejprve transformujeme blok obrazku na koeficienty pomoci DCT
			fdct2D (inputBlock, coeffBlock);

			// pak provedeme kvantizaci techto koeficientu (ztrata informace, komprese)
			quantize (coeffBlock, quantBlock);

			// z kvantovanych koeficientu zrekonstruujeme puvodni blok obrazu
			idct2D (quantBlock, decodBlock);
		}

	// cely postup zobrazit
	imshow ("input",      Mat_<uchar> (input) );
	imshow ("transform",  abs (coeff/15) );
	imshow ("quantized",  abs (quant/15) );
	imshow ("decoded",    Mat_<uchar> (decod) );
	imshow ("difference", Mat_<uchar> ( abs (input-decod) ) );
}

int main (int argc, char **argv)
{
	const char *imagename = argc > 1
		? argv[1] : "Lenna.png";

	cout << "Loading " << imagename << endl;

	Mat img = Mat_<double> ( imread (imagename, CV_LOAD_IMAGE_GRAYSCALE) );
	if (!img.data)
		return -1;

	demo (img);

	waitKey ();

	return 0;
}
