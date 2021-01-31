/*
Desc: Libreria del Test de Kasiski.
Authors: Daniel Mateo
         Laura Sanchez
File: kaiski.h
*/

#include <stdio.h>
#include <stdlib.h>

#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in

#define OK 1
#define ERROR 0
#define NTRY 20 //Probamos para valor de claves 0 - 20

/*
* Function: int read_args_kasiski(char* argv[], int argc, int *l, char* ifile)
* Desc: Lee los argumentos introducidos y comprueba que los datos sean correctos,
*       modificando los valores de l y ifile con ellos.
*       Devuelve el modo del programa: sin ficheros in ni out, con solo uno de ellos
*       o con ambos.
* Ret: int - Modo
*/
int read_args_kasiski(char* argv[], int argc, int *l, char* ifile);


/*
* Function: void obtener_longitud_clave(char* texto, int longitud_repeticion)
* Desc: Obtiene las posibles longitudes de la clave con la que fue cifrado el texto
*       con el método Test de Kasiski.
*/
void obtener_longitud_clave(char* texto, int longitud_repeticion);


/*
* Function: char* limpia_texto(char* texto)
* Desc: Devuelve el texto limpio del texto original (solo con caracteres
*       que pertenezcan al Alfabeto).
* Ret: texto_limpio o NULL
*/
char* limpia_texto(char* texto);
