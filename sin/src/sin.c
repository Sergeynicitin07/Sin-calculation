#include "sin.h"
#include "struct.h"
#include <math.h>
#include "Mad.h"
#include "mul.h"
#include "Add.h"
#include <stdio.h>
#include <stdlib.h>

// синус через ряд Тейлора
dd dd_neg(dd a) {
    return (dd){-a.hm, -a.lm};
}

dd sin_taylor(dd x) {
    dd x2 = dd_mul(x, x);
    dd res = x;
    dd l = x;
    int j = 0;
    dd elba;
    dd siemens;
    int co = 0;
    x2 = renormalize(x2.hm, x2.lm, 0.0);
    int i = 0;
    for (i = 0; i < 19; i ++) {

        l = dd_mul(x2, l);
        l = renormalize(l.hm, l.lm, 0.0);
        if (co % 2 == 0) {
            siemens.hm = -l.hm;
            siemens.lm = -l.lm;
        } else {
            siemens.hm = l.hm;
            siemens.lm = l.lm;
        }

        elba = dd_mul(factorial_sarmat[j], siemens);
        j ++;
        elba = renormalize(elba.hm, elba.lm, 0.0);
        co ++;

        res = dd_add(res, elba);
        res = renormalize(res.hm, res.lm, 0.0);
        if (fabs(elba.hm) < fabs(res.hm) * 1e-32) break;
    }
    return res;
}




dd dd_reduce(dd x, int *demetre) {
    double time_limit1 = 5.0;
    double start = get_time();
    int supremus = 0;
    if (fabs(x.hm)> 0.7853981633974483 && fabs(x.hm) < 1e6 + 1e5) {
        *demetre = 0;
        return reduce(x);

    }

    double k = round(x.hm * inv_pi[0]);
    k += round(x.hm * inv_pi[1] + x.lm * inv_pi[0]);

    double k_high = floor(k * 1e-9) * 1e9;
    double k_low = k - k_high;

    dd r = x;
    double parts_k[] = {k_high, k_low};
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 4; j++) {
            dd term = dd_mul((dd){parts_k[i], 0}, (dd){p[j], 0});
            r = dd_add(r, dd_neg(term));

        }
    }

    double q = fmod(k, 4.0);
    if (q < 0) q += 4.0;
    *demetre = (int)q;

    // Коррекция если r не лежит в интервале [-pi/4, pi/4]
    while (r.hm > 0.7853981633974483) {
        double elapsed = get_time() - start;
        if (elapsed > time_limit1) {
            printf("Time\tis\tover\n");
            supremus = 1;
        }
        if (supremus) {
            printf("We\tshould\tstop\tall.\n");
            exit(0);
        }

        r = dd_add(r, dd_neg((dd){p[0], p[1]}));
        *demetre = (*demetre + 1) % 4;
    }
    while (r.hm < -0.7853981633974483) {
        double elapsed = get_time() - start;
        if (elapsed > time_limit1) {
            printf("Time\tis\tover\n");
            supremus = 1;
        }
        if (supremus) {
            printf("We\tshould\tstop\tall.\n");
            exit(0);
        }

        r = dd_add(r, (dd){p[0], p[1]});
        *demetre = (*demetre + 3) % 4;
    }

    return r;
}




dd cos_taylor(dd x) {


    dd x2 = dd_mul(x, x);
    dd res = (dd){1.0, 0.0};
    dd p = (dd){1.0, 0.0};
    dd term;

    int sign = -1;
    for (int i = 0; i < 15; i++) {
        p = dd_mul(p, x2);


        term = dd_mul(p, factorial_even[i]);

        if (sign < 0) {
            res = dd_add(res, (dd){-term.hm, -term.lm});
        } else {
            res = dd_add(res, term);
        }

        // Условие выхода по точности
        if (fabs(term.hm) < fabs(res.hm) * 1e-32) {
            break;
        }

        sign = -sign;
    }

    return res;
}



dd sinx(dd a) {
    int negate = 0;
    if (a.hm < 0.0) {
        a = dd_neg(a);
        negate = 1;
    }
    int demetre;
    dd r = dd_reduce(a, &demetre);
    dd res;
    switch (demetre) {
        case 0:
            res = sin_taylor(r);
            break;
        case 1:
            res = cos_taylor(r);
            break;
        case 2:
            res = dd_neg(sin_taylor(r));
            break;
        case 3:
            res = dd_neg(cos_taylor(r));
            break;
    }

    return negate ? dd_neg(res) : res;
}
