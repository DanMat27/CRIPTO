/*
Desc: Algoritmo de Euclides
      Calcula el maximo comun divisor
Author: Daniel Mateo
File: euclides.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "euclides.h"

int mcd(int m, int a){
    int aux;
    int r, rplus1, rplus2, coc;

    if(m <= a){
        aux = m;
        m = a;
        a = m;
    }

    r = m;
    rplus1 = a;
    rplus2 = r % rplus1;
    coc = r / rplus1;

    while(rplus2 != 0){
        r = rplus1;
        rplus1 = rplus2;
        rplus2 = r % rplus1;
        coc = r / rplus1;
    }

    return rplus1;
}
