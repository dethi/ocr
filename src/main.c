#include "main.h"

int main(int argc, char *argv [])
{
    GtkWidget *main_window = NULL;
    SGlobalData data;
    GError *error = NULL;
    gchar *filename = NULL;

    gtk_init(&argc, &argv);
    data.builder = gtk_builder_new();

    filename =  g_build_filename ("gui.glade", NULL);
    gtk_builder_add_from_file(data.builder, filename, &error);
    g_free(filename);

    if (error) {
        gint code = error->code;
        g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
    }

    gtk_builder_connect_signals (data.builder, &data);
    main_window = GTK_WIDGET(gtk_builder_get_object (data.builder, "window1"));
    gtk_widget_show_all (main_window);

    text_view = (GtkTextView*) (gtk_builder_get_object(data.builder, "textview1"));
    buffer = gtk_text_view_get_buffer (text_view);
    gtk_text_buffer_set_text(buffer, "", strlen(""));

    spell = gtk_spell_checker_new();
    gtk_spell_checker_attach( spell, text_view);
    gtk_spell_checker_set_language( spell, "fr", NULL);

    gtk_window_set_title (GTK_WINDOW(main_window), "OhCaSert by (Neurone)*");

    gtk_main();

    return 0;
}

// about window + credits window
void callback_about (GtkMenuItem *menuitem, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object (data->builder, "AboutWindow"));
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_hide(dialog);
}

// load and print the choosen image, also get the pathname
void get_img(GtkFileChooser *widget, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFileChooser *loader = GTK_FILE_CHOOSER(gtk_builder_get_object(data->builder, "img_loader"));
    GtkImage *image = (GtkImage*) (gtk_builder_get_object(data->builder, "image1"));

    img_name = (gchar*) gtk_file_chooser_get_filename(loader);

    GtkLabel *label_img = GTK_LABEL(gtk_builder_get_object(data->builder, "label3"));
    gtk_label_set_text(label_img, img_name);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(img_name, 680, 500, NULL);
    gtk_image_set_from_pixbuf(image, pixbuf);
}

// image processing in a different thread
void* thread_processing(void *arg)
{
    t_img_desc *img = load_image(img_name, 3);

    printf("[INFO] Load %s ( %ix%i -- %i)\n", img_name, img->x, img->y, img->comp);

    grey_scale(img);
    filter_median(img);
    binarize_otsu(img);

    write_image("out_img.png", img);
    printf("[INFO] Write img_out.png\n");

    free_image(img);

    pthread_exit(NULL);
}

// print the text produce by the ocr (actually just print "test" for now)
void ocr_text (GtkButton *widget, gpointer user_data)
{
    /* Process image
     * img_name = path to image
     * txt_ocr = text that have to be processed
     */

    t_img_desc *img = load_image(img_name, 3);
    printf("[INFO] Load %s (%ix%i -- %i)\n", img_name, img->x, img->y, img->comp);

    grey_scale(img);
    binarize_otsu(img);

    printf("[INFO] Rotation of %.2f degree\n", rotate_img(img));
    binarize_otsu(img);
    
    struct coorList *l = malloc(sizeof(struct coorList));
    XYCut(img->data, (char)0, (size_t)img->x, (size_t)img->y, 10, 0, 0, l);
    free(img);
    img->data = l->data;
    img->x = l->X;
    img->y = l->Y;
    struct coorList *aux = NULL;
    while (l->next != NULL) {
        aux = l->next;
        free(l);
        l = aux;
    }
    free(aux);
    
    write_image("out_img.png", img);
    printf("[INFO] Write img_out.png\n");
    free_image(img);

    /* End process image */

    // Multithreading, not used yet.
    //pthread_t thread;
    //pthread_create(&thread, NULL, thread_processing, NULL);

    gtk_text_buffer_set_text (buffer, txt_ocr, strlen(txt_ocr));
}

void save_dial (GtkButton *widget, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;

    dialog_save = GTK_WIDGET(gtk_builder_get_object(data->builder, "dialog_save"));
    b_save = GTK_BUTTON(gtk_builder_get_object(data->builder, "button_save"));

    g_signal_connect_swapped(b_save, "clicked", (GCallback) gtk_widget_hide, dialog_save);

    gtk_dialog_run(GTK_DIALOG(dialog_save));
    gtk_widget_hide(dialog_save);
}

void save_text (GtkButton *widget, gpointer user_data)
{
    SGlobalData *data = (SGlobalData*) user_data;
    GtkFileChooser *chooser = NULL;
    chooser = GTK_FILE_CHOOSER(dialog_save);
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(data->builder, "entry1"));

    txt_saved_name = gtk_entry_get_text(entry);
    txt_saved_path = gtk_file_chooser_get_current_folder(chooser);

    GtkTextIter iter_start, iter_end;

    gtk_text_buffer_get_start_iter(buffer, &iter_start);
    gtk_text_buffer_get_end_iter(buffer, &iter_end);

    gchar *text2save = gtk_text_buffer_get_text(buffer, &iter_start, &iter_end, TRUE);

    gchar *txt_saved = (gchar*) strcat(txt_saved_path, "/");
    txt_saved = (gchar*) strcat(txt_saved, txt_saved_name);
    txt_saved = (gchar*) strcat(txt_saved, ".txt");

    FILE *file = fopen(txt_saved, "w");

    if (file == NULL) {
        g_print("Error when writing .txt file !\n");
        return;
    }

    fputs(text2save, file);
    fputc('\n', file);

    GtkLabel *label_txt = NULL;
    label_txt = GTK_LABEL(gtk_builder_get_object(data->builder, "label4"));
    gtk_label_set_text(label_txt, txt_saved);

    fclose(file);
}

void empty_buffer( GtkMenuItem *menuitem, gpointer user_data)
{
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);

    gtk_text_buffer_delete(buffer, &start, &end);
}
