#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmdline.h"
#include "stb_image/stb_image_ext.h"
#include "proc/preprocessing.h"
#include "proc/detection.h"
#include "proc/filter.h"
#include "nn/xor.h"

int main(int argc, char *argv[]);
void writing_error();

#endif
