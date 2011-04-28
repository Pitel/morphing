#include "globals.hpp"

Mat get_grid_mat_from_imgdata (TImgData *idata)
{
    Mat ret(idata->grid_size->xline, idata->grid_size->yline, DataType<Point>::type);

    for(int y = 0; y < idata->grid_size->yline; y++)
        for(int x = 0; x < idata->grid_size->xline; x++)
        {
//            int xgrid = idata->grid[x+y*grid_xline].x;
//            int ygrid = idata->grid[x+y*grid_xline].y;

//            //preskoci krajni body
//            if(xgrid <= 0 || xgrid >= idata->ocvImage->width)
//                continue;
//            if(ygrid <= 0 || ygrid >= idata->ocvImage->height)
//                continue;

            ret.at<Point>(x, y) = idata->grid[x+y*idata->grid_size->xline];
        }

    return ret;
}

//Vynuluje mrizku
void imgdata_grid_null (TImgData *idata)
{
    for(int i=0; i< GRID_MAX; i++)
    {
        idata->grid[i].x = -1;
        idata->grid[i].y = -1;
    }
}

void imgdata_grid_default (TImgData *idata)
{
    imgdata_grid_null(idata);

    const unsigned int step_x = idata->ocvMatImage.cols / (idata->grid_size->xline - 1);
    const unsigned int step_y = idata->ocvMatImage.rows  / (idata->grid_size->yline - 1);

    for(int y=0; y < idata->grid_size->yline; y++)
        for(int x=0; x < idata->grid_size->xline; x++)
        {
            if (x <= 0)
            {
                idata->grid[x+y*idata->grid_size->xline].x = 0;
            }
            else if (x >= idata->grid_size->xline - 1)
            {
                idata->grid[x+y*idata->grid_size->xline].x = idata->ocvMatImage.cols;
            }
            else
            {
                idata->grid[x+y*idata->grid_size->xline].x = x * step_x;
            }
            if (y <= 0)
            {
                idata->grid[x+y*idata->grid_size->xline].y = 0;
            }
            else if (y >= idata->grid_size->yline - 1)
            {
                idata->grid[x+y*idata->grid_size->xline].y = idata->ocvMatImage.rows;
            }
            else
            {
                idata->grid[x+y*idata->grid_size->xline].y = y * step_y;
            }
        }
}

void ocvMat2gtkImg (Mat &opencvImage, GtkWidget **img)
{
    IplImage *tmp = new IplImage(opencvImage);

    gtk_image_set_from_pixbuf( GTK_IMAGE(*img), gdk_pixbuf_new_from_data((guchar*) tmp->imageData, GDK_COLORSPACE_RGB, FALSE, tmp->depth, tmp->width, tmp->height, (tmp->widthStep), NULL, NULL));
}

void ocvCopyImg (IplImage **src, IplImage **dest)
{
    if((*src) == NULL)
        return;

//    if((*dest) != NULL)
//        cvReleaseImage (dest);

    (*dest) = cvCreateImage(cvSize( (*src)->width, (*src)->height), (*src)->depth, (*src)->nChannels);

    cvCopy(*src, *dest, NULL);
}
