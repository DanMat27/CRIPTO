/*
Desc: Libreria de primo.
Authors: Daniel Mateo
         Laura Sanchez
File: primo.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"

#define OK 0
#define ERROR -1
#define TECLADO 1
#define OFILE 2
#define POSIBLEPRIMO 3
#define COMPUESTO 4
#define NTEST 50 // Numero de tests de Miller-Rabin

/*
* Function: int test_miller_rabin(mpz_t p, mpz_t k, mpz_t m, int bits, double sec)
* Desc: Realiza el test Miller-Rabin devuelve POSIBLEPRIMO en caso de cumplirlo
*       y en caso contrario COMPUESTO
* Ret: int - POSIBLEPRIMO o COMPUESTO
*/
int test_miller_rabin(mpz_t p, mpz_t k, mpz_t m, int bits, double sec);

/*
* Function: void calcula_k_m(mpz_t k, mpz_t m, mpz_t p, int bits)
* Desc: Calcula m y k tales que p-1 = (2^k)*m
*/
void calcula_k_m(mpz_t k, mpz_t m, mpz_t p, int bits);

/*
* Function: int prueba_tabla_primos(mpz_t p)
* Desc: Comprueba si el primo p no es divisible por ningun primo menor de 2k,
*       en caso de no serlo devuelve POSIBLEPRIMO y en el caso contrario devuelve
*       COMPUESTO.
* Ret: int - POSIBLEPRIMO o COMPUESTO o ERROR
*/
int prueba_tabla_primos(mpz_t p);

/*
* Function: int read_args(char* argv[], int argc, int *bits, double *sec, char* ofile)
* Desc: Lee los argumentos escritos por terminal y los mete en la variable correspondiente
* Ret: int - OK o ERROR
*/
int read_args(char* argv[], int argc, int *bits, double *sec, char* ofile);
