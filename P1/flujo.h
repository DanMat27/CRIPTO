/*
Desc: Libreria del Metodo Vigenere.
Authors: Daniel Mateo
         Laura Sanchez
File: flujo.h
*/

#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"

#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in
#define OFILE 4 //Modo con fichero out
#define IOFILES 5 //Modo con ficheros in y out

#define OK 1
#define ERROR 0

/*
* Function: int read_args_flujo(char* argv[], int argc, char* ifile, char* ofile)
* Desc: Lee los argumentos introducidos y comprueba que los datos sean correctos,
*       modificando los valores de k ifile y ofile con ellos.
*       Devuelve el modo del programa: sin ficheros in ni out, con solo uno de ellos
*       o con ambos.
* Ret: int - Modo
*/
int read_args_flujo(char* argv[], int argc, char* ifile, char* ofile);
/*
* Function: void cifrar(char* texto, char *k);
* Desc: Cifra el texto plano siguiendo el cifrado de flujo: (x1+k1,...,xn+kn) mod m,
*       y lo guarda en la variable texto.
*/
void cifrar(char* texto);

/*
* Function: void descifrar(char* texto, char *k);
* Desc: Descifra el texto cifrado siguiendo el descifrado de flujo: (x1-k1,...,xn-kn) mod m,
*       y lo guarda en la variable texto.
*/
void descifrar(char* texto);

/*
* Function: int flujo_claves_pseudoaleatorias(int x);
* Desc: Generador del flujo de numeros pseudoaleatorios mediante el método Afín.
*       Posee una semilla inicial y la x va cambiando con una periocidad.
*/
int flujo_claves_pseudoaleatorias(int x);
