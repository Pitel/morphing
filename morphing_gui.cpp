#include <cstddef>
#include <cstdlib>
#include <gtk/gtk.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "gui_setgridwnd.hpp"

#include "globals.hpp"

GtkWidget *win;
GtkWidget *scrolled_window = NULL;
GtkWidget *label_source = NULL;
GtkWidget *label_dest = NULL;
GtkWidget *label_grid = NULL;


IplImage *opencvImage = NULL;
IplImage *opencvImageGTK = NULL; //Tady je kopie obrazku, ktera se pak zobrazuje v GTK
GtkWidget *image = NULL;

TImgData src_imgdata, dst_imgdata;


IplImage *opencvImage_source = NULL;

int zoom = 100;

#define RANGE_MIN   0
#define RANGE_MAX   50
#define RANGE_STEP  1



void
save_file  ()
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Save image as...",
                                          GTK_WINDOW (win),
                                          GTK_FILE_CHOOSER_ACTION_SAVE,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                          NULL);

    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);


    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

//        cvSaveImage(filename, opencvImagevbox, 0);

        g_free (filename);
    }
    gtk_widget_destroy (dialog);

    return;

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


        if (img->ocvImage != NULL)
            cvReleaseImage(&(img->ocvImage));

        img->ocvImage = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);

        cvCvtColor(img->ocvImage, img->ocvImage, CV_BGR2RGB);

        //ocvCopyImg(&opencvImage, &opencvImagevbox);
        //ocvCopyImg(&opencvImage, &opencvImageGTK);

        //ocvImg2gtkImg(&opencvImageGTK, &image);

        //gtkvbox_loaded = FALSE;

        if (img->is_source)
            gtk_label_set_text (GTK_LABEL(label_source), g_path_get_basename(filename));
        else
            gtk_label_set_text (GTK_LABEL(label_dest), g_path_get_basename(filename));

        g_free (filename);

        //show_girdwnd(dummy, img);


    }
    gtk_widget_destroy (dialog);
}


void
setup_grid (gpointer dummy, TImgData *imgdata)
{
    if( imgdata->ocvImage == NULL )
    {
        open_file(dummy, imgdata);
        if( imgdata->ocvImage != NULL )
            show_girdwnd(dummy, imgdata);
    }
    else
        show_girdwnd(dummy, imgdata);
}

void zoom_image ()
{
    gtk_image_clear(GTK_IMAGE(image));

    if(opencvImageGTK != NULL)
        cvReleaseImage(&opencvImageGTK);

    opencvImageGTK = cvCreateImage(cvSize((int)((opencvImage->width*(zoom))/100), (int)((opencvImage->height*(zoom))/100) ), opencvImage->depth, opencvImage->nChannels );

    cvResize(opencvImage, opencvImageGTK, CV_INTER_LINEAR);

    ocvImg2gtkImg(&opencvImageGTK, &image);

    //gtktmp_loaded = FALSE;
}

void zoomout()
{
    if(opencvImageGTK != NULL)
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
    if(opencvImageGTK != NULL)
    {
        zoom += 10;
        zoom_image();
    }
}

void zoomnormal()
{

    if(opencvImageGTK != NULL)
    {
        zoom = 100;
        zoom_image();
    }
}

void zoomfit ()
{
    if(opencvImageGTK != NULL)
    {

        gint w = scrolled_window->allocation.width;
        gint h = scrolled_window->allocation.height;
        //gtk_widget_get_size_request  (GTK_WIDGET(win), &w, &h);

        if(w <= 0 || h <= 0)
            return;

        int width, height;

        //if(opencvImage->height > opencvImage->width)
        if(h < w)
        {
            height = h;
            width = (int)(((float)opencvImage->width/(float)opencvImage->height) * h);
        }
        else
        {
            width = w;
            height =  (int)(((float)opencvImage->height/(float)opencvImage->width) * w);
        }


        gtk_image_clear(GTK_IMAGE(image));

        if(opencvImageGTK != NULL)
            cvReleaseImage(&opencvImageGTK);

        opencvImageGTK = cvCreateImage(cvSize(width, height), opencvImage->depth, opencvImage->nChannels );

        cvResize(opencvImage, opencvImageGTK, CV_INTER_LINEAR);

        ocvImg2gtkImg(&opencvImageGTK, &image);

        //gtktmp_loaded = FALSE;
    }
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
    g_signal_connect (GTK_OBJECT (menuitem), "activate", save_file, NULL);
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


    /* Help menu */
    menu = gtk_menu_item_new_with_mnemonic ("_Help");
    gtk_menu_shell_append (GTK_MENU_SHELL(menu1), menu);
    gtk_widget_show(menu);

    submenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu), submenu);

    menuitem = gtk_menu_item_new_with_mnemonic ("_About...");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
   // g_signal_connect (GTK_OBJECT (menuitem), "activate", show_about, NULL);
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
  //  g_signal_connect (G_OBJECT (save), "clicked", G_CALLBACK (save_file), NULL);
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


    GtkWidget *hscale = NULL;
    hscale = gtk_hscale_new_with_range(RANGE_MIN, RANGE_MAX, RANGE_STEP);


    hbox = gtk_hbox_new (FALSE, 6);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 6);

    //gtk_range_set_value  (GTK_RANGE(hscale), RANGE_MAX/2);
    //gtk_range_set_inverted (GTK_RANGE(vscale), TRUE);

   // g_signal_connect (G_OBJECT (vscale), "value-changed", G_CALLBACK (change_values), &(player.eq[i]));

    gtk_box_pack_start(GTK_BOX(hbox), hscale, TRUE, TRUE, 6);

   // gtk_misc_set_alignment (GTK_MISC(image_left), 0.0,0.0);

    //gtk_box_pack_start (GTK_BOX (hbox), image_left, FALSE, FALSE, 6);
   // gtk_box_pack_start (GTK_BOX (hbox), image_center, FALSE, FALSE, 6);

    //gtk_window_set_default_size (GTK_WINDOW (image_center->window), 200,200);

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


    label = gtk_label_new ("Grid:");
    gtk_container_add(GTK_CONTAINER(halign), label);
    gtk_table_attach(GTK_TABLE(table), halign, 0, 1, 3, 4,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);


    halign = gtk_alignment_new(0, 0, 0, 1);
    label_grid = gtk_label_new ("-");
    gtk_container_add(GTK_CONTAINER(halign), label_grid);
    gtk_table_attach(GTK_TABLE(table), halign, 1, 2, 3, 4,  (GtkAttachOptions)(GTK_FILL ),(GtkAttachOptions) (GTK_FILL | GTK_EXPAND), 6, 2);



    /* Enter the main loop */
    gtk_widget_show_all (win);
    gtk_main ();
    return 0;
}

