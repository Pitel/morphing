#include <cstddef>
#include <cstdlib>
#include <gtk/gtk.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>

#include "cvmorph.hpp"
#include "gui_setgridwnd.hpp"
#include "globals.hpp"

GtkWidget *win;
GtkWidget *scrolled_window = NULL;
GtkWidget *label_source = NULL;
GtkWidget *label_dest = NULL;
GtkWidget *label_grid = NULL;
GtkWidget *label_steps = NULL;
GtkWidget *hscale = NULL;

pthread_t playeThread;

Mat opencvMatImage;
Mat opencvMatImageZOOM;
GtkWidget *image = NULL;

TGrid grid = {4,4};

TImgData src_imgdata, dst_imgdata;

TPLay play_data;

int zoom = 100;
gboolean bestfit = FALSE;

#define RANGE_MIN   0
#define RANGE_STEP  1
int RANGE_MAX = 10;

static void zoom_image();
static void zoomfit();
int show_message (GtkMessageType type, const gchar *format, gchar *msgtxt, const gchar *format_sec, gchar *msgtxt_sec);

static void
show_morph_image (float ratio = 0.0)
{
    if(src_imgdata.ocvMatImage.data == NULL || dst_imgdata.ocvMatImage.data == NULL)
        return;

    morph((src_imgdata.ocvMatImage), (dst_imgdata.ocvMatImage), opencvMatImage, (src_imgdata.ocvMatGrid), (dst_imgdata.ocvMatGrid), ratio);

//    if(!opencvImage)
//        cvReleaseImage(&opencvImage);
//
//    opencvImage = new IplImage(opencvMatImage);
//
//    ocvCopyImg(&opencvImage, &opencvImageGTK);

    if(bestfit)
    {
        zoomfit();
    }
    else if (zoom != 100)
    {
        zoom_image();
    }
    else
    {
        ocvMat2gtkImg(opencvMatImage, &image);
    }

//		imshow("src", *src_imgdata.ocvMatImage);
//		imshow("dst", *dst_imgdata.ocvMatImage);
//		imshow("out", *opencvMatImage);

//	for (float ratio = 0; ratio <= 1; ratio += 0.25) {
//        morph(*(src_imgdata.ocvMatImage), *(dst_imgdata.ocvMatImage), *(opencvMatImage), *(src_imgdata.ocvMatGrid), *(dst_imgdata.ocvMatGrid), ratio);
//		stringstream ss;
//		ss << ratio;
//		imshow(ss.str(), *opencvMatImage);
//	}


}


static void
change_corrent_step (GtkWidget *range, gpointer)
{
    gtk_label_set_text (GTK_LABEL(label_grid), g_strdup_printf("%dx%d; %d points", grid.xline, grid.yline, grid.xline*grid.yline));


    if(src_imgdata.ocvMatImage.data == NULL || dst_imgdata.ocvMatImage.data == NULL)
        return;

    float ratio = gtk_range_get_value (GTK_RANGE(range)) / RANGE_MAX;

    show_morph_image(ratio);
}



