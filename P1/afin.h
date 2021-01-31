/*
Desc: Libreria del Metodo Afin.
Authors: Daniel Mateo
         Laura Sanchez
File: afin.h
*/

#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"

#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in
#define OFILE 4 //Modo con fichero out
#define IOFILES 5 //Modo con ficheros in y out

/*
* Function: int read_args_afin(char* argv[], int argc, mpz_t m, mpz_t a, mpz_t b, char* ifile, char* ofile)
* Desc: Lee los argumentos introducidos y comprueba que los datos sean correctos,
*       modificando los valores de m, a, b, ifile y ofile con ellos.
*       Devuelve el modo del programa: sin ficheros in ni out, con solo uno de ellos
*       o con ambos.
* Ret: int - Modo
*/
int read_args_afin(char* argv[], int argc, mpz_t m, mpz_t a, mpz_t b, char* ifile, char* ofile);

/*
* Function: void cifrar(char* texto, mpz_t a, mpz_t b, mpz_t m);
* Desc: Cifra el texto plano siguiendo el algoritmo afin: ax + b mod m,
*       y lo guarda en la variable texto.
*/
void cifrar(char* texto, mpz_t a, mpz_t b, mpz_t m);

/*
* Function: void descifrar(char* texto, mpz_t inv, mpz_t b, mpz_t m);
* Desc: Desifra el texto cifrado siguiendo el algoritmo afin: (x - b)*a⁻1 mod m,
*       y lo guarda en la variable texto.
*/
void descifrar(char* texto, mpz_t inv, mpz_t b, mpz_t m);
