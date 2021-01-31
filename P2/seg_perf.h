/*
Desc: Libreria de seg_perf.
Authors: Daniel Mateo
         Laura Sanchez
File: seg_perf.h
*/
#include <stdio.h>
#include <stdlib.h>

#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in
#define OFILE 4 //Modo con fichero out
#define IOFILES 5 //Modo con ficheros in y out

#define OK 1
#define ERROR 0

/*
* Function: int read_args_seg_perf(char* argv[], int argc, char* ifile, char* ofile)
* Desc: Lee los argumentos introducidos y comprueba que los datos sean correctos,
*       modificando los valores de ifile y ofile con ellos.
*       Devuelve el modo del programa: sin ficheros in ni out, con solo uno de ellos
*       o con ambos.
* Ret: int - Modo
*/
int read_args_seg_perf(char* argv[], int argc, char* ifile, char* ofile);

/*
* Function: void cifrar_equiprobable(char* texto, double *p_k);
* Desc: Cifra el texto plano siguiendo el cifrado de desplazamiento y claves equiprobables,
*       y lo guarda en la variable texto. Ademas calcula las p_k.
*/
void cifrar_equiprobable(char* texto, double *p_k);

/*
* Function: void cifrar_no_equiprobable(char* texto,double *p_k);
* Desc: Cifra el texto plano siguiendo el cifrado de desplazamiento y claves no equiprobables,
*       y lo guarda en la variable texto. Ademas calcula las p_k.
*/
void cifrar_no_equiprobable(char* texto, double *p_k);

/*
* Function: void calcula_p_caracter_plano_y_cifrado(char *texto_c, char *texto_p, double *p_cy, double *p_px);
* Desc: Cifra el texto plano siguiendo el cifrado de desplazamiento y claves no equiprobables,
*       y lo guarda en la variable texto. Ademas calcula las p_k. HACEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEER
*/
void calcula_p_caracter_plano_y_cifrado(char *texto_c, char *texto_p, double *p_cy, double *p_px);

/*
* Function: void calcula_p_caracter_cifrado_condicionado(char* texto_c, char* texto_p, double* p_k, double* p_cyx);
* Desc: Cifra el texto plano siguiendo el cifrado de desplazamiento y claves no equiprobables,
*       y lo guarda en la variable texto. Ademas calcula las p_k. HACEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEER
*/
void calcula_p_caracter_cifrado_condicionado(char* texto_c, char* texto_p, double* p_k, double* p_cyx);
