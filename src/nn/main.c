#include "main.h"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

int main()
{
    net *nn = new_net(2,3);
    unsigned error, it = 1;
    double in[8] = { 0, 1, 1, 0, 0, 0, 1, 1 };
    double result = 0;

    do {
        if (it % 100 == 0) printf("it: %d\n", it);
        error = 0;

        result = net_train(nn, in, 2, 1);
        if (it % 100 == 0) printf("0 xor 1 = %f\n", result);
        if (result < 0.5)
            ++error;
        result = net_train(nn, &in[2], 2, 1);
        if (it % 100 == 0) printf("1 xor 0 = %f\n", result);
        if (result < 0.5)
            ++error;
        result = net_train(nn, &in[4], 2, 0);
        if (it % 100 == 0) printf("0 xor 0 = %f\n", result);
        if (result >= 0.5)
            ++error;
        result = net_train(nn, &in[6], 2, 0);
        if (it % 100 == 0) printf("1 xor 1 = %f\n", result);
        if (result >= 0.5)
            ++error;

        /*
        for (int i = 1; i >= 0; --i) {
            for (int j = 1; j >= 0; --j) {
                int answer = xor(i, j);
                double in[2] = { i? 1:-1, j?1:-1 };
                double result = net_train(nn, in, 2, answer);

                if (answer && result < 0.7)
                    ++error;
                else if (!answer && result > 0.3)
                    ++error;

                printf("\t%s%d xor %d = %d (%f)%s\n",
                        ((result >= 0.5 && answer) || (!answer && result < 0.5)) ? KNRM:KRED,
                        i, j, (result>=0.5)?1:0, result, KNRM);
            }
        }
        */
        if (error && it % 100 == 0)
            printf("%sERROR = %d%s\n", KRED, error, KNRM);
        ++it;
    } while (error && it <= 40000);

    printf("iteration: %d\n", it-1);
    return EXIT_SUCCESS;
}

int xor(int a, int b)
{
    return (a && !b) || (!a && b);
}
