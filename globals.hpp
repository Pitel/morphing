#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstddef>
#include <cstdlib>
#include <gtk/gtk.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

typedef struct mp
{
    gint x;
    gint y;
} myPoint;

typedef struct imgs
{
    IplImage *ocvImage;
    myPoint grid[100];      //TODO - predelat na allokovani pole nebo z C++
                            //a taky tady nefungoval point z opencv takze mozna predelat
    gboolean is_source; // Source || Destination

} TImgData;



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
