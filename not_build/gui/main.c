#include <gtk/gtk.h>

typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;


gchar *img_name;


void callback_about (GtkMenuItem *menuitem, gpointer user_data);
void get_img (GtkFileChooserButton *wigdet, gpointer user_data);


int main(int argc, char *argv [])
{
    GtkWidget *fenetre_principale = NULL;
    SGlobalData data;
    GError *error = NULL;
    gchar *filename = NULL;
    
    gtk_init(&argc, &argv);

    data.builder = gtk_builder_new();

    filename =  g_build_filename ("gui.glade", NULL);

    gtk_builder_add_from_file (data.builder, filename, &error);
    g_free (filename);
    if (error)
    {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
    }

    gtk_builder_connect_signals (data.builder, &data);

    fenetre_principale = GTK_WIDGET(gtk_builder_get_object (data.builder, "window1"));

    gtk_widget_show_all (fenetre_principale);
    /*
    GtkImage *image = NULL;
    image = (GtkImage*) (gtk_builder_get_object(data.builder, "image1"));
    */
    gtk_main();

    return 0;
}


void callback_about (GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *dialog = NULL;

    dialog =  GTK_WIDGET (gtk_builder_get_object (data->builder, "AboutWindow"));
    gtk_dialog_run (GTK_DIALOG (dialog));

    gtk_widget_hide (dialog);
}

void get_img(GtkFileChooserButton *widget, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFileChooserButton *loader = NULL;
    loader = GTK_WIDGET(gtk_builder_get_object(data->builder, "img_loader"));

    g_print("loading image...\n");
    img_name = gtk_file_chooser_get_filename( loader );
    g_print("Image path: %s\n", img_name);
    GtkImage *image = NULL;
    image = (GtkImage*) (gtk_builder_get_object(data->builder, "image1"));

    gtk_image_set_from_file( image, (gchar*)img_name);
}
/*
void ocr_text (GtkButton *widget, gpointer user_data)
{
    SGloabalData *data = (SGlobalData*) user_data;
    GtkButton *ocr_button
}
*/























