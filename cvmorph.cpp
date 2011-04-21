#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "cvmorph.hpp"

using namespace std;
using namespace cv;

void morph(const Mat &img1, const Mat &img2, Mat &out) {
	const unsigned short pocet = 5;
	Point grid1[pocet + 1][pocet + 1];
	Point grid2[pocet + 1][pocet + 1];
	float width = img1.size().width;
	float height = img1.size().height;
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
	
	warpPerspective(img1, out, trans, out.size(), WARP_INVERSE_MAP | QUALITY, BORDER_TRANSPARENT);
}
