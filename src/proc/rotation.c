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
                        * M_PI;
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
    double theta = (winTheta / (double)indexMaxTheta) * M_PI;
    t_hough result = { rho, theta };
    return result;
}

double hough(t_img_desc *img)
{
    assert(img->comp == 1);

    double maxRho = sqrt(img->x * img->x + img->y * img->y);
    size_t indexMaxTheta = 180;
    size_t indexMaxRho = (size_t)(1 + maxRho);
    unsigned *acc = calloc(indexMaxTheta * indexMaxRho, sizeof(unsigned*));

    hough_compute(img, acc, maxRho, indexMaxTheta, indexMaxRho);
    t_hough result = hough_search_max(acc, maxRho, indexMaxTheta, indexMaxRho);

    free(acc);
    return result.theta;
}

static inline
int valid_rot(t_img_desc *img, int x, int y)
{
    return ((0 <= x && x < img->x) && (0 <= y && y <= img->y));
}

void rotate(t_img_desc *img, double theta)
{
    assert(img->comp == 1);

    int x_center = img->x / 2;
    int y_center = img->y / 2;

    int new_x = 0;
    int new_y = 0;

    if (fmod(theta, M_PI) < (M_PI / 2.)) {
        new_x = abs(ceil(2 * (((img->x - 1) - x_center) * cos(theta) +
                        (y_center) * sin(theta))));
        new_y = abs(ceil(2 * (((img->x - 1) - x_center) * sin(theta) +
                        ((img->y - 1) - y_center) * cos(theta))));
    } else {
        new_x = abs(ceil(2 * (((img->x - 1)- x_center) * cos(theta) -
                        ((img->y - 1) - y_center) * cos(theta))));
        new_y = abs(ceil(2 * (((img->x - 1) - x_center) * sin(theta) -
                        (y_center) * cos(theta))));
    }

    uchar *tmp = malloc(sizeof(char) * new_x * new_y);

    int new_x_center = new_x / 2;
    int new_y_center = new_y / 2;

    for (int y = 0; y < new_y; ++y) {
        for (int x = 0; x < new_x; ++x) {
            int rot_x = ceil(((x - new_x_center) * cos(theta) -
                        (y - new_y_center) * sin(theta)) + x_center);
            int rot_y = ceil(((x - new_x_center) * sin(theta) +
                        (y - new_y_center) * cos(theta)) + y_center);

            int c = 255;
            if (valid_rot(img, rot_x, rot_y))
                c = img->data[xytoi(rot_x, rot_y, img)];

            tmp[x + (new_x * y)] = c;
        }
    }

    free(img->data);
    img->data = tmp;
    img->x = new_x;
    img->y = new_y;
}

double rotate_img(t_img_desc *img)
{
    double theta = hough(img) - (M_PI / 2);

    /* Don't apply rotation if angle < 1 degree */
    if (fabs(theta) > 0.017)
        rotate(img, theta);

    return theta * (360 / (2 * M_PI));
}
