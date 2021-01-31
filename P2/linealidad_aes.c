/*
Desc: Calcula la frecuncia de no linealidad de la S-box del cifrado AES.
      Es decir, que no se cumple linealidad cuando no cumple que S(x+y) = S(x) + S(y).
      Probamos para cualquier combinacion de entradas de 00000000 a 11111111. (256*256)
      ./linealidad_aes
Authors: Daniel Mateo
         Laura Sanchez
File: linealidad_aes.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes_tables.h"
#include "funciones_DES_CBC.h"

/*
* Function: void calcula_sbox(char entrada, char* salida_dir, char* salida_inv);
* Desc: Obtiene la salida de la S-box del AES en funcion de la entrada.
* Ret: char - S(entrada)
*/
void calcula_sbox(char entrada, char* salida_dir, char* salida_inv);

int main(int argc, char const *argv[]) {
  unsigned char x, y, xi, yi, z, zi, xi_inv, yi_inv, zi_inv; // Valores con los 8 bits para la S-box, 0x00 -- 0xFF
  int j, k;
  double frecuencias[2]; // Frecuencias de no linealidad de las cajas del AES

  frecuencias[0] = 0;
  frecuencias[1] = 0;
  x = 0x00;
  for(j=0; j<256; j++){
    y = 0x00;
    for(k=0; k<256; k++){
      z = x^y; // z = x + y
      calcula_sbox(z, &zi, &zi_inv); // zi = S(z) zi_inv = S_-1(z)
      calcula_sbox(x, &xi, &xi_inv); // xi = S(x) xi_inv = S_-1(x)
      calcula_sbox(y, &yi, &yi_inv); // yi = S(y) yi_inv = S_-1(y)

      /* Comprobamos si se cumple S(x+y) = S(x) + S(y) */
      if(zi == (xi^yi)){
        printf("Se cumple la linealidad en la S-box directa del AES para los valores x=%d e y=%d\n", x, y);
        frecuencias[0] += 1;
      }
      if(zi_inv == (xi_inv^yi_inv)){
        printf("Se cumple la linealidad en la S-box inversa del AES para los valores x=%d e y=%d\n", x, y);
        frecuencias[1] += 1;
      }

      y++;
    }
    x++;
  }

  printf("\n");
  /* Imprimimos por pantalla la probabilidad de no linealidad de la caja */
  frecuencias[0] = 1 - (frecuencias[0]/(256*256));
  frecuencias[1] = 1 - (frecuencias[1]/(256*256));
  printf("La S-box directa del AES tiene frecuencia de no linealidad %lf\n", frecuencias[0]);
  printf("La S-box inversa del AES tiene frecuencia de no linealidad %lf\n", frecuencias[1]);

  return OK;
}


void calcula_sbox(char entrada, char* salida_dir, char* salida_inv){
  char salida;
  int f, c;
  int fila[4];
  int columna[4];

  /*print_binary_byte(entrada);*/

  /* Bit 7 */
  if(entrada & (1 << 0)) columna[0] = 1;
  else columna[0] = 0;

  /* Bit 6 */
  if(entrada & (1 << 1)) columna[1] = 1;
  else columna[1] = 0;

  /* Bit 5 */
  if(entrada & (1 << 2)) columna[2] = 1;
  else columna[2] = 0;

  /* Bit 4 */
  if(entrada & (1 << 3)) columna[3] = 1;
  else columna[3] = 0;

  /* Bit 3 */
  if(entrada & (1 << 4)) fila[0] = 1;
  else fila[0] = 0;

  /* Bit 2 */
  if(entrada & (1 << 5)) fila[1] = 1;
  else fila[1] = 0;

  /* Bit 1 */
  if(entrada & (1 << 6)) fila[2] = 1;
  else fila[2] = 0;

  /* Bit 0 */
  if(entrada & (1 << 7)) fila[3] = 1;
  else fila[3] = 0;

  f = convertirDecimal(fila, 4);
  c = convertirDecimal(columna, 4);

  /* Valor de salida de la S-Box */
  *salida_dir = DIRECT_SBOX[f][c];
  *salida_inv = INVERSE_SBOX[f][c];

}
