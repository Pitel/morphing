#include <cstddef>
#include <cstdlib>
#include <cairo.h>
#include <gtk/gtk.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "gui_setgridwnd.hpp"
#include "globals.hpp"

#define TOLERANCE 15

typedef struct sd
{
    TImgData *idata;
    GtkWidget *image;
    gint drag_point;
    GtkWidget *event_box;
} TSignalData;

TSignalData dst_sigdata, src_sigdata;


void
add_line_vertical (GtkWidget *, TSignalData *sigdata)
{
        if((sigdata->idata->grid_size->xline * (sigdata->idata->grid_size->yline + 1)) < GRID_MAX)
            sigdata->idata->grid_size->yline++;

        gboolean ret;
        if(src_sigdata.idata != NULL)
        {
            imgdata_grid_default(src_sigdata.idata);

            //if(src_sigdata.idata->ocvMatGrid)
            //    src_sigdata.idata->ocvMatGrid.release();

             src_sigdata.idata->ocvMatGrid = get_grid_mat_from_imgdata(src_sigdata.idata);

            if(src_sigdata.event_box != NULL)
                g_signal_emit_by_name(G_OBJECT(src_sigdata.event_box), "expose-event", sigdata, &ret);
        }

        if(dst_sigdata.idata != NULL)
        {
            imgdata_grid_default(dst_sigdata.idata);

           // if(dst_sigdata.idata->ocvMatGrid)
           //     dst_sigdata.idata->ocvMatGrid->release();

             dst_sigdata.idata->ocvMatGrid = get_grid_mat_from_imgdata(dst_sigdata.idata);

            if(dst_sigdata.event_box != NULL)
                g_signal_emit_by_name(G_OBJECT(dst_sigdata.event_box), "expose-event", sigdata, &ret);
        }
}

void
add_line_horizontal (GtkWidget *, TSignalData *sigdata)
{
        if((sigdata->idata->grid_size->yline * (sigdata->idata->grid_size->xline + 1)) < GRID_MAX)
            sigdata->idata->grid_size->xline++;

        gboolean ret;
        if(src_sigdata.idata != NULL)
        {
            imgdata_grid_default(src_sigdata.idata);

            //if(src_sigdata.idata->ocvMatGrid)
            //    src_sigdata.idata->ocvMatGrid.release();

             src_sigdata.idata->ocvMatGrid = get_grid_mat_from_imgdata(src_sigdata.idata);

            if(src_sigdata.event_box != NULL)
                g_signal_emit_by_name(G_OBJECT(src_sigdata.event_box), "expose-event", sigdata, &ret);
        }

        if(dst_sigdata.idata != NULL)
        {
            imgdata_grid_default(dst_sigdata.idata);

           // if(dst_sigdata.idata->ocvMatGrid)
           //     dst_sigdata.idata->ocvMatGrid->release();

             dst_sigdata.idata->ocvMatGrid = get_grid_mat_from_imgdata(dst_sigdata.idata);

            if(dst_sigdata.event_box != NULL)
                g_signal_emit_by_name(G_OBJECT(dst_sigdata.event_box), "expose-event", sigdata, &ret);
        }
}


void
remove_line_vertical (GtkWidget *, TSignalData *sigdata)
{
        if((sigdata->idata->grid_size->yline - 1) > 2)
            sigdata->idata->grid_size->yline--;

        gboolean ret;
        if(src_sigdata.idata != NULL)
        {
            imgdata_grid_default(src_sigdata.idata);

            //if(src_sigdata.idata->ocvMatGrid)
            //    src_sigdata.idata->ocvMatGrid.release();

             src_sigdata.idata->ocvMatGrid = get_grid_mat_from_imgdata(src_sigdata.idata);

            if(src_sigdata.event_box != NULL)
                g_signal_emit_by_name(G_OBJECT(src_sigdata.event_box), "expose-event", sigdata, &ret);
        }

        if(dst_sigdata.idata != NULL)
        {
            imgdata_grid_default(dst_sigdata.idata);

           // if(dst_sigdata.idata->ocvMatGrid)
           //     dst_sigdata.idata->ocvMatGrid->release();

             dst_sigdata.idata->ocvMatGrid = get_grid_mat_from_imgdata(dst_sigdata.idata);

            if(dst_sigdata.event_box != NULL)
                g_signal_emit_by_name(G_OBJECT(dst_sigdata.event_box), "expose-event", sigdata, &ret);
        }
}

