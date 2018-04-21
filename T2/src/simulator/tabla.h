#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Tuple {
    double espacio_total;
    double espacio_direccion;
};

void optimo_1();
void optimo_2();
void optimo_3();
void optimo_4();
void optimo_5();
struct Tuple T_2(int i);
struct Tuple T_3(int i, int j);
struct Tuple T_4(int i, int j, int p);
struct Tuple T_5(int i, int j, int p, int q);
