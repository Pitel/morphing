#include <cstddef>
#include <cstdlib>
#include <cairo.h>
#include <gtk/gtk.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "gui_setgridwnd.hpp"
#include "globals.hpp"


static void
close_wnd (GtkWidget *win)
{
//    gtk_widget_hide_all (win);

    gtk_widget_destroy(win);
}

void draw_grid (cairo_t *cr, TImgData *imgdata)
{

}

void
show_girdwnd  (gpointer, TImgData *imgdata)
{
    GtkWidget *win;
    GtkWidget *scrolled_window = NULL;
    GtkWidget *image = NULL;
    cairo_t *cr;


    const gchar * title_source = "Setting gird on SOURCE image";
    const gchar * title_dest = "Setting gird on DESTINATION image";


    GtkWidget *vbox = NULL;
    GtkWidget *hbox = NULL;


    GtkWidget *toolbar = NULL;
    GtkToolItem *zin = NULL;
    GtkToolItem *zout = NULL;
    GtkToolItem *zfit = NULL;
    GtkToolItem *zall = NULL;
    GtkToolItem *sep = NULL;


    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 0);

    if(imgdata->is_source)
        gtk_window_set_title (GTK_WINDOW (win), title_source);
    else
        gtk_window_set_title (GTK_WINDOW (win), title_dest);

    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(win), 500, 500);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", (GCallback) close_wnd, win);

    /* Create a vertical box with buttons */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (win), vbox);


    /* Toolbbar menu */
    toolbar = gtk_toolbar_new();
    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);

    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);

//    save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE_AS);
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);

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
//    g_signal_connect (G_OBJECT (zin), "clicked", G_CALLBACK (zoomin), NULL);
//    g_signal_connect (G_OBJECT (zout), "clicked", G_CALLBACK (zoomout), NULL);
//    g_signal_connect (G_OBJECT (zall), "clicked", G_CALLBACK (zoomnormal), NULL);
//    g_signal_connect (G_OBJECT (zfit), "clicked", G_CALLBACK (zoomfit), NULL);


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


    gtk_misc_set_alignment (GTK_MISC(image), 0.0,0.0);
    ocvImg2gtkImg(&(imgdata->ocvImage), &image);

    cr = gdk_cairo_create (image->window);

    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), image);



    hbox = gtk_hbox_new (FALSE, 6);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 6);

    //gtk_range_set_value  (GTK_RANGE(hscale), RANGE_MAX/2);
    //gtk_range_set_inverted (GTK_RANGE(vscale), TRUE);

   // g_signal_connect (G_OBJECT (vscale), "value-changed", G_CALLBACK (change_values), &(player.eq[i]));

   // gtk_box_pack_start(GTK_BOX(hbox), hscale, TRUE, TRUE, 6);

   // gtk_misc_set_alignment (GTK_MISC(image_left), 0.0,0.0);

    //gtk_box_pack_start (GTK_BOX (hbox), image_left, FALSE, FALSE, 6);
   // gtk_box_pack_start (GTK_BOX (hbox), image_center, FALSE, FALSE, 6);

    //gtk_window_set_default_size (GTK_WINDOW (image_center->window), 200,200);

    /* Enter the main loop */




    gtk_widget_show_all (win);
}

