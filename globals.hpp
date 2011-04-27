#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstddef>
#include <gtk/gtk.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>


#include <iostream>
using namespace cv;

#define GRID_MAX 500


static gint grid_xline = 4;
static gint grid_yline = 4;


typedef struct imgs
{
    IplImage    *ocvImage;
    Mat         *ocvMatImage;
    Mat         *ocvMatGrid;
    Point grid[GRID_MAX];       //TODO - predelat na allokovani pole nebo z C++
    gboolean is_source;         // Source || Destination

} TImgData;


static Mat *get_grid_mat_from_imgdata (TImgData *idata)
{
    Mat *ret = new Mat(grid_xline, grid_yline, DataType<Point>::type);

    for(int y = 0; y < grid_yline; y++)
        for(int x = 0; x < grid_xline; x++)
        {
//            int xgrid = idata->grid[x+y*grid_xline].x;
//            int ygrid = idata->grid[x+y*grid_xline].y;

//            //preskoci krajni body
//            if(xgrid <= 0 || xgrid >= idata->ocvImage->width)
//                continue;
//            if(ygrid <= 0 || ygrid >= idata->ocvImage->height)
//                continue;

            ret->at<Point>(x, y) = idata->grid[x+y*grid_xline];
		}

	return ret;
}

//Vynuluje mrizku
static void imgdata_grid_null (TImgData *idata)
{
    for(int i=0; i< GRID_MAX; i++)
    {
        idata->grid[i].x = -1;
        idata->grid[i].y = -1;
    }
}


static void imgdata_grid_default (TImgData *idata)
{
    imgdata_grid_null(idata);

    gint step_x = (float) idata->ocvImage->height / (float) (grid_xline-1);
    gint step_y = (float) idata->ocvImage->width / (float) (grid_yline-1);

    //float x = ((float) idata->ocvImage->height) / ((float) (grid_xline-1));
   // float y = ((float) idata->ocvImage->width) / ((float) (grid_yline-1));

    //std::cout << "[" << grid_xline << "," << grid_yline << "]\t" << x << " " << y << " ------- " << step_x << " " << step_y << std::endl;
    for(int y=0; y < grid_yline; y++)
        for(int x=0; x < grid_xline; x++)
        {
          //  gint a = x*step_y;
          //  gint b = y*step_x;
            idata->grid[x+y*grid_xline].x = (((x*step_y) > idata->ocvImage->width )? idata->ocvImage->width : x*step_x);
            idata->grid[x+y*grid_xline].y = (((y*step_x) > idata->ocvImage->height) ? idata->ocvImage->height : y*step_y);

            //std::cout << a << "," << b << " - [" << idata->grid[x+y*grid_xline].x << "," << idata->grid[x+y*grid_xline].y << "] " << endl;
        }
}



static void ocvImg2gtkImg (IplImage **opencvImage, GtkWidget **img)
{
    gtk_image_set_from_pixbuf( GTK_IMAGE(*img), gdk_pixbuf_new_from_data((guchar*) (*opencvImage)->imageData, GDK_COLORSPACE_RGB, FALSE, (*opencvImage)->depth,
                                                                         (*opencvImage)->width, (*opencvImage)->height, ((*opencvImage)->widthStep), NULL, NULL));
}


static void ocvCopyImg (IplImage **src, IplImage **dest)
{
    if((*dest) != NULL)
        cvReleaseImage(dest);

    (*dest) = cvCreateImage(cvSize( (*src)->width, (*src)->height), (*src)->depth, (*src)->nChannels);

    cvCopy(*src, *dest, NULL);
}


#endif
