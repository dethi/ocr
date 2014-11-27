#include <gtk/gtk.h>

typedef struct
{
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;


gchar *img_name;
gchar *txt_ocr = "test";

void callback_about (GtkMenuItem *menuitem, gpointer user_data);
void get_img (GtkFileChooserButton *wigdet, gpointer user_data);
void ocr_text (GtkButton *widget, gpointer user_data);
void save_text( GtkButton *widget, gpointer user_data);

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

    gtk_main();

    return 0;
}

// about window + credits window
void callback_about (GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *dialog = NULL;

    dialog =  GTK_WIDGET (gtk_builder_get_object (data->builder, "AboutWindow"));
    gtk_dialog_run (GTK_DIALOG (dialog));

    gtk_widget_hide (dialog);
}

// load and print the choosen image, also get the pathname
void get_img(GtkFileChooserButton *widget, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFileChooserButton *loader = NULL;
    GtkImage *image = NULL;

    loader = (GtkFileChooserButton*) (gtk_builder_get_object(data->builder, "img_loader"));
    image = (GtkImage*) (gtk_builder_get_object(data->builder, "image1"));

    g_print("loading image...\n");
    img_name =  gtk_file_chooser_get_filename(  loader );
    g_print("Image path: %s\n", img_name);

    gtk_image_set_from_file( image, (gchar*)img_name);
}
// print the text produce by the ocr (actually just print "test" for now)
void ocr_text (GtkButton *widget, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkTextView *text_view = NULL;
    GtkTextBuffer *buffer = NULL;

    text_view = (GtkTextView*) ( gtk_builder_get_object( data->builder, "textview1")); 
    buffer = gtk_text_view_get_buffer ( text_view );

    gtk_text_buffer_set_text ( buffer, txt_ocr, strlen(txt_ocr) );
}
/*
void save_text (GtkButton *widget, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkTextView *text_view = NULL;
    GtkTextBuffer *buffer = NULL;
    
    
    text_view = (GtkTextView*) ( gtk_builder_get_object( data->builder, "textview1"));
    buffer = gtk_text_view_get_buffer ( text_view );

    GtkTextIter *iter_start = NULL;
    GtkTextIter *iter_end = NULL;

    gtk_text_buffer_get_start_iter ( buffer, iter_start);
    gtk_text_buffer_get_end_iter ( buffer, iter_end);
    
    gchar *text2save = gtk_text_iter_get_slice ( iter_start, iter_end);
    //gint *len2save = gtk_text_buffer_char_count ( buffer );
    
    FILE *file = NULL;
    file = fopen("file.txt", "w");

    if ( file == NULL ) {
        g_print("Error in opening .txt file !\n");
        exit(1);
    }
    g_print(".txt saving...\n");

    fputs( text2save, file);
    //fwrite ( text2save, 1, sizeof(text2save), file);
    
    fclose ( file );
}
*/



