void *play_thread (void *p)
{

    TPLay *tmp = (TPLay *)p;
    int i = 0;

    while(tmp->playing)
    {
        gdk_threads_enter();
        //tmpM = tmp->images[i];


       // gchar *n = g_strdup_printf(".zpotmpimg%d.jpg",i);

      //  gtk_image_set_from_file(GTK_IMAGE(tmp->imageGTK), n);

        //gtk_image_set_from_pixbuf(GTK_IMAGE(tmp->imageGTK), gtk_image_get_pixbuf(GTK_IMAGE(img)));
        //ocvMat2gtkImg(*tmpM, &(tmp->imageGTK));


//        float ratio = (float) i/RANGE_MAX;
//
//        morph((src_imgdata.ocvMatImage), (dst_imgdata.ocvMatImage), opencvMatImage, (src_imgdata.ocvMatGrid), (dst_imgdata.ocvMatGrid), ratio);
//        ocvMat2gtkImg(opencvMatImage, &(tmp->imageGTK));
//
//
//
//        usleep(2000);


        //gtk_range_set_value (GTK_RANGE(hscale), i);

	   gchar *n = g_strdup_printf(".zpotmpimg%d.jpg",i);

       gtk_image_set_from_file(GTK_IMAGE(tmp->imageGTK), n);
       //gtk_image_set_from_file(GTK_IMAGE(tmp->imageGTK), n);


        i++;

        if(i > RANGE_MAX)
        {
            i = 0;
            //gdk_window_process_updates(GDK_WINDOW(tmp->imageGTK->window), TRUE);

        }
        gdk_window_invalidate_rect (GDK_WINDOW(tmp->imageGTK->window), NULL, TRUE);

        gdk_threads_leave();
        sleep(1);

		//gdk_window_process_updates (GDK_WINDOW(tmp->wnd->window), TRUE);

    }

    pthread_exit (NULL);
    return NULL;
}


static void
stop_click ()
{
    if(play_data.playing)
    {
        play_data.playing = FALSE;

        for(int i = 0; i <= RANGE_MAX; i++)
        {
            const gchar *n = g_strdup_printf(".zpotmpimg%d.jpg",i);

            remove(n);

        }

    }
}


static void
play_click ()
{
    if(src_imgdata.ocvMatImage.data == NULL || dst_imgdata.ocvMatImage.data == NULL)
        return;

    if(!play_data.playing)
    {

       // play_data.images = //g_array_new(FALSE,FALSE, sizeof(Mat));

      // Mat *imgs[RANGE_MAX+1];

//       play_data.images = new Mat *[RANGE_MAX+1];
//
//
        for(int i = 0; i <= RANGE_MAX; i++)
        {
            Mat *tmpMat = new Mat(opencvMatImage);

            //*tmpMat = opencvMatImage.clone();


            float ratio = (float) i/RANGE_MAX;

            morph((src_imgdata.ocvMatImage), (dst_imgdata.ocvMatImage), *tmpMat, (src_imgdata.ocvMatGrid), (dst_imgdata.ocvMatGrid), ratio);
            //ocvMat2gtkImg(tmpMat, &img);

            //play_data.images[i] = tmpMat;

            IplImage *tmp = new IplImage(*tmpMat);
            cvCvtColor(tmp, tmp, CV_BGR2RGB);

           // GtkWidget *img = gtk_image_new ();
           // ocvMat2gtkImg(tmpMat, &img);

            //gtk_image_set_from_pixbuf(GTK_IMAGE(img), gdk_pixbuf_new_from_data((guchar*) tmp->imageData, GDK_COLORSPACE_RGB, FALSE, tmp->depth, tmp->width, tmp->height, (tmp->widthStep), NULL, NULL));

           // g_array_append_val(play_data.images, tmpMat);

            gchar *n = g_strdup_printf(".zpotmpimg%d.jpg",i);

            cvSaveImage(n, tmp,0);
        }

      //  play_data.images = &imgs;

        play_data.playing = TRUE;
        pthread_create (&playeThread, NULL, play_thread, &play_data);
    }

}



