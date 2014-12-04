#ifndef MAIN_H_
#define MAIN_H_

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <gtkspell/gtkspell.h>

typedef struct
{
        GtkBuilder *builder;
            gpointer user_data;
} SGlobalData;

gchar *img_name;
gchar *txt_ocr = "test\tbloblo\nbli";
const gchar *txt_saved_name;
gchar *txt_saved_path;

GtkTextView *text_view = NULL;
GtkTextBuffer *buffer = NULL;
GtkWidget *dialog_save = NULL;
GtkButton *b_save = NULL;

void callback_about (GtkMenuItem *menuitem, gpointer user_data);
void get_img (GtkFileChooser *wigdet, gpointer user_data);
void ocr_text (GtkButton *widget, gpointer user_data);
void save_text( GtkButton *widget, gpointer user_data);
void save_dial (GtkButton *widget, gpointer user_data);
void empty_buffer(GtkMenuItem *menuitem, gpointer user_data);


#endif