void
remove_line_horizontal (GtkWidget *, TSignalData *sigdata)
{
        if((sigdata->idata->grid_size->xline - 1) > 2)
            sigdata->idata->grid_size->xline--;

        gboolean ret;
        if(src_sigdata.idata != NULL)
        {
            imgdata_grid_default(src_sigdata.idata);

            //if(src_sigdata.idata->ocvMatGrid)
            //    src_sigdata.idata->ocvMatGrid.release();

             src_sigdata.idata->ocvMatGrid = get_grid_mat_from_imgdata(src_sigdata.idata);

            if(src_sigdata.event_box != NULL)
                g_signal_emit_by_name(G_OBJECT(src_sigdata.event_box), "expose-event", sigdata, &ret);
        }

        if(dst_sigdata.idata != NULL)
        {
            imgdata_grid_default(dst_sigdata.idata);

           // if(dst_sigdata.idata->ocvMatGrid)
           //     dst_sigdata.idata->ocvMatGrid->release();

             dst_sigdata.idata->ocvMatGrid = get_grid_mat_from_imgdata(dst_sigdata.idata);

            if(dst_sigdata.event_box != NULL)
                g_signal_emit_by_name(G_OBJECT(dst_sigdata.event_box), "expose-event", sigdata, &ret);
        }
}

static void
close_wnd (GtkWidget *win)
{
    dst_sigdata.event_box = NULL;
    src_sigdata.event_box = NULL;

    gtk_widget_destroy(win);
}

void draw_grid (cairo_t *cr, TImgData *imgdata)
{
    cairo_set_source_rgb (cr, 0, 0, 0);

    for(int y = 0; y < imgdata->grid_size->yline; y++)
        for(int x = 0; x < imgdata->grid_size->xline; x++)
        {
            int xx = x+1 < imgdata->grid_size->xline ? x+1 : x;
            int yy = y+1 < imgdata->grid_size->yline ? y+1 : y;


            if (imgdata->grid[xx+y* imgdata->grid_size->xline].y != imgdata->ocvMatImage.rows &&
                    imgdata->grid[xx+y* imgdata->grid_size->xline].y != 0 &&
                    imgdata->grid[x+y* imgdata->grid_size->xline].y != imgdata->ocvMatImage.rows &&
                    imgdata->grid[x+y* imgdata->grid_size->xline].y > 0)
            {
                cairo_move_to (cr, imgdata->grid[x+y* imgdata->grid_size->xline].x, imgdata->grid[x+y* imgdata->grid_size->xline].y);
                cairo_line_to (cr, imgdata->grid[xx+y* imgdata->grid_size->xline].x, imgdata->grid[xx+y* imgdata->grid_size->xline].y);
            }


            if (imgdata->grid[x+yy* imgdata->grid_size->xline].x != imgdata->ocvMatImage.cols &&
                    imgdata->grid[x+yy* imgdata->grid_size->xline].x != 0 &&
                    imgdata->grid[x+y* imgdata->grid_size->xline].x != imgdata->ocvMatImage.cols &&
                    imgdata->grid[x+y* imgdata->grid_size->xline].x > 0)
            {
                cairo_move_to (cr, imgdata->grid[x+y* imgdata->grid_size->xline].x, imgdata->grid[x+y* imgdata->grid_size->xline].y);
                cairo_line_to (cr, imgdata->grid[x+yy* imgdata->grid_size->xline].x, imgdata->grid[x+yy* imgdata->grid_size->xline].y);
            }

        }

    cairo_stroke(cr);
}

static gboolean
button_release_callback (GtkWidget       *event_box,
                         GdkEventButton  *,
                         TSignalData     *sigdata)
{

    sigdata->idata->ocvMatGrid = get_grid_mat_from_imgdata(sigdata->idata);

    gdk_window_invalidate_rect (event_box->window,NULL, FALSE);
    return TRUE;
}