void show_set_steps ()
{
    GtkWidget *dialog, *label, *content_area, *halign, *hsep, *entry_rangemax, *vbox;

    gchar *command = g_strdup_printf("%d", RANGE_MAX);

    /* Create the widgets */
    dialog = gtk_dialog_new_with_buttons ("Set number of steps",
                                          GTK_WINDOW (win),
                                          GTK_DIALOG_DESTROY_WITH_PARENT,
                                          GTK_STOCK_APPLY, GTK_RESPONSE_APPLY,
                                          GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE,
                                          NULL);

    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

    vbox = gtk_vbox_new(FALSE,6);
    gtk_container_add (GTK_CONTAINER (content_area), vbox);

    gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);

    halign = gtk_alignment_new(0, 0, 0, 1);
    label = gtk_label_new ("Number of morphing steps (default 10):");
    gtk_container_add(GTK_CONTAINER(halign), label);
    gtk_box_pack_start (GTK_BOX (vbox),  halign,  FALSE, TRUE, 0);


    /* ----- */
    hsep = gtk_hseparator_new();
    gtk_box_pack_start (GTK_BOX (vbox), hsep,  FALSE, TRUE, 0);


    entry_rangemax = gtk_entry_new ();
    gtk_box_pack_start (GTK_BOX (vbox), entry_rangemax,  FALSE, TRUE, 0);
    gtk_entry_set_text (GTK_ENTRY(entry_rangemax), command);
    //gtk_entry_set_width_chars (GTK_ENTRY(entry_rangemax), strlen);

    /* ----- */
    hsep = gtk_hseparator_new();
    gtk_box_pack_start (GTK_BOX (vbox), hsep,  FALSE, TRUE, 0);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_APPLY)
    {

        RANGE_MAX = atoi(gtk_entry_get_text (GTK_ENTRY(entry_rangemax)));

        if (RANGE_MAX <= 0)
            RANGE_MAX = 10;

        gchar *tmp = g_strdup_printf("%d", RANGE_MAX);

        gtk_label_set_text (GTK_LABEL(label_steps), tmp);

        gtk_range_set_range (GTK_RANGE(hscale), RANGE_MIN, RANGE_MAX);


        g_free(tmp);
    }

    g_free(command);
    gtk_widget_destroy (dialog);
}



void
save_file()
{
    if(opencvMatImage.data == NULL)
        return;

    GtkFileFilter * fimg, * fall;

    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Save image as...",
                                          GTK_WINDOW (win),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                          NULL);

    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);

    fimg = gtk_file_filter_new();
    gtk_file_filter_set_name(fimg,"Images");
    gtk_file_filter_add_mime_type (fimg, "image/png");
    gtk_file_filter_add_mime_type (fimg, "image/jpeg");
    gtk_file_filter_add_mime_type (fimg, "image/gif");
    gtk_file_filter_add_mime_type (fimg, "image/tiff");
    gtk_file_filter_add_mime_type (fimg, "image/bmp");
    gtk_file_filter_add_mime_type (fimg, "image/x-cmu-raster");
    gtk_file_filter_add_mime_type (fimg, "image/x-portable-bitmap");
    gtk_file_filter_add_mime_type (fimg, "image/x-portable-graymap");
    gtk_file_filter_add_mime_type (fimg, "image/x-portable-pixmap");
    gtk_file_filter_add_mime_type (fimg, "image/x-cmu-raster");

    gtk_file_filter_add_pattern(fimg,"*.bmp");
    gtk_file_filter_add_pattern(fimg,"*.jpg");
    gtk_file_filter_add_pattern(fimg,"*.jpeg");
    gtk_file_filter_add_pattern(fimg,"*.jpe");
    gtk_file_filter_add_pattern(fimg,"*.png");
    gtk_file_filter_add_pattern(fimg,"*.pbm");
    gtk_file_filter_add_pattern(fimg,"*.pgm");
    gtk_file_filter_add_pattern(fimg,"*.ppm");
    gtk_file_filter_add_pattern(fimg,"*.ras");
    gtk_file_filter_add_pattern(fimg,"*.tiff");
    gtk_file_filter_add_pattern(fimg,"*.tif");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),fimg);

    fall = gtk_file_filter_new();
    gtk_file_filter_set_name(fall,"All files");
    gtk_file_filter_add_pattern(fall,"*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),fall);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

        IplImage *opencvImage = new IplImage(opencvMatImage);
        cvSaveImage(filename, opencvImage, 0);

        g_free (filename);
    }
    gtk_widget_destroy (dialog);

    return;

}


