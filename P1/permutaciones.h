/*
Desc: Libreria de un Doble Cifrado de Permutacion.
Authors: Daniel Mateo
         Laura Sanchez
File: permutaciones.h
*/
#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in
#define OFILE 4 //Modo con fichero out
#define IOFILES 5 //Modo con ficheros in y out

#define OK 1
#define ERROR 0


/*
* Function: int read_args_permutaciones(char* argv[], int argc, char* k1, char* k2, char* ifile, char* ofile)
* Desc: Lee los argumentos introducidos y comprueba que los datos sean correctos,
*       modificando los valores de k1, k2, ifile y ofile con ellos.
*       Devuelve el modo del programa: sin ficheros in ni out, con solo uno de ellos
*       o con ambos.
* Ret: int - Modo
*/
int read_args_permutaciones(char* argv[], int argc, char* k1, char* k2, char* ifile, char* ofile);

/*
* Function: int comprueba_claves_correctas(char *k1, char *k2)
* Desc: Comprueba que las claves de las permutaciones poseen numeros sin repeticiones y que ninguno
*       de esos numeros supera el tamanio m, para k1, o el tamanio n, para k2; ni sea menor que 1.
* Ret: int - OK o ERROR
*/
int comprueba_claves_correctas(char *k1, char *k2);

/*
* Function: char* limpia_texto(char* texto)
* Desc: Devuelve el texto limpio del texto original (solo con caracteres
*       que pertenezcan al Alfabeto).
* Ret: texto_limpio o NULL
*/
char* limpia_texto(char* texto);