static gboolean
button_press_callback (GtkWidget       *,
                       GdkEventButton  *event,
                       TSignalData     *sigdata)
{
  //  printf("%f, %f\n",event->x, event->y);

    sigdata->drag_point = -1;

    for(int y = 0; y < sigdata->idata->grid_size->yline; y++)
        for(int x = 0; x < sigdata->idata->grid_size->xline; x++)
        {
            int xgrid = sigdata->idata->grid[x+y*sigdata->idata->grid_size->xline].x;
            int ygrid = sigdata->idata->grid[x+y*sigdata->idata->grid_size->xline].y;

            //preskoci krajni body
            if(xgrid <= 0 || xgrid >= sigdata->idata->ocvMatImage.cols)
                continue;
            if(ygrid <= 0 || ygrid >= sigdata->idata->ocvMatImage.rows)
                continue;

            if ((event->x > xgrid - TOLERANCE) && (event->x < xgrid + TOLERANCE))
            {
                if ((event->y > ygrid - TOLERANCE) && (event->y < ygrid + TOLERANCE))
                    sigdata->drag_point = x+y*sigdata->idata->grid_size->xline;
            }
        }

    return FALSE;
}

static gboolean
button_move_callback (GtkWidget      *event_box,
                      GdkEventButton *event,
                      TSignalData    *sigdata)
{
    if(sigdata->drag_point > 0)
    {
       // if(event->x < 0 || event->x > sigdata->idata->)

        sigdata->idata->grid[sigdata->drag_point].x = event->x;
        sigdata->idata->grid[sigdata->drag_point].y = event->y;


        gdk_window_invalidate_rect (event_box->window,NULL, FALSE);
    }

    return TRUE;
}


static gboolean
expose_callback (GtkWidget      *event_box,
                 GdkEventButton *,
                 TSignalData    *sigdata)
{
    cairo_t *cr;
    if (sigdata->event_box != NULL)
        cr = gdk_cairo_create (GDK_DRAWABLE (sigdata->event_box->window));
    else
    {
        cr = gdk_cairo_create (GDK_DRAWABLE (event_box->window));
        sigdata->event_box = event_box;
    }


    if (sigdata->image != NULL)
    {
        GdkPixbuf *pxbf = gtk_image_get_pixbuf(GTK_IMAGE(sigdata->image));

        gdk_cairo_set_source_pixbuf (cr, pxbf, 0,0);

        cairo_paint (cr);

        draw_grid(cr, sigdata->idata);
    }


    return TRUE;
}




void
show_girdwnd  (gpointer, TImgData *imgdata)
{
    GtkWidget *win;
    GtkWidget *button;
    GtkWidget *scrolled_window = NULL;
    GtkWidget *image = NULL;

    const gchar *title_source = "Setting gird on SOURCE image";
    const gchar *title_dest = "Setting gird on DESTINATION image";

    dst_sigdata.event_box = NULL;
    src_sigdata.event_box = NULL;

    GtkWidget *vbox = NULL;
    GtkWidget *vbox2 = NULL;
    GtkWidget *label = NULL;
    GtkWidget *hbox = NULL;


//    GtkWidget *toolbar = NULL;
//    GtkToolItem *zin = NULL;
//    GtkToolItem *zout = NULL;
//    GtkToolItem *zfit = NULL;
//    GtkToolItem *zall = NULL;
//    GtkToolItem *sep = NULL;


    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width (GTK_CONTAINER (win), 0);

    if(imgdata->is_source)
        gtk_window_set_title (GTK_WINDOW (win), title_source);
    else
        gtk_window_set_title (GTK_WINDOW (win), title_dest);

    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(win), 600, 500);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", (GCallback) close_wnd, win);

    /* Create a vertical box with buttons */
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (win), vbox);


