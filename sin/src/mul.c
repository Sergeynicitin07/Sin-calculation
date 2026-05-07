#include "mul.h"
#include <math.h>


void veltkamp (double x, double *x_high, double *x_low) {
    // в double 64 бита - 1 sign, 53 у мантисса и 10 у порядок
    // C = 2^27 + 1 - константа Велткампа для p = 53
    double C = 134217729.0;
    // Все кроме 26 старших бит должны обнулиться путем сдвига.
    double gamma = C * x;
    double delta = x - gamma;
    // x_high содержит старшие 26 бит мантиссы
    *x_high = gamma + delta;
    // x_low хранит остальное
    *x_low = x - *x_high;
}
// page 135
// 4.4.2
void dekker (double x, double y, double *r_1, double *r_2) {
    double x_high, x_low;
    double y_high, y_low;
    double t_1;
    double t_2;
    double t_3;
    // split
    veltkamp(x, &x_high, &x_low);
    veltkamp(y, &y_high, &y_low);
    // обрубленное значение x * y
    *r_1 = x * y;
    // вычитаем результат x * y из произведения старших частей мантисс двух чисел
    t_1 = -*r_1 + x_high * y_high;
    // добавляем перекрестное произведение
    t_2 = t_1 + x_high * y_low;
    t_3 = t_2 + x_low * y_high;
    // добавляем произведение хвостов
    *r_2 = t_3 + x_low * y_low;
}


dd dd_mul (dd a, dd b) {
    double s, e;
    dd res;
 
    dekker(a.hm, b.hm, &s, &e);
    e += a.lm * b.hm;
    e += a.hm * b.lm;

    // сложение с упрощением
    res.hm = s + e;
    // компенсация
    res.lm = e - (res.hm - s) ;
    return res;

}
