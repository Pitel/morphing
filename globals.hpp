#ifndef MORPH_GLOBALS_H
#define MORPH_GLOBALS_H

#include <cstddef>
#include <gtk/gtk.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


#include <iostream>
using namespace cv;

#define GRID_MAX 500


typedef struct grd
{
    int xline;  //Pocet car na ose x
    int yline;  //Pocet car na ose y
} TGrid;



typedef struct imgs
{
    //IplImage    *ocvImage;      //Image ktery se potom prevede na gtk image
    Mat         ocvMatImage;   //Matice ktera se nacte pri otevreni a predava se do fce morph
    Mat         ocvMatGrid;    //Matice uchovavajici mrizku GRID
    Point grid[GRID_MAX];       //TODO - predelat na allokovani pole nebo z C++
    gboolean is_source;         // Source || Destination
    TGrid       *grid_size;     //Velikos mrizky

} TImgData;


typedef struct play
{
   // GtkWidget *hscale; //slider
    GtkWidget *imageGTK;
    Mat     **images; //array of images
    gboolean   playing;
} TPLay;



Mat get_grid_mat_from_imgdata (TImgData *idata);

//Vynuluje mrizku
void imgdata_grid_null (TImgData *idata);

void imgdata_grid_default (TImgData *idata);

void ocvMat2gtkImg (Mat &opencvImage, GtkWidget **img);

void ocvCopyImg (IplImage **src, IplImage **dest);

#endif
