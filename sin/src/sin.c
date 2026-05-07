#include "sin.h"
#include "struct.h"
#include <math.h>
#include "Mad.h"
#include "mul.h"
#include "Add.h"

// найдем синус через ряд Тейлора
dd sinx (dd x) {
    if (x.hm > dd_pi.hm / 4) {
        x = reduce(x);
    }
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

        res = dd_add1(res, elba);
        res = renormalize(res.hm, res.lm, 0.0);
        if (fabs(elba.hm) < fabs(res.hm) * 1e-32) break;
    }
    return res;
}