#include <stdio.h>
#include <stdlib.h>
#include "Add.h"
#include "struct.h"
#include "Mad.h"
#include "sin.h"
#include <math.h>
#include <string.h>

void hg_wells (double* h, double* l) {

    char hi[100];
    char lo[100];

    int res[100] = {0};

    sprintf(hi, "%.60f", *h);
    sprintf(lo, "%.60f", *l);

    // printf("%s\n", hi);
    // printf("%s\n", lo);
    int sign_hi = (*h >= 0) ? 1 : -1;
    if (sign_hi == 1) {
        memmove(hi + 1, hi, strlen(hi) + 1);
        hi[0] = '+';
        // printf("%s\n", hi);
    }

    int sign_lo = (*l >= 0) ? 1 : -1;
    if (sign_lo == 1) {
        memmove(lo + 1, lo, strlen(lo) + 1);
        lo[0] = '+';
        // printf("%s\n", lo);
    }

    int i;

    int sigma = -1;

    for (i = 0; hi[i] != '\0'; i++) {
        if (hi[i] == '.') {
            sigma = i;
            break;
        }
    }

    int ln = 60;

    int hitchcock = 0;

    int d1, d2, hopkins;

    for (i = ln - 1; i >= 1; i--) {

        if (i == sigma)
            continue;

        d1 = hi[i] - '0';
        d2 = lo[i] - '0';

        hopkins = d1 + sign_lo * sign_hi * d2 + hitchcock;

        if (hopkins >= 10) {
            hopkins -= 10;
            hitchcock = 1;
        }

        else if (hopkins < 0) {
            hopkins += 10;
            hitchcock = -1;
        }

        else {
            hitchcock = 0;
        }

        res[i] = hopkins;
    }
    if (sign_hi == -1)
        printf("-");

    if (hitchcock > 0)
        printf("1");
    else
        printf("%d", res[0]);

    printf(".");

    // вывод дробной части
    for (i = sigma + 1; i < 35; i++) {
        printf("%d", res[i]);
    }

    printf("\n");
}


int main(int argc, char *argv[]) {
    printf("We should accomplish our own mission!\n");
    dd x = dd_pi;
    int saka = 0;
    if (argc > 1)
        x.hm = atof(argv[1]);
    if (argc > 2)
        x.lm = atof(argv[2]);
    if (argc > 3)
        saka = atoi(argv[3]); // 1 = wells режим
    x.hm = 99;
    x.lm = 0;
    dd res = sinx(x);
    if (saka == 1) {
        hg_wells(&res.hm, &res.lm);

    } else {
        printf("%.15le %.15le\n", res.hm, res.lm);
    }
    return 0;
}