gboolean image_size_test ()
{

    if(src_imgdata.ocvMatImage.data != NULL && dst_imgdata.ocvMatImage.data != NULL)
        if((src_imgdata.ocvMatImage.cols != dst_imgdata.ocvMatImage.cols) || (src_imgdata.ocvMatImage.rows != dst_imgdata.ocvMatImage.rows))
        {
            gchar *txt1 = g_strdup("INFO: Source and destination image has different size!");
            gchar *txt2 = g_strdup("Press OK to resize destination image\nPress CANCEL to cancel opening image");

            if(show_message (GTK_MESSAGE_INFO, "%s",txt1,"%s",txt2))
            {
                Mat tmp = dst_imgdata.ocvMatImage.clone();
                dst_imgdata.ocvMatImage.release();
                resize(tmp, dst_imgdata.ocvMatImage, src_imgdata.ocvMatImage.size(), 0, 0, INTER_LINEAR);

                //Oprava mrizky u resiznute matice
                imgdata_grid_default(&dst_imgdata); //nejdrive spocitat mrizku
                dst_imgdata.ocvMatGrid = get_grid_mat_from_imgdata(&dst_imgdata); //Pak mrizku dat do matice


            }
            else
                return FALSE;

            g_free(txt1);
            g_free(txt2);
        }

    return TRUE;
}


void
open_file (gpointer, TImgData *img)
{
    GtkWidget *dialog;

    GtkFileFilter * fimg, * fall;

    dialog = gtk_file_chooser_dialog_new ("Open image file...",
                                          GTK_WINDOW (win),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

    fimg = gtk_file_filter_new();
    gtk_file_filter_set_name(fimg,"Images");
    gtk_file_filter_add_mime_type (fimg, "image/png");
    gtk_file_filter_add_mime_type (fimg, "image/jpeg");
    gtk_file_filter_add_mime_type (fimg, "image/gif");
    gtk_file_filter_add_mime_type (fimg, "image/tiff");
    gtk_file_filter_add_mime_type (fimg, "image/bmp");
    gtk_file_filter_add_mime_type (fimg, "image/x-cmu-raster");
    gtk_file_filter_add_mime_type (fimg, "image/x-portable-bitmap");
    gtk_file_filter_add_mime_type (fimg, "image/x-portable-graymap");
    gtk_file_filter_add_mime_type (fimg, "image/x-portable-pixmap");
    gtk_file_filter_add_mime_type (fimg, "image/x-cmu-raster");


    gtk_file_filter_add_pattern(fimg,"*.bmp");
    gtk_file_filter_add_pattern(fimg,"*.jpg");
    gtk_file_filter_add_pattern(fimg,"*.jpeg");
    gtk_file_filter_add_pattern(fimg,"*.jpe");
    gtk_file_filter_add_pattern(fimg,"*.png");
    gtk_file_filter_add_pattern(fimg,"*.pbm");
    gtk_file_filter_add_pattern(fimg,"*.pgm");
    gtk_file_filter_add_pattern(fimg,"*.ppm");
    gtk_file_filter_add_pattern(fimg,"*.ras");
    gtk_file_filter_add_pattern(fimg,"*.tiff");
    gtk_file_filter_add_pattern(fimg,"*.tif");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),fimg);

    fall = gtk_file_filter_new();
    gtk_file_filter_set_name(fall,"All files");
    gtk_file_filter_add_pattern(fall,"*");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),fall);


    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

        img->ocvMatImage.release();

        img->ocvMatImage = imread(filename);

        IplImage *tmp = new IplImage(img->ocvMatImage);
        cvCvtColor(tmp, tmp, CV_BGR2RGB);

        if(!image_size_test())
        {
            gtk_widget_destroy (dialog);
            return;
        }

        imgdata_grid_default(img); //nejdrive spocitat mrizku


        img->ocvMatGrid = get_grid_mat_from_imgdata(img); //Pak mrizku dat do matice


        const gchar *filetxt = g_strdup_printf("%s [%dx%d]", g_path_get_basename(filename), img->ocvMatImage.cols, img->ocvMatImage.rows);

        if (img->is_source)
        {
            gtk_label_set_text (GTK_LABEL(label_source), filetxt);

            opencvMatImage =  img->ocvMatImage.clone(); // new Mat(img->ocvMatImage->size(), img->ocvMatImage->type());
        }
        else
            gtk_label_set_text (GTK_LABEL(label_dest), filetxt);


        gtk_label_set_text (GTK_LABEL(label_grid), g_strdup_printf("%dx%d; %d points", grid.xline, grid.yline, grid.xline*grid.yline));


        if(src_imgdata.ocvMatImage.data != NULL && dst_imgdata.ocvMatImage.data != NULL)
        {
            show_morph_image();
        }

        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}


