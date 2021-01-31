/*
Desc: Libreria del Indice de Coincidencia.
Authors: Daniel Mateo
         Laura Sanchez
File: indice_c.H
*/

#include <stdio.h>
#include <stdlib.h>

#define ERROR -1
#define OK 1
#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in
#define OFILE 4 //Modo con fichero out
#define IOFILES 5 //Modo con ficheros in y out
#define ICINGLES 0.065 //Indice de coincidencia del lenguaje ingles
#define ICRANDOM 0.038 //Indice de coincidencia de un lenguaje aletorio
#define TAMTEXTO 496 //Tamanio maximo cadena de texto
#define FILENAME 64 //Tamanio maximo cadena de nombre de un fichero
#define NTRY 20 //Numero maximo de la clave n con el que se prueba

/*
* Function: int read_args_afin(char* argv[], int argc, int* l, char* ifile, char* ofile)
* Desc: Lee los argumentos introducidos y comprueba que los datos sean correctos,
*       modificando los valores de l, ifile y ofile con ellos.
*       Devuelve el modo del programa: sin ficheros in ni out, con solo uno de ellos
*       o con ambos.
* Ret: int - Modo
*/
int read_args_afin(char* argv[], int argc, int* l, char* ifile, char* ofile);

/*
* Function: char** divide_texto_bloques_n(int n, int ngrama, char* texto)
* Desc: Divide el texto en particiones de NGramas en funcion del tamanio n. Luego, asigna
*       a cada bloque los ngramas correspondientes según sus posiciones.
* Ret: Estructura de bloques
*/
char** divide_texto_bloques_n(int n, int ngrama, char* texto);

/*
* Function: char* limpia_texto(char* texto)
* Desc: Devuelve el texto limpio del texto original (solo con caracteres 
*       que pertenezcan al Alfabeto).
* Ret: texto_limpio o NULL
*/
char* limpia_texto(char* texto);

/* 
* Function: char** reserva_estructura_bloques(int tam, int n)
* Desc: Reserva memoria y devuelve una estructura array
*       de cadenas (bloques).
* Ret: bls o NULL
*/
char** reserva_estructura_bloques(int tam, int n);

/* 
* Function: void libera_estructura_bloques(char** bls, int tam)
* Desc: Libera la memoria de la estructura bls.
* Ret: -
*/
void libera_estructura_bloques(char** bls, int tam);

/* 
* Function: void asigna_ngramas_a_bloques(char** bloques, char* texto, int npts, int ngrama, int n)
* Desc: Asigna a cada bloque los ngramas en posiciones multiplos de n.
* Ret: -
*/
void asigna_ngramas_a_bloques(char** bloques, char* texto, int npts, int ngrama, int n);

/* 
* Function: int calcula_ic(char* bloque, int ngrama)
* Desc: Realiza el calculo del Indice de Coincidencia del bloque pasado.
* Ret: double - valor del ic
*/
double calcula_ic(char* bloque, int ngrama);