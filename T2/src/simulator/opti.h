#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Tuple {
    double espacio_total;
    double espacio_direccion;
};

struct info_bits {
    int b1;
    int b2;
    int b3;
    int b4;
    int b5;
};

struct info_bits optimo_1();
struct info_bits optimo_2();
struct info_bits optimo_3();
struct info_bits optimo_4();
struct info_bits optimo_5();
struct Tuple T_2(int i);
struct Tuple T_3(int i, int j);
struct Tuple T_4(int i, int j, int p);
struct Tuple T_5(int i, int j, int p, int q);