void
setup_grid (gpointer dummy, TImgData *imgdata)
{
    if( imgdata->ocvMatImage.data == NULL)
    {
        open_file(dummy, imgdata);
        if( imgdata->ocvMatImage.data != NULL)
            show_girdwnd(dummy, imgdata);
    }
    else
        show_girdwnd(dummy, imgdata);
}

void zoom_image ()
{
    if (zoom != 100)
    {

        double fzoom = zoom / 100.0;

        resize(opencvMatImage, opencvMatImageZOOM, Size(), fzoom, fzoom, INTER_LINEAR);

        ocvMat2gtkImg(opencvMatImageZOOM, &image);
    }
}

void zoomout()
{
    if(opencvMatImage.data != NULL)
    {
        if((zoom-10) > 0)
            zoom -= 10;
        else
            zoom = 1;

        zoom_image();
    }
}


void zoomin()
{
    if(opencvMatImage.data != NULL)
    {
        zoom += 10;
        zoom_image();
    }
}

void zoomnormal()
{

    if(opencvMatImage.data != NULL)
    {
        zoom = 100;

        double fzoom = zoom / 100.0;
        resize(opencvMatImage, opencvMatImageZOOM, Size(), fzoom, fzoom, INTER_LINEAR);
        ocvMat2gtkImg(opencvMatImageZOOM, &image);
    }

    bestfit = FALSE;
}

void zoomfit ()
{
    bestfit = TRUE;

    if(opencvMatImage.data != NULL)
    {

        double w = scrolled_window->allocation.width;
        double h = scrolled_window->allocation.height;

        if(w <= 0 || h <= 0)
            return;

        double width, height;

        if(h < w)
        {
            height = h;
            width = (((float)opencvMatImage.cols/(float)opencvMatImage.rows) * h);
        }
        else
        {
            width = w;
            height =  (((float)opencvMatImage.rows/(float)opencvMatImage.cols) * w);
        }

        resize(opencvMatImage, opencvMatImageZOOM, Size(), width/opencvMatImage.cols, height/opencvMatImage.rows, INTER_LINEAR);

        ocvMat2gtkImg(opencvMatImageZOOM, &image);
    }
}


gboolean
show_message (GtkMessageType type, const gchar *format, gchar *msgtxt, const gchar *format_sec, gchar *msgtxt_sec)
{
    GtkWidget *dialog;
    gboolean ret = FALSE;

    dialog = gtk_message_dialog_new ( NULL,
                                      GTK_DIALOG_DESTROY_WITH_PARENT,
                                      type,
                                      GTK_BUTTONS_OK_CANCEL,
                                      format,
                                      msgtxt);

    gtk_window_set_title (GTK_WINDOW (dialog), "ZPO - morphing, grid warping");

    gtk_message_dialog_format_secondary_text ( GTK_MESSAGE_DIALOG (dialog),
            format_sec,
            msgtxt_sec);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK)
        ret = TRUE;

    gtk_widget_destroy (dialog);

    return ret;
}


