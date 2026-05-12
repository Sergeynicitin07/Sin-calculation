#include "Mad.h"
#include "struct.h"
#include "Add.h"
#include "mul.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// используем закон fabs(a_i + 1) <= 0.5 ULP(a_i)
// следующее за первым, второе число должно быть не больше чем одна вторая от значения последнего бита мантиссы с учетом порядка.
dd renormalize (double a0, double a1, double a2) {
    double t2;
    double s;
    // граница точности зависима от порядка числа. То есть порядок a0 индивидуален и дальнейшие числа зависимы от него.
    // при выполнении операции умножения мы можем владеть тремя числами полученными в конце
    // (a.h + a.l) * (b.h + b.l) = a.h * b.h + a.l * b.h + b.l * a.h + a.l * b.l
    // в a0 лежит основная часть числа: a.h * b.h
    // a1: a.l * b.h + b.l * a.h
    // a2: a.l * b.l лежат за 106 битом мантиссы относительно порядка a0
    // a2 + a1 может быть не равно a1, так как при сложении значений двух чисел,
    // CPU может округлить результат с учетом старшего значущего разряда a2
    // Ввиду переноса разряда


    //в s лежит сумма a1 и a2
    // t2 - то, что остается за суммой
    fast2Sum(a1, a2, &s, &t2);
    double t0, t1;
    // складываем в t0 числа a0 и s - засунем все, что влезет в 53 бита мантиссы
    // t0 + t1 = a0 + s
    // в t1 биты, что ушли за первые 53 бита
    fast2Sum(a0, s, &t0, &t1);

    double b0, b1;
    // за второй шаг t1 мог стать больше, чем половина младшего бита t0
    fast2Sum(t0, t1, &b0, &s);
    // в b1 лежат 54-106 биты
    fast2Sum(b0, s + t2, &b0, &b1);

    return (dd){b0, b1};
}

// так как синус периодическая функция (2 * Пи)

// чтобы не потерять в точности в синусе при вводе больших чисел, сделаем редуцирование - сравнение по пи модулю
dd reduce (dd x) {
    double n = round((x.hm + x.lm) /
                     (dd_2pi.hm + dd_2pi.lm));
    // умножаемые числа должны быть dd формата
    dd N = {n, 0.0};
    dd ms = dd_mul(N, dd_2pi);
    dd res;


    double s = -ms.hm;
    double t = -ms.lm;
    dd ms1 = {s, t};
    // x - (ms.hm + ms.lm) = x - ms.hm - ms.lm
    res = dd_add(x, ms1);
    return renormalize(res.hm, res.lm, 0.0);

}


