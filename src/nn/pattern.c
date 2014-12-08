#include "pattern.h"

void gen_input(t_img_desc *img, double *dst)
{
    assert(img->comp == 1);

    for (int i = 0; i < img->x * img->y; ++i)
        dst[i] = img->data[i];
}

void gen_output(double *dst, size_t n, char c)
{
    assert(n == 52);
    for (size_t i = 0; i < n; ++i)
        dst[i] = 0;

    if ('a' <= c && c <= 'z')
        dst[c-'a'] = 1;
    else if ('A' <= c && c <= 'Z')
        dst[26+c-'A'] = 1;
}

char convert_output(double *src, size_t n)
{
    assert(n == 52);

    size_t i = 0;
    while (i < n && src[i] != 1)
        ++i;

    if (i < 26)
        return 'a' + i;
    else if (i < n)
        return 'A' + (i - 26);
    else
        return '\0';
}

void process_pattern(t_img_desc *img, int inversed)
{
    assert(img->comp == 1);
    for (int i = 0; i < img->x * img->y; ++i) {
        if (inversed)
            img->data[i] = (img->data[i] > 127) ? 1 : 0;
        else
            img->data[i] = (img->data[i] > 127) ? 0 : 1;
    }
}

struct training load_pattern(char *dir)
{
    char path[2048];
    char filename[2048];
    t_img_desc *img;

    strcpy(path, dir);
    for (dir = path; *dir; ++dir);
    if (*(--dir) != '/')
        strcat(path, "/");

    const size_t n_font = 7;
    const size_t n_letter = 52;
    const size_t n_set = n_letter * n_font;
    const size_t size_in = 400;
    const size_t size_out = 52;

    struct training t = { NULL, NULL, n_set, size_in, size_out };
    t.in = malloc(sizeof(double) * t.n_in * t.n_set);
    assert(t.in);
    t.out = malloc(sizeof(double) * t.n_out * t.n_set);
    assert(t.out);

    for (size_t f = 0; f < n_font; ++f) {
        /* Lower */
        for (size_t l = 0; l < n_letter / 2; ++l) {
            sprintf(filename, "%s%s/%c/%lu.png", path, "lower",
                    (char)l+'A', f);

            img = load_image(filename, 1);
            printf("[INFO] Load %s (%ix%i -- %i)\n", filename,
                    img->x, img->y, img->comp);

            process_pattern(img, 1);

            size_t line = l + f * n_letter;
            gen_input(img, get_in(&t, line));
            gen_output(get_out(&t, line), size_out, (char)l+'a');

            free_image(img);
        }

        /* Upper */
        for (size_t l = 0; l < n_letter / 2; ++l) {
            sprintf(filename, "%s%s/%c/%lu.png", path, "upper",
                    (char)l+'A', f);

            img = load_image(filename, 1);
            printf("[INFO] Load %s (%ix%i -- %i)\n", filename,
                    img->x, img->y, img->comp);

            process_pattern(img, 1);

            size_t line = (l + 26) + f * n_letter;
            gen_input(img, get_in(&t, line));
            gen_output(get_out(&t, line), size_out, (char)l+'A');

            free_image(img);
        }
    }

    return t;
}

void train_nn(struct net nwk, struct training t)
{
    unsigned epoch = 0;
    unsigned freq = 500;
    unsigned limit = 5000;

    do {
        unsigned error = 0;

        for (size_t i = 0; i < t.n_set; ++i) {
            net_compute(nwk, get_in(&t, i));
            net_error(nwk, get_out(&t, i));

            if (epoch % freq == 0) {
                char c = convert_output(get_out(&t, i), 52);
                char rst = convert_output(net_output(nwk), 52);
                if (c != rst) {
                    printf("In: %c\t", c);
                    printf("Out: %c\n", rst);
                    ++error;
                }
            }
        }

        if (epoch % freq == 0) {
            printf("[%d] ERROR: %d\n", epoch, error);
            printf("******************\n");
        }

        ++epoch;

        if (epoch == limit) {
            printf("Continue? ");
            int b = 0;
            scanf("%d", &b);
            if (b)
                epoch = 0;
        }

    } while(epoch < limit);
}

char ask_nn(struct net nwk, t_img_desc *img)
{
    assert(img->comp == 1);

    if (img->x != 20 || img->y != 20) {
        uchar *ptr = malloc(sizeof(char) * 20 * 20);
        stbir_resize_uint8(img->data, img->x, img->y, 0, ptr, 20, 20, 0, 1);

        img->data = ptr;
        img->x = 20;
        img->y = 20;
        img->comp = 1;
    }

    double in[400];
    gen_input(img, in);

    net_compute(nwk, in);
    double *out = net_output(nwk);
    return convert_output(out, 52);
}

void make_nn(char *path_pattern, char *saved_name)
{
    size_t desc_layers[] = { 400, 210, 52 };
    struct net nwk = net_init(3, desc_layers);
    struct training t = load_pattern(path_pattern);

    train_nn(nwk, t);
    net_save(nwk, saved_name);
    net_free(nwk);
}
