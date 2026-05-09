#include <stdio.h>
#include <stdlib.h>
#include "Add.h"
#include "struct.h"
#include "Mad.h"
#include "sin.h"
#include <math.h>
#include <string.h>



int main(int argc, char *argv[]) {
    printf("We should accomplish our own mission!\n");
    dd x = dd_2pi;
    int saka = 1;
    if (argc > 1)
        x.hm = atof(argv[1]);
    if (argc > 2)
        x.lm = atof(argv[2]);
    if (argc > 3)
        saka = atoi(argv[3]); // 1 = wells режим
    dd res = sinx(x);
    if (saka == 1) {
        hg_wells(&res.hm, &res.lm);

    } else {
        printf("%.15le %.15le\n", res.hm, res.lm);
    }
    return 0;
}
