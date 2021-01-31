/*
Desc: Libreria del Algoritmo de Euclides
      Extendido.
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
* Function: int euclides_ext(char* a, char* b, mpz_t inv)
* Desc: Ejecuta el Algoritmo de Euclides extendido sobre
*       dos numeros a y b, donde el mayor será m (espacio)
*       y modifica el valor de inv, si son coprimos, con 
*       su inverso multiplicativo en el espacio m.
*       Si no son coprimos, devuelve error.
* Ret: int - OK o ERROR
*/
int euclides_ext(char* a, char* b, mpz_t inv);
