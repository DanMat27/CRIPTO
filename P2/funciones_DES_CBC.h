/*
Desc: Libreria de desCBC.
Authors: Daniel Mateo
         Laura Sanchez
File: desCBC.h
*/
#include <stdio.h>
#include <stdlib.h>

#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in
#define OFILE 4 //Modo con fichero out
#define IOFILES 5 //Modo con ficheros in y out

#define OK 1
#define ERROR 0

#define BIT(x) (1<<(x)) //Devuelve el bit x puesto a 1 y resto de bits a 0.
#define BIT_GET(x,n) ((x) & BIT(n)) //Devuelve el bit n-esimo de x.
#define BIT_SET(x,n) ((x) |= BIT(n)) //Pone a 1 el bit n-esimo de x.
#define BIT_CLEAR(x,b) ((x) &= ~BIT(n)) //Pone a 0 el bit n-esimo de x.
#define BIT_TOGGLE(x,b) ((x) ^= BIT(n)) //Invierte el bit n-esimo de x.

/*
* Function: int read_args_des(char* argv[], int argc, char *clave, char *iv, char* ifile, char* ofile, int triple)
* Desc: Lee los argumentos introducidos y comprueba que los datos sean correctos,
*       modificando los valores de ifile y ofile con ellos.
*       Devuelve el modo del programa: sin ficheros in ni out, con solo uno de ellos
*       o con ambos.
* Ret: int - Modo
*/
int read_args_des(char* argv[], int argc, char *clave, char *iv, char* ifile, char* ofile, int triple);

/*
* Function: void print_binary_byte(char num)
* Desc: Imprime por pantalla el byte en binario.
* Ret: int - num
*/
void print_binary_byte(char num);

/*
* Function: void print_binary_bytes(char* num, int b)
* Desc: Imprime por pantalla un numero de b bytes en binario.
* Ret: int - num
*/
void print_binary_bytes(char* num, int nbytes);

/*
* Function: void ip(char *bloque, char *bloque_p, int inversa)
* Desc: Calcula la permutacion IP de un bloque, si se le indica inversa a 0.
*       Si se le indica inversa a 1, calcula IP-1.
* Ret: -
*/
void ip(char *bloque, char *bloque_p, int inversa);

/*
* Function: void copia_palabra_bytes(char* b1, char* b2, int nbytes)
* Desc: Copia los nbytes indicados de la cadena b1 en la cadena b2.
* Ret: -
*/
void copia_palabra_bytes(char* b1, char* b2, int nbytes);

/*
* Function: void pc1(char *clave, char *clave_p)
* Desc: Calcula el valor PC1 de la clave y lo deja en clave_p.
* Ret: -
*/
void pc1(char *clave, char *clave_p);

/*
* Function: void pc2(char *clave, char *clave_p)
* Desc: Calcula el valor PC2 de la subclave y lo deja en clave_p.
* Ret: -
*/
void pc2(char *clave, char *clave_p);

/*
* Function: void lcs(char *clave, int nronda)
* Desc: Realiza un desplazamiento LCS en la clave pasada.
* Ret: -
*/
void lcs(char *clave, int nronda);

/*
* Function: void f(char *ra, char *rn, char *clave)
* Desc: Calcula la funcion F entre R anterior (ra) y la subclave correspondiente.
*       Se almacena en Rn (rn).
* Ret: -
*/
void f(char *ra, char *rn, char *clave);

/*
* Function: void e(char *ra, char *ra_p)
* Desc: Calcula la expansion y permutacion de R anterior y lo almacena en ra_p.
* Ret: -
*/
void e(char *ra, char *ra_p);

/*
* Function: void p(char *sust, char *sust_p)
* Desc: Calcula la permutacion del resultado de las SBoxes y lo almacena en sust_p.
* Ret: -
*/
void p(char *sust, char *sust_p);

/*
* Function: void Sboxes(char *ra_p, char *sust)
* Desc: Calcula la permutacion del bloque en funcion de las SBoxes. Se almacena en sust.
* Ret: -
*/
void Sboxes(char *ra_p, char *sust);

/*
* Function: int controlb5(int ind);
* Desc: Devuelve siguiente posicion del bit 5 de un char en funcion de su ciclo.
*       ind es el valor actual.
* Ret: int - valor
*/
int controlb5(int ind);

/*
* Function: int controlb0(int ind);
* Desc: Devuelve siguiente posicion del bit 0 de un char en funcion de su ciclo.
*       ind es el valor actual
* Ret: int - valor
*/
int controlb0(int ind);

/*
* Function: int cambioByte(int ind, int *cont)
* Desc: Indica el valor del byte donde se encuentra la iteracion del LCS.
*       ind es el actual y cont lleva la cuenta de los bits mirados (0-7).
* Ret: int - valor
*/
int cambioByte(int ind, int *cont);

/*
* Function: int convertirDecimal(int *array, int tam)
* Desc: Calcula el valor decimal de un array de 1s y 0s de tamanio tam.
* Ret: int - valor
*/
int convertirDecimal(int *array, int tam);

/*
* Function: char** reserva_estructura_subclaves()
* Desc: Reserva memoria para la estructura que tendra las 16 subclaves.
* Ret: char** - estructura
*/
char** reserva_estructura_subclaves();

/*
* Function: void libera_estructura_subclaves(char** subclaves)
* Desc: Libera la memoria de la estructura con las 16 subclaves.
* Ret: -
*/
void libera_estructura_subclaves(char** subclaves);
