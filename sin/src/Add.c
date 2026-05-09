#include "Add.h"
#include "math.h"

// page 127 4.2
void fast2Sum (double a, double b, double *s, double *t) {
    double dum;
    double z;

    // a должно быть больше или равно b для работы алгоритма
    if (fabs(b) > fabs(a)) {
        dum = a;
        a = b;
        b = dum;
    }
    // получим double a + b
    *s = a + b;
    // получим ошибочный b
    // вычели оригинальную a из неидеального a + b
    z = *s - a;
    // значение ошибки - разность ideal и неидеального b
    *t = b - z;
}
// page 130
// 4.4
void is2Sum (double a, double b, double *s, double *t) {
    double a_;
    double b_;
    double del_a;
    double del_b;

    // получим double a + b
    *s = a + b;
    // получим ошибочный b
    // вычтем оригинальную a из неидеального a + b
    a_ = *s - b;
    b_ = *s - a_;
    del_a = a - a_;
    del_b = b - b_;
    *t = del_a + del_b;

    // значение ошибки - разность ideal и неидеального b
}


dd dd_add(dd a, dd b) {
    double sh, sl, th, tl, c, vh, vl, wh, zh, zl;

    is2Sum(a.hm, b.hm, &sh, &sl);
    is2Sum(a.lm, b.lm, &th, &tl);

    c = sl + th;
    fast2Sum(sh, c, &vh, &vl);

    wh = tl + vl;
    fast2Sum(vh, wh, &zh, &zl);

    return (dd){zh, zl};
}
