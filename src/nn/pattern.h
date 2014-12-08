#ifndef _PATTERN_H_
#define _PATTERN_H_

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../stb_image/stb_image_ext.h"
#include "nn.h"

void gen_input(t_img_desc *img, double *dst);
void gen_output(double *dst, size_t n, char c);
char convert_output(double *src, size_t n);
void process_pattern(t_img_desc *img, int inversed);
struct training load_pattern(char *dir);

char ask_nn(struct net nwk, t_img_desc *img);
void train_nn(struct net nwk, struct training t);
void make_nn(char *path_pattern, char *saved_name);

#endif