void show_about (gpointer, GtkWidget *win)
{
    GtkWidget *dialog;
    GtkAboutDialog *about;

    const gchar    *auth[] = {  "Bc. Jan Kaláb (xkalab00)",
                                "Bc. Jan Lipovský (xlipov00)",
                                "Bc. František Skála (xskala05)",
                                NULL
                             };

    dialog = gtk_about_dialog_new();
    gtk_window_set_transient_for( GTK_WINDOW( dialog ),
                                  GTK_WINDOW( win ) );
    about = GTK_ABOUT_DIALOG( dialog );

#if GTK_CHECK_VERSION(2,12,0)
    gtk_about_dialog_set_program_name( about, "ZPO - morphing, grid warping" );
#endif

    gtk_about_dialog_set_version( about, "0.1.0" );
    gtk_about_dialog_set_copyright( about, "Copyright 2011 © Jan Kaláb, Jan Lipovský, František Skála" );
    gtk_about_dialog_set_website( about, "https://github.com/Pitel/morphing" );
    gtk_about_dialog_set_logo(about, gdk_pixbuf_new_from_file("cibo00_Algebraic_Mosaic.svg", NULL));	//http://www.openclipart.org/detail/12322
    gtk_about_dialog_set_authors( about, auth );

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG( dialog ));
    gtk_widget_destroy (dialog);
}


int main (int argc, char *argv[])
{
//    GtkWidget *button = NULL;
    GtkWidget *vbox = NULL;
//    GtkWidget *vbox2 = NULL;
    GtkWidget *hbox = NULL;
//    GtkWidget *label = NULL;
    GtkWidget *menu1 = NULL;
    GtkWidget *menu = NULL;
    GtkWidget *submenu = NULL;
    GtkWidget *menuitem = NULL;


    GtkWidget *toolbar = NULL;
    GtkToolItem *save = NULL;
    GtkToolItem *zin = NULL;
    GtkToolItem *zout = NULL;
    GtkToolItem *zfit = NULL;
    GtkToolItem *zall = NULL;
    GtkToolItem *sep = NULL;

    src_imgdata.is_source = TRUE;
    dst_imgdata.is_source = FALSE;

    src_imgdata.grid_size = &grid;
    dst_imgdata.grid_size = &grid;

    /* Threads init */
    g_thread_init (NULL);
    gdk_threads_init ();

    /* Initialize GTK+ */
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 0);
    gtk_window_set_title (GTK_WINDOW (win), "ZPO - Morphing, gird warping");
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(win), 500, 500);
    gtk_window_set_icon(GTK_WINDOW(win), gdk_pixbuf_new_from_file("cibo00_Algebraic_Mosaic.svg", NULL));	//http://www.openclipart.org/detail/12322
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);

    /* Create a vertical box with buttons */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (win), vbox);

    /* Create menu bar*/
    menu1 = gtk_menu_bar_new();
    gtk_box_pack_start (GTK_BOX (vbox), menu1, FALSE, TRUE, 0);

    /* File menu */
    menu = gtk_menu_item_new_with_mnemonic ("_File");
    gtk_menu_shell_append (GTK_MENU_SHELL(menu1), menu);
    gtk_widget_show(menu);

    /*-- Create File submenu  --*/
    submenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu), submenu);

    menuitem = gtk_menu_item_new_with_mnemonic ("Load source file");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate", (GCallback) open_file, &src_imgdata);
    gtk_widget_show (menuitem);

    menuitem = gtk_menu_item_new_with_mnemonic ("Load destination file");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate", (GCallback) open_file, &dst_imgdata);
    gtk_widget_show (menuitem);

    menuitem = gtk_menu_item_new_with_mnemonic ("Save image as...");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate", (GCallback) save_file, NULL);
    gtk_widget_show (menuitem);

    menuitem = gtk_separator_menu_item_new ();
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    gtk_widget_show (menuitem);

    menuitem = gtk_menu_item_new_with_mnemonic ("_Quit");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate", gtk_main_quit, NULL);
    gtk_widget_show (menuitem);


    /* Tools menu */
    menu = gtk_menu_item_new_with_mnemonic ("_Tools");
    gtk_menu_shell_append (GTK_MENU_SHELL(menu1), menu);
    gtk_widget_show(menu);

    submenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu), submenu);

    menuitem = gtk_menu_item_new_with_mnemonic ("Set source gird ");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate",  (GCallback) setup_grid, (gpointer) &src_imgdata);
    gtk_widget_show (menuitem);

    menuitem = gtk_menu_item_new_with_mnemonic ("Set destination gird");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate",  (GCallback) setup_grid, (gpointer) &dst_imgdata);
    gtk_widget_show (menuitem);


    menuitem = gtk_menu_item_new_with_mnemonic ("Set morphing steps");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate",  (GCallback) show_set_steps, NULL);
    gtk_widget_show (menuitem);


    /* Help menu */
    menu = gtk_menu_item_new_with_mnemonic ("_Help");
    gtk_menu_shell_append (GTK_MENU_SHELL(menu1), menu);
    gtk_widget_show(menu);

    submenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu), submenu);

    menuitem = gtk_menu_item_new_with_mnemonic ("_About...");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate", (GCallback) show_about, win);
    gtk_widget_show (menuitem);



    /* Toolbbar menu */
    toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);

    save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE_AS);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);

    sep = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1);

    zin = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_IN);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), zin, -1);

    zout = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_OUT);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), zout, -1);

    zfit = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_FIT);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), zfit, -1);

    zall = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_100);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), zall, -1);