//    /* Toolbbar menu */
//    toolbar = gtk_toolbar_new();
//    gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
//
//    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);
//
////    save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE_AS);
////    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);
//
//    sep = gtk_separator_tool_item_new();
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1);
//
//    zin = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_IN);
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), zin, -1);
//
//    zout = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_OUT);
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), zout, -1);
//
//    zfit = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_FIT);
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), zfit, -1);
//
//    zall = gtk_tool_button_new_from_stock(GTK_STOCK_ZOOM_100);
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), zall, -1);

//    sep = gtk_separator_tool_item_new();
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1);
//
//    refresh = gtk_tool_button_new_from_stock(GTK_STOCK_REFRESH);
//    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), refresh, -1);

   // gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);


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



    //image_left = gtk_image_new_from_file("Lenna.png");


    //gtk_box_pack_start (GTK_BOX (vbox), image, TRUE, TRUE, 0);



    image = gtk_image_new ();
    gtk_misc_set_alignment (GTK_MISC(image), 0.0,0.0);

    GtkWidget *event_box;
    event_box = gtk_event_box_new ();
    gtk_container_add (GTK_CONTAINER (event_box), image);

    TSignalData *d;

    if(imgdata->is_source)
        d = &src_sigdata;
    else
        d = &dst_sigdata;

    d->idata = imgdata;
    d->image = image;

    g_signal_connect (G_OBJECT (event_box),
                      "motion_notify_event",//g_signal_name (GDK_MOTION_NOTIFY),
                      G_CALLBACK (button_move_callback), (gpointer)d);


    g_signal_connect (G_OBJECT (event_box),
                      "button-press-event",
                      G_CALLBACK (button_press_callback), (gpointer)d);

    g_signal_connect (G_OBJECT (event_box),
                      "button-release-event",
                      G_CALLBACK (button_release_callback), (gpointer)d);


    g_signal_connect (G_OBJECT (event_box),
                      "expose-event",
                      G_CALLBACK (expose_callback), (gpointer)d);

    ocvMat2gtkImg(imgdata->ocvMatImage, &image);
    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), event_box);


    //hbox = gtk_hbox_new (FALSE, 6);
    //gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 6);

    vbox2 = gtk_vbox_new (FALSE, 6);
    gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 6);
    gtk_box_pack_start (GTK_BOX (hbox), scrolled_window, TRUE, TRUE, 6);



    GtkWidget *valign;

    valign = gtk_alignment_new(0, 1, 0, 1);
    label = gtk_label_new ("Horizontal line:");
    gtk_container_add(GTK_CONTAINER(valign), label);
    gtk_box_pack_start(GTK_BOX(vbox2), valign, FALSE, FALSE, 0);


    valign = gtk_alignment_new(0, 1, 1, 1);
    button = gtk_button_new_with_label ("Add");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (add_line_horizontal), d);
    gtk_container_add(GTK_CONTAINER(valign), button);
    gtk_box_pack_start(GTK_BOX(vbox2), valign, FALSE, FALSE, 0);

    valign = gtk_alignment_new(0, 1, 1, 1);
    button = gtk_button_new_with_label ("Remove");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (remove_line_horizontal), d);
    gtk_container_add(GTK_CONTAINER(valign), button);
    gtk_box_pack_start(GTK_BOX(vbox2), valign, FALSE, FALSE, 0);


    valign = gtk_alignment_new(0, 1, 0, 1);
    label = gtk_label_new ("Vertical line:");
    gtk_container_add(GTK_CONTAINER(valign), label);
    gtk_box_pack_start(GTK_BOX(vbox2), valign, FALSE, FALSE, 0);

    valign = gtk_alignment_new(0, 1, 1, 1);
    button = gtk_button_new_with_label ("Add");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (add_line_vertical), d);
    gtk_container_add(GTK_CONTAINER(valign), button);
    gtk_box_pack_start(GTK_BOX(vbox2), valign, FALSE, FALSE, 0);

    valign = gtk_alignment_new(0, 1, 1, 1);
    button = gtk_button_new_with_label ("Remove");
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (remove_line_vertical), d);
    gtk_container_add(GTK_CONTAINER(valign), button);
    gtk_box_pack_start(GTK_BOX(vbox2), valign, FALSE, FALSE, 0);

    gtk_widget_show_all (win);
}

