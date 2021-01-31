/*
Desc: Libreria de SBOX_AES.c.
Authors: Daniel Mateo
         Laura Sanchez
File: SBOX_AES.h
*/
#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

/*
* Function: unsigned char **reserva_matriz()
* Desc: Reserva memoria para guardar la matriz directa o inversa.
* Ret: unsigned char ** - matriz reservada
*/
unsigned char **reserva_matriz();

/*
* Function: void liberar_matriz(unsigned char **matriz)
* Desc: Libera la memoria reservada para guardar la matriz directa o inversa.
* Ret: -
*/
void liberar_matriz(unsigned char **matriz);

/*
* Function: char calcular_inverso_multiplicativo(char polinomio)
* Desc: Calcula el inverso multiplicativo de un polinomio en GF(2⁸)/m(x),
*       utilizando el algoritmo de Euclides y el algoritmo extendido de
*       Euclides.
* Ret: char - inverso multiplicativo del polinomio
*/
char calcular_inverso_multiplicativo(char polinomio);

/*
* Function: char dividir_m_polinomio(char polinomio, char* coc)
* Desc: Divide m(x)= x⁸ + x⁴ + x³ + x + 1 entre un polinomio, devuelve el
*       cociente por argumento y devuelve el resto.
* Ret: char - resto de la division.
*/
char dividir_m_polinomio(char polinomio, char* coc);

/*
* Function: char dividir_polinomios(char p1, char p2, char *coc)
* Desc: Divide dos polinomio (p1/p2), devuelve el
*       cociente por argumento y devuelve el resto.
* Ret: char - resto de la division.
*/
char dividir_polinomios(char p1, char p2, char *coc);

/*
* Function: int calcula_grado(char p)
* Desc: Calcula el grado del un polinomio p.
* Ret: int - grado del polinomio.
*/
int calcula_grado(char p);

/*
* Function: char desplaza_izquierda(char num)
* Desc: Desplaza a la izquierda el polinomio 1 posicion.
* Ret: char - polinomio desplazado.
*/
char desplaza_izquierda(char num);

/*
* Function: char get_bit_n(char p, int n)
* Desc: Devuelve el bit de la posicion n del polinomio p.
* Ret: char - bit.
*/
char get_bit_n(char p, int n);

/*
* Function: char xtime(char p)
* Desc: Realiza el xtime de p.
* Ret: char - xtime(p).
*/
char xtime(char p);

/*
* Function: char producto_polinomios(char p1, char p2)
* Desc: Realiza el producto de dos polinomios .
* Ret: char - resultado del producto.
*/
char producto_polinomios(char p1, char p2);

/*
* Function: char transformacion_afin(char p)
* Desc: Realiza la transformacion afin del polinomio p.
* Ret: char - resultado de la transformacion.
*/
char transformacion_afin(char p);

/*
* Function: char transformacion_afin_inversa(char p)
* Desc: Realiza la transformacion afin inversa del polinomio p.
* Ret: char - resultado de la transformacion inversa.
*/
char transformacion_afin_inversa(char p);

/*
* Function: char desplaza_izquierda_circular(char p, int despl)
* Desc: Desplaza a la izquierda circularmente el polinomio despl posiciones.
* Ret: char - polinomio desplazado.
*/
char desplaza_izquierda_circular(char p, int despl);

/*
* Function: int fila(char p)
* Desc: Devuelve la fila de la matriz correspondiente al polinomio p.
* Ret: int - fila de la matriz.
*/
int fila(char p);

/*
* Function: int columna(char p)
* Desc: Devuelve la columna de la matriz correspondiente al polinomio p.
* Ret: int - columna de la matriz.
*/
int columna(char p);

/*
* Function: void imprime_matriz_fichero(unsigned char **matriz, char *ofile)
* Desc: Imprime la matriz directa o inversa.
* Ret: -
*/
void imprime_matriz_fichero(unsigned char **matriz, char *ofile);
