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
#include "nn/nn.h"
#include "nn/pattern.h"

int main(int argc, char *argv[]);

#endif
