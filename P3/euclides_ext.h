/*
Desc: Libreria del algoritmo de Euclides
      y el algoritmo de Euclides Extendido.
Authors: Daniel Mateo
         Laura Sanchez
File: euclides_ext.h
*/

#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"

#define OK 1
#define ERROR 0

/*
* Function: int euclides_ext(mpz_t a, mpz_t b, mpz_t inv)
* Desc: Ejecuta el Algoritmo de Euclides extendido sobre
*       dos numeros a y b, donde el mayor es b (espacio)
*       y modifica el valor de inv, si son coprimos, con
*       el inverso multiplicativo de a en el espacio b.
*       Si no son coprimos, devuelve error.
* Ret: int - OK o ERROR
*/
int euclides_ext(mpz_t a, mpz_t b, mpz_t inv);

/*
* Function: void euclides(mpz_t a, mpz_t b, mpz_t d)
* Desc: Ejecuta el Algoritmo de Euclides sobre
*       dos numeros a y b, donde el mayor es a
*       y modifica el valor de d, con
*       el mcd de a y b.
*/
void euclides(mpz_t a, mpz_t b, mpz_t d);
