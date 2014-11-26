#include "rotation.h"

static
void hough_compute(t_img_desc *img, unsigned *acc, double maxRho,
        size_t indexMaxTheta, size_t indexMaxRho)
{
    for (int i = 0; i < img->x; ++i) {
        for (int j = 0; j < img->y; ++j) {
            if (img->data[xytoi(i, j, img)] == 0) {

                int x = i - (img->x / 2);
                int y = j - (img->y / 2);

                for (size_t i_theta = 0; i_theta < indexMaxTheta; ++i_theta) {
                    double theta = ((double)i_theta / (double)indexMaxTheta)
                        *  2 * M_PI;
                    double rho = x * cos(theta) + y * sin(theta);
                    size_t i_rho = 0.5 + (rho / maxRho + 0.5) * indexMaxRho;
                    ++acc[i_theta + i_rho * indexMaxTheta];
                }
            }
        }
    }
}

static
t_hough hough_search_max(unsigned *acc, double maxRho,
        size_t indexMaxTheta, size_t indexMaxRho)
{
    double max = 0;
    double winRho = 0;
    double winTheta = 0;

    for (size_t r = 0; r < indexMaxRho; ++r) {
        for (size_t t = 0; t < indexMaxTheta; ++t) {
            if (acc[t + r * indexMaxTheta] > max) {
                max = acc[t + r * indexMaxTheta];
                winRho = r;
                winTheta = t;
            }
        }
    }

    double rho = (winRho / (double)indexMaxRho - 0.5) * maxRho;
    double theta = (winTheta / (double)indexMaxTheta) * (2 * M_PI);
    t_hough result = { rho, theta };
    return result;
}

double hough(t_img_desc *img)
{
    if (img->comp != 1)
        return -1;

    double maxRho = sqrt(img->x * img->x + img->y * img->y);
    size_t indexMaxTheta = 360;
    size_t indexMaxRho = (size_t)(1 + maxRho);
    unsigned *acc = calloc(indexMaxTheta * indexMaxRho, sizeof(unsigned*));

    hough_compute(img, acc, maxRho, indexMaxTheta, indexMaxRho);
    t_hough result = hough_search_max(acc, maxRho, indexMaxTheta, indexMaxRho);

    //printf("\nroh: %f\ttheta: %f\n", result.rho, result.theta);
    free(acc);
    return result.theta;
}
