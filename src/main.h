#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stb_image/stb_image_ext.h"
#include "proc/preprocessing.h"
#include "proc/detection.h"
#include "proc/rotation.h"
#include "proc/filter.h"
#include "proc/xy-cut.h"
//#include "nn/xor.h"

#include <gtk/gtk.h>
#include <gtkspell/gtkspell.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    GtkBuilder *builder;
    gpointer user_data;
} SGlobalData;

gchar txt_ocr[] = "Nothing.";
gchar txt_saved[2048];
gchar img_name[2048];

GtkTextView *text_view = NULL;
GtkTextBuffer *buffer = NULL;
GtkWidget *dialog_save = NULL;
GtkButton *b_save = NULL;
GtkSpellChecker *spell = NULL;

void callback_about(GtkMenuItem * menuitem, gpointer user_data);
void get_img(GtkFileChooser * wigdet, gpointer user_data);
void processing();
void ocr_text(GtkButton * widget, gpointer user_data);
void save_text(GtkButton * widget, gpointer user_data);
void save_dial(GtkButton * widget, gpointer user_data);
void empty_buffer(GtkMenuItem * menuitem, gpointer user_data);

#endif