//    sep = gtk_separator_tool_item_new();
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1);
//
//    refresh = gtk_tool_button_new_from_stock(GTK_STOCK_REFRESH);
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), refresh, -1);

    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);


    //  g_signal_connect (G_OBJECT (open), "clicked", G_CALLBACK (open_file), NULL);
    g_signal_connect (G_OBJECT (save), "clicked", G_CALLBACK (save_file), NULL);
    g_signal_connect (G_OBJECT (zin), "clicked", G_CALLBACK (zoomin), NULL);
    g_signal_connect (G_OBJECT (zout), "clicked", G_CALLBACK (zoomout), NULL);
    g_signal_connect (G_OBJECT (zall), "clicked", G_CALLBACK (zoomnormal), NULL);
    g_signal_connect (G_OBJECT (zfit), "clicked", G_CALLBACK (zoomfit), NULL);


    hbox = gtk_hbox_new (FALSE, 6);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 6);

//
//    vbox2 = gtk_vbox_new (FALSE, 6);
//    gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 6);


    //  GtkWidget *valign;

//    valign = gtk_alignment_new(0, 1, 1, 0);
//    label = gtk_label_new ("Tools:");
//    gtk_container_add(GTK_CONTAINER(valign), label);
//    gtk_box_pack_start(GTK_BOX(vbox2), valign, FALSE, FALSE, 0);

// create a new scrolled window.
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 0);

    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_box_pack_start (GTK_BOX (hbox), scrolled_window, TRUE, TRUE, 6);


    //image_left = gtk_image_new_from_file("Lenna.png");
    image = gtk_image_new ();


    //gtk_box_pack_start (GTK_BOX (vbox), image, TRUE, TRUE, 0);


    //GtkWidget *event_box;
//    event_box = gtk_event_box_new ();
//    gtk_container_add (GTK_CONTAINER (event_box), image);
//
//    g_signal_connect (G_OBJECT (event_box),
//                      "button_press_event",
//                      G_CALLBACK (img_zoom_click), NULL);
//

    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), image);

    play_data.wnd = scrolled_window;


    hscale = gtk_hscale_new_with_range(RANGE_MIN, RANGE_MAX, RANGE_STEP);

    g_signal_connect (G_OBJECT (hscale), "value-changed", G_CALLBACK (change_corrent_step), NULL);


    hbox = gtk_hbox_new (FALSE, 6);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 6);


    GtkWidget *button = gtk_button_new ();
    GtkWidget *img = gtk_image_new_from_stock(GTK_STOCK_MEDIA_PLAY, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image (GTK_BUTTON(button), img);

    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (play_click), NULL);


    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 6);



    button = gtk_button_new ();
    img = gtk_image_new_from_stock(GTK_STOCK_MEDIA_STOP, GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image (GTK_BUTTON(button), img);

    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (stop_click), NULL);

    gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 0);


    //RANGE
    gtk_box_pack_start(GTK_BOX(hbox), hscale, TRUE, TRUE, 6);



    // gtk_misc_set_alignment (GTK_MISC(image_left), 0.0,0.0);

    //gtk_box_pack_start (GTK_BOX (hbox), image_left, FALSE, FALSE, 6);
    // gtk_box_pack_start (GTK_BOX (hbox), image_center, FALSE, FALSE, 6);

    //gtk_window_set_default_size (GTK_WINDOW (image_center->window), 200,200);



