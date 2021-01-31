/*
Desc: Libreria de vegas.
Authors: Daniel Mateo
         Laura Sanchez
File: vegas.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#define POSIBLEPRIMO 3
#define COMPUESTO 4
#define NORESPONDE -1

/*
* Function: void genera_primo_n_bits(mpz_t primo, int bits, gmp_randstate_t seed)
* Desc: Genera un numero primo de bits bits de manera aleatoria,
*       guarda el primo generado en la variable primo.
*/
void genera_primo_n_bits(mpz_t primo, int bits, gmp_randstate_t seed);

/*
* Function: int prueba_tabla_primos(mpz_t p)
* Desc: Comprueba si el primo p no es divisible por ningun primo menor de 2k,
*       en caso de no serlo devuelve POSIBLEPRIMO y en el caso contrario devuelve
*       COMPUESTO.
* Ret: int - POSIBLEPRIMO o COMPUESTO o ERROR
*/
int prueba_tabla_primos(mpz_t p);

/*
* Function: int algoritmo_vegas(mpz_t d, mpz_t n, mpz_t e, mpz_t z)
* Desc: Realiza el algoritmo de las vegas, devuelve 1 en caso de haber encontrado
*       el valor de p o q, y en caso contrario devuelve NORESPONDE
* Ret: int - 1 o NORESPONDE
*/
int algoritmo_vegas(mpz_t d, mpz_t n, mpz_t e, mpz_t z);

/*
* Function: void calcula_k_m(mpz_t k, mpz_t m, mpz_t p, int bits)
* Desc: Calcula m y k tales que p-1 = (2^k)*m
*/
void calcula_k_m(mpz_t k, mpz_t m, mpz_t p);
