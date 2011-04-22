#include <stdlib.h>
#include <gtk/gtk.h>


GtkWidget *win;


//IplImage *opencvImage = NULL;
GtkWidget *image_left = NULL;
GtkWidget *image_center = NULL;
GtkWidget *image_right = NULL;


#define RANGE_MIN   0
#define RANGE_MAX   50
#define RANGE_STEP  1


static void
save_file  ()
{
 /*   GtkWidget *dialog;
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
    gtk_widget_destroy (dialog);*/


    gfloat x,y;
    gint xp,yp;

    gtk_misc_get_alignment (GTK_MISC(image_left), &x,&y);
    gtk_misc_get_padding(GTK_MISC(image_left), &xp,&yp);

    return;

}


static void
open_file ()
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new ("Open image file...",
                                          GTK_WINDOW (win),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));



//        if (opencvImage != NULL)
//            cvReleaseImage(&opencvImage);
//
//        opencvImage = cvLoadImage(filename, CV_LOAD_IMAGE_UNCHANGED);
//
//        cvCvtColor(opencvImage, opencvImage, CV_BGR2RGB);
//
//        ocvCopyImg(&opencvImage, &opencvImagevbox);
//        ocvCopyImg(&opencvImage, &opencvImageGTK);
//
//        ocvImg2gtkImg(&opencvImageGTK, &image);
//
//        gtkvbox_loaded = FALSE;
        g_free (filename);
    }
    gtk_widget_destroy (dialog);
}





int main (int argc, char *argv[])
{
    GtkWidget *button = NULL;
    GtkWidget *vbox = NULL;
    GtkWidget *vbox2 = NULL;
    GtkWidget *hbox = NULL;
    GtkWidget *label = NULL;
    GtkWidget *menu1 = NULL;
    GtkWidget *menu = NULL;
    GtkWidget *submenu = NULL;
    GtkWidget *menuitem = NULL;
    GtkWidget *scrolled_window = NULL;


    GtkWidget *toolbar = NULL;
    GtkToolItem *open = NULL;
    GtkToolItem *save = NULL;
    GtkToolItem *zin = NULL;
    GtkToolItem *zout = NULL;
    GtkToolItem *zfit = NULL;
    GtkToolItem *zall = NULL;
    GtkToolItem *sep = NULL;
    GtkToolItem *refresh = NULL;

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
    g_signal_connect (GTK_OBJECT (menuitem), "activate", open_file, NULL);
    gtk_widget_show (menuitem);

    menuitem = gtk_menu_item_new_with_mnemonic ("Load destination file");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    g_signal_connect (GTK_OBJECT (menuitem), "activate", save_file, NULL);
    gtk_widget_show (menuitem);

    menuitem = gtk_menu_item_new_with_mnemonic ("Save image as...");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
//    g_signal_connect (GTK_OBJECT (menuitem), "activate", set_default, NULL);
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
//    g_signal_connect (GTK_OBJECT (menuitem), "activate", run_options_dialog, NULL);
    gtk_widget_show (menuitem);

    menuitem = gtk_menu_item_new_with_mnemonic ("Set destination gird");
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menuitem);
    //g_signal_connect (GTK_OBJECT (menuitem), "activate", run_command_dialog, NULL);
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

    sep = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1);

    refresh = gtk_tool_button_new_from_stock(GTK_STOCK_REFRESH);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), refresh, -1);

    gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);


  //  g_signal_connect (G_OBJECT (open), "clicked", G_CALLBACK (open_file), NULL);
  //  g_signal_connect (G_OBJECT (save), "clicked", G_CALLBACK (save_file), NULL);


    hbox = gtk_hbox_new (FALSE, 6);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 6);

//
//    vbox2 = gtk_vbox_new (FALSE, 6);
//    gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, 6);


    GtkWidget *valign;

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


    image_left = gtk_image_new_from_file("Lenna.png");
  //  image_center = gtk_image_new       ();


    //gtk_box_pack_start (GTK_BOX (vbox), image, TRUE, TRUE, 0);


    //GtkWidget *event_box;
//    event_box = gtk_event_box_new ();
//    gtk_container_add (GTK_CONTAINER (event_box), image);
//
//    g_signal_connect (G_OBJECT (event_box),
//                      "button_press_event",
//                      G_CALLBACK (img_zoom_click), NULL);
//

    gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), image_left);


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

    /* Enter the main loop */
    gtk_widget_show_all (win);
    gtk_main ();
    return 0;
}

