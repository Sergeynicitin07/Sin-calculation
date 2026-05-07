#include <stdio.h>
#include <stdlib.h>
#include "Add.h"
#include "struct.h"
#include "Mad.h"
#include "sin.h"



int main(int argc, char *argv[]) {
    printf("We should accomplish our own mission!\n");
    dd x = dd_2pi;
    if (argc > 1)
        x.hm = atof(argv[1]);
    if (argc > 2)
        x.lm = atof(argv[2]);
    dd res = sinx(x);
    printf("%.17le %.15le\n", res.hm, res.lm);
    return 0;
}
