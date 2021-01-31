/*
Desc: Calcula la frecuncia de no linealidad de cada S-box del cifrado DES.
      Es decir, que no se cumple linealidad cuando no cumple que S(x+y) = S(x) + S(y).
      Probamos para cualquier combinacion de entradas de 000000 a 111111. (64*64)
      ./linealidad_des
Authors: Daniel Mateo
         Laura Sanchez
File: linealidad_des.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des_tables.h"
#include "funciones_DES_CBC.h"

/*
* Function: char calcula_sbox(char entrada, int box);
* Desc: Obtiene la salida de una S-box indicada de una entrada en concreto.
* Ret: char - S(entrada)
*/
char calcula_sbox(char entrada, int box);


int main(int argc, char const *argv[]) {
  char x, y, xi, yi, z, zi; // Valores con los 6 bits para las S-boxes, 0x0000 -- 0x003F
  int i, j, k;
  int cont=0;
  double frecuencias[8]; // Array con las frecuencias de no linealidad

  /* Bucle de S-boxes */
  for(i=0; i<NUM_S_BOXES; i++){
    frecuencias[i] = 0;
    x = 0x00;
    for(j=0; j<64; j++){
      y = 0x00;
      for(k=0; k<64; k++){
        z = x^y; // z = x + y
        zi = calcula_sbox(z,i); // zi = S(z)
        xi = calcula_sbox(x,i); // xi = S(x)
        yi = calcula_sbox(y,i); // yi = S(y)

        /* Comprobamos si se cumple S(x+y) = S(x) + S(y) */
        if(zi == (xi^yi)){
          printf("Se cumple la linealidad en la S-box %d para los valores x=%d e y=%d\n", i, x, y);
          frecuencias[i] += 1;
        }

        y++;
      }
      x++;
    }
  }

  printf("\n");
  /* Imprimimos por pantalla las probabilidades de no linealidad de cada caja */
  for(i=0; i<NUM_S_BOXES; i++){
    frecuencias[i] = 1 - (frecuencias[i]/(64*64));
    printf("La caja %d tiene frecuencia de no linealidad %lf\n", i, frecuencias[i]);
  }

  return OK;
}


char calcula_sbox(char entrada, int box){
  char salida;
  int f, c;
  int fila[2];
  int columna[4];

  /*print_binary_byte(entrada);*/

  /* Bit 5 */
  if(entrada & (1 << 0)) fila[0] = 1;
  else fila[0] = 0;

  /* Bit 4 */
  if(entrada & (1 << 5)) fila[1] = 1;
  else fila[1] = 0;

  /* Bit 3 */
  if(entrada & (1 << 1)) columna[0] = 1;
  else columna[0] = 0;

  /* Bit 2 */
  if(entrada & (1 << 2)) columna[1] = 1;
  else columna[1] = 0;

  /* Bit 1 */
  if(entrada & (1 << 3)) columna[2] = 1;
  else columna[2] = 0;

  /* Bit 0 */
  if(entrada & (1 << 4)) columna[3] = 1;
  else columna[3] = 0;

  f = convertirDecimal(fila, 2);
  c = convertirDecimal(columna, 4);

  /* Valor de salida de la S-Box */
  salida = (char)S_BOXES[box][f][c];

  return salida;
}
