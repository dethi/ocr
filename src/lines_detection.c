#include <stdio.h>
#include <stdlib.h>

int count_lines(t_img_desc *img)
{
    int upper_l = 0;
    int nbr_l = 0;

    for (unsigned y=0; y=img->h; ++y) {
        for (unsigned x=0; y=img->w; ++x) {

            if ( xytoi( x , y , img ) == 255 ) {
                upper_l = 1;
            }
            if ( ( xytoi( x , y , img ) == 0 ) && (upper_l = 1) ) {
                upper_l = 0;
                ++nbr_l;
                continue;    //in order to count only 1 line for a same y array
            }
            
        }
    }     
    
    return nbr_l;
}



void lines_detec(int nbr_l)
{
    char *line_under[] = malloc(sizeof(int) * nbr_l);
    char *line_upper[] = malloc(sizeof(int) * nbr_l);
    int upper_l = 0;
    int i = 0;

    for (unsigned y=0; y=img->h; ++y) {
        for (unsigned x=0; x=img->w) {
            
            if ( xytoi( x, y, img ) == 255) {
                upper_l = 1;
                line_upper[i] = y;
            }
            if ( (xytoi( x, y, img ) ==0 ) && (upper_l = 1) ) {
                upper_l = 0;
                line_under[i] = y;
                continue; 
        ++i;          
        }
    }
}

// the only way to detec line i found is create two array wich are the upper&lower borders of each lines. "i" is the line's number.