//    play_data.hscale = hscale;
    play_data.imageGTK = image;
    play_data.images = NULL;
    play_data.playing = FALSE;

    GtkWidget * hsep = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(vbox), hsep, FALSE, FALSE, 2);

    GtkWidget * table = gtk_table_new (5, 2, FALSE);
    gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, FALSE, 2);

    GtkWidget * label = gtk_label_new (NULL);
    gtk_label_set_markup (GTK_LABEL (label), "<span weight=\"bold\"><big>Global informations</big></span>");
    gtk_table_attach(GTK_TABLE(table), label, 0, 2, 0, 1,  (GtkAttachOptions)(GTK_FILL  | GTK_EXPAND), (GtkAttachOptions)(GTK_FILL | GTK_EXPAND), 6, 2);



    GtkWidget * halign = gtk_alignment_new(0, 0, 0, 1);
    label = gtk_label_new ("Source file:");
    gtk_container_add(GTK_CONTAINER(halign), label);
    gtk_table_attach(GTK_TABLE(table), halign, 0, 1, 1, 2,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions)( GTK_FILL | GTK_EXPAND), 6, 2);


    halign = gtk_alignment_new(0, 0, 0, 1);
    label_source = gtk_label_new ("-");
    gtk_container_add(GTK_CONTAINER(halign), label_source);
    gtk_table_attach(GTK_TABLE(table), halign, 1, 2, 1, 2,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);


    halign = gtk_alignment_new(0, 0, 0, 1);
    label = gtk_label_new ("Destination file:");
    gtk_container_add(GTK_CONTAINER(halign), label);
    gtk_table_attach(GTK_TABLE(table), halign, 0, 1, 2, 3, (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);


    halign = gtk_alignment_new(0, 0, 0, 1);
    label_dest = gtk_label_new ("-");
    gtk_container_add(GTK_CONTAINER(halign), label_dest);
    gtk_table_attach(GTK_TABLE(table), halign, 1, 2, 2, 3,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);
    halign = gtk_alignment_new(0, 0, 0, 1);


    halign = gtk_alignment_new(0, 0, 0, 1);
    label = gtk_label_new ("Grid:");
    gtk_container_add(GTK_CONTAINER(halign), label);
    gtk_table_attach(GTK_TABLE(table), halign, 0, 1, 3, 4,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);


    halign = gtk_alignment_new(0, 0, 0, 1);
    label_grid = gtk_label_new ("-");
    gtk_container_add(GTK_CONTAINER(halign), label_grid);
    gtk_table_attach(GTK_TABLE(table), halign, 1, 2, 3, 4,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);


    halign = gtk_alignment_new(0, 0, 0, 1);
    label = gtk_label_new ("Morphing steps:");
    gtk_container_add(GTK_CONTAINER(halign), label);
    gtk_table_attach(GTK_TABLE(table), halign, 0, 1, 4, 5,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);


    halign = gtk_alignment_new(0, 0, 0, 1);
    label_steps = gtk_label_new ("10");
    gtk_container_add(GTK_CONTAINER(halign), label_steps);
    gtk_table_attach(GTK_TABLE(table), halign, 1, 2, 4, 5,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);




    /* Enter the main loop */
    gtk_widget_show_all (win);

    gdk_threads_enter ();
    gtk_main ();
    gdk_threads_leave ();

    return 0;
}

