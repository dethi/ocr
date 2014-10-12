#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <err.h>

#define LEARNING 1e-3

void printf_tab(float tab[], size_t n)
{
    for (size_t i = 0; i < n; ++i)
        printf("%.4f\t", tab[i]);
    printf("\n");
}

void printf_result(float in[], float w[], size_t n, int sum, int out)
{
    printf("Inputs: ");
    printf_tab(in, n);
    printf("Weights: ");
    printf_tab(w, n);
    printf("Sum: %d\n", sum);
    printf("Out: %d\n\n", out);
}

float* set_weights(size_t n)
{
    float *w = malloc(sizeof(float) * n);
    if (!w)
        return NULL;

    srand(time(NULL));
    for (size_t i = 0; i < n; ++i)
        w[i] = -1 + 2 * ((float)rand() / RAND_MAX);

    return w;
}

float sigma(float in[], float w[], size_t n)
{
    float sum = 0;
    for (size_t i = 0; i < n; ++i)
        sum += in[i] * w[i];
    return sum;
}

int activate(float sum)
{
    return (sum > 0) ? 1 : 0;
}

int feedforward(float in[], float w[], size_t n)
{
    int sum = sigma(in, w, n);
    int out = activate(sum);
    printf_result(in, w, n, sum, out);
    return out;
}

int train(float in[], float w[], size_t n, int desired)
{
    int guess = feedforward(in, w, n);
    int error = desired - guess;

    if (error != 0) {
        for (size_t i = 0; i < n; ++i)
            w[i] += error * LEARNING * in[i];
    }

    return (error >= 0) ? error : -1 * error;
}

int answer(int a, int b)
{
    return a && b;
}

int main()
{
    size_t n = 3;
    float *w = set_weights(n);
    if (!w)
        exit(EXIT_FAILURE);

    float in1[] = { 1, 0, 0};
    float in2[] = { 1, 0, 1};
    float in3[] = { 1, 1, 0};
    float in4[] = { 1, 1, 1};

    unsigned pass = 50000;
    float *in;

    for (unsigned i = 0; i < pass; ++i) {
        int error = 0;

        printf("PASS %d:\n", i+1);
        in = in1;
        error += train(in, w, n, answer(in[1], in[2]));
        in = in2;
        error += train(in, w, n, answer(in[1], in[2]));
        in = in3;
        error += train(in, w, n, answer(in[1], in[2]));
        in = in4;
        error += train(in, w, n, answer(in[1], in[2]));
        printf("\n");

        if (error == 0)
            break;
    }

    printf("0 0 = %d\n", answer(0, 0));
    printf("0 1 = %d\n", answer(0, 1));
    printf("1 0 = %d\n", answer(1, 0));
    printf("1 1 = %d\n", answer(1, 1));

    free(w);
}