void hg_wells (double* h, double* l) {
    int t = 0;
    char hi[600];
    char lo[600];

    int res[600] = {0};

    sprintf(hi, "%.540f", *h);
    sprintf(lo, "%.540f", *l);

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
    int aura = 0;
    int sigma = -1;

    for (i = 0; hi[i] != '\0'; i++) {
        if (hi[i] == '.') {
            sigma = i;
            break;
        }
    }

    int ln = 290;

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

    int j;
    if (hitchcock > 0) {
        printf("1");
        printf(".");

        // вывод дробной части
        for (i = sigma + 1; i < sigma + 33; i++) {
            printf("%d", res[i]);
        }

        printf("e+00");
    } else {
        int can_you_hope_me_please = 0;
        for (i = 2; i < 250; i ++) {
            if (res[i] == 0) {
                t --;

            } else {
                can_you_hope_me_please = i;
                break;
            }
        }
        if (t < -230) {
            printf("0");
            printf(".");

            // вывод дробной части
            for (i = sigma + 1; i < 35; i++) {
                printf("%d", res[i]);
            }
            printf("e+00");

        } else {
            printf("%d", res[can_you_hope_me_please]);
            printf(".");

            // вывод дробной части
            for ( j = can_you_hope_me_please + 1; j < can_you_hope_me_please + 33; j++) {
                printf("%d", res[j]);
            }
            if (abs(t) < 10) {
                printf("e-0%d", abs(t));
            } else {
                printf("e-%d", abs(t));

            }
        }



    }printf("\n");





}
#include "Mad.h"
#include "struct.h"
#include "Add.h"
#include "mul.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// используем закон fabs(a_i + 1) <= 0.5 ULP(a_i)
// следующее за первым, второе число должно быть не больше чем одна вторая от значения последнего бита мантиссы с учетом порядка.
dd renormalize (double a0, double a1, double a2) {
    double t2;
    double s;
    // граница точности зависима от порядка числа. То есть порядок a0 индивидуален и дальнейшие числа зависимы от него.
    // при выполнении операции умножения мы можем владеть тремя числами полученными в конце
    // (a.h + a.l) * (b.h + b.l) = a.h * b.h + a.l * b.h + b.l * a.h + a.l * b.l
    // в a0 лежит основная часть числа: a.h * b.h
    // a1: a.l * b.h + b.l * a.h
    // a2: a.l * b.l лежат за 106 битом мантиссы относительно порядка a0
    // a2 + a1 может быть не равно a1, так как при сложении значений двух чисел,
    // CPU может округлить результат с учетом старшего значущего разряда a2
    // Ввиду переноса разряда


    //в s лежит сумма a1 и a2
    // t2 - то, что остается за суммой
    fast2Sum(a1, a2, &s, &t2);
    double t0, t1;
    // складываем в t0 числа a0 и s - засунем все, что влезет в 53 бита мантиссы
    // t0 + t1 = a0 + s
    // в t1 биты, что ушли за первые 53 бита
    fast2Sum(a0, s, &t0, &t1);

    double b0, b1;
    // за второй шаг t1 мог стать больше, чем половина младшего бита t0
    fast2Sum(t0, t1, &b0, &s);
    // в b1 лежат 54-106 биты
    fast2Sum(b0, s + t2, &b0, &b1);

    return (dd){b0, b1};
}

// так как синус периодическая функция (2 * Пи)

// чтобы не потерять в точности в синусе при вводе больших чисел, сделаем редуцирование - сравнение по пи модулю
dd reduce (dd x) {
    double n = round((x.hm + x.lm) /
                     (dd_2pi.hm + dd_2pi.lm));
    // умножаемые числа должны быть dd формата
    dd N = {n, 0.0};
    dd ms = dd_mul(N, dd_2pi);
    dd res;


    double s = -ms.hm;
    double t = -ms.lm;
    dd ms1 = {s, t};
    // x - (ms.hm + ms.lm) = x - ms.hm - ms.lm
    res = dd_add(x, ms1);
    return renormalize(res.hm, res.lm, 0.0);

}


void hg_wells (double* h, double* l) {
    int t = 0;
    char hi[300];
    char lo[300];

    int res[400] = {0};

    sprintf(hi, "%.290f", *h);
    sprintf(lo, "%.290f", *l);

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
    int aura = 0;
    int sigma = -1;

    for (i = 0; hi[i] != '\0'; i++) {
        if (hi[i] == '.') {
            sigma = i;
            break;
        }
    }

    int ln = 290;

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

    int j;
    if (hitchcock > 0) {
        printf("1");
        printf(".");

        // вывод дробной части
        for (i = sigma + 1; i < sigma + 33; i++) {
            printf("%d", res[i]);
        }

        printf("e+00");
    } else {
        int can_you_hope_me_please = 0;
        for (i = 2; i < 250; i ++) {
            if (res[i] == 0) {
                t --;

            } else {
                can_you_hope_me_please = i;
                break;
            }
        }
        if (t < -230) {
            printf("0");
            printf(".");

            // вывод дробной части
            for (i = sigma + 1; i < 35; i++) {
                printf("%d", res[i]);
            }
            printf("e+00");

        } else {
            printf("%d", res[can_you_hope_me_please]);
            printf(".");

            // вывод дробной части
            for ( j = can_you_hope_me_please + 1; j < can_you_hope_me_please + 33; j++) {
                printf("%d", res[j]);
            }
            if (abs(t) < 10) {
                printf("e-0%d", abs(t));
            } else {
                printf("e-%d", abs(t));

            }
        }



    }printf("\n");





}
