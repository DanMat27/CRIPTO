/*
Desc: Estudia el efecto avalancha en cada ronda del DES para un bloque y clave.
      Es decir, cambiando un bit del bloque plano o de la clave cuantos bits
      cambian en la salida.
      Generamos un bloque aleatorio y una clave aleatoria.
      ./avalancha_des {-B|-K}
      -B: modificamos un bit del bloque para estudiar el efecto avalancha
      -K: modificamos un bit de la clave para estudiar el efecto avalancha
Authors: Daniel Mateo
         Laura Sanchez
File: avalancha_des.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "des_tables.h"
#include "funciones_DES_CBC.h"

/*
* Function: char* generar_64_bits_aleatorio();
* Desc: Genera 64 bits aleatorios.
* Ret: char* - cadena de 8 caracteres
*/
char* generar_64_bits_aleatorio();

/*
* Function: char* modificar_bit(char* bloque);
* Desc: Modifica 1 de los 64 bits, de manera aleatoria.
* Ret: char* - bloque con un bit modificado
*/
char* modificar_bit(char* bloque);

/*
* Function: char** generar_subclaves(char* clave);
* Desc: Genera las 16 subclaves empleadas en las rondas del DES.
* Ret: char** - array con las 16 subclaves
*/
char** generar_subclaves(char* clave);

/*
* Function: void cifrado_DES_B(char *bloque, char *bloque_cambiado, char **subclaves);
* Desc: Realiza el cifrado DES paralelamente para bloque y para bloque_cambiado
*       estudiando a su vez el efecto avalancha.
*/
void cifrado_DES_B(char *bloque, char *bloque_cambiado, char **subclaves);

/*
* Function: int cuenta_bits_distintos(char* bloque1, char* bloque2);
* Desc: Cuenta los bits distintos entre bloque1 y bloque2.
* Ret: int - numero de bits diferentes
*/
int cuenta_bits_distintos(char* bloque1, char* bloque2);

/*
* Function: void cifrado_DES_K(char *bloque, char **subclaves, char **subclaves_mod);
* Desc: Realiza el cifrado DES paralelamente para bloque con subclaves
*       y para bloque con subclaves_mod estudiando a su vez el efecto avalancha.
*/
void cifrado_DES_K(char *bloque, char **subclaves, char **subclaves_mod);

int main(int argc, char const *argv[]) {
  char* bloque, *bloque_cambiado, *clave, *clave_cambiada;
  char **subclaves, **subclaves_mod; /* Estructuras para guardar las subclaves */
  int flag_b = 0;

  if(argc != 2){
    printf("Argumentos mal introducidos:\n");
    printf("./avalancha_des {-B|-K}\n");
    return ERROR;
  }

  /* Cambiamos semilla por tiempo */
  srand (time(NULL));

  /* Generamos un bloque (64 bits) y una clave aleatoria (64 bits) */
  bloque = generar_64_bits_aleatorio();
  clave = generar_64_bits_aleatorio();
  printf("Bloque generado: ");
  print_binary_bytes(bloque,8);
  printf("Clave generada: ");
  print_binary_bytes(clave,8);

  /* Si la opcion elegida es -B */
  if(strcmp("-B",argv[1]) == 0){
    flag_b = 1;
    printf("Has elegido la opcion de modificar un bit del bloque:\n");
    bloque_cambiado = modificar_bit(bloque);
    print_binary_bytes(bloque_cambiado,8);
  }
  else if(strcmp("-K",argv[1]) == 0){
    printf("Has elegido la opcion de modificar un bit de la clave:\n");
    clave_cambiada = modificar_bit(clave);
    print_binary_bytes(clave_cambiada,8);
  }
  else{
    printf("Error. Argumento incorrecto, debe introducir: -K o -B\n");
    return ERROR;
  }

  /* Generamos las subclaves */
  /* Para la clave sin modificar */
  subclaves = generar_subclaves(clave);

  /* Para la clave modificada si es opcion -K */
  if(flag_b == 0){
    subclaves_mod = generar_subclaves(clave_cambiada);
  }

  /* Si la opcion elegida es -B ciframos ambos bloques paralelamente
  comparando las salidas en cada ronda */
  if(flag_b){
    cifrado_DES_B(bloque, bloque_cambiado, subclaves);
  }
  /* Si la opcion elegida es -K ciframos el bloque con la clave normal y el bloque
  con la clave modificada paralelamente comparando las salidas en cada ronda */
  else{
    cifrado_DES_K(bloque, subclaves, subclaves_mod);
  }

  /* Liberamos memoria */
  free(bloque);
  free(clave);
  libera_estructura_subclaves(subclaves);

  if(flag_b) free(bloque_cambiado);
  else{
    free(clave_cambiada);
    libera_estructura_subclaves(subclaves_mod);
  }

  return OK;
}


char* generar_64_bits_aleatorio(){
  char *bloque;
  int i;

  bloque = (char*)calloc(8+1, sizeof(char));
  if(!bloque) return NULL;

  /* Cada byte del bloque es aleatorio de 0 a 256 */
  for(i=0; i<8; i++){
    bloque[i] = rand() % 256;
  }
  bloque[8] = '\0';

  return bloque;
}

char* modificar_bit(char *bloque){
  char* bloque_cambiado;
  int byte, bit;

  bloque_cambiado = (char*)calloc(8+1, sizeof(char));
  if(!bloque) return NULL;

  copia_palabra_bytes(bloque, bloque_cambiado, 8);

  byte = rand() % 8;
  bit = rand() % 8;

  /* Complementamos un bit aleatoriamente */
  bloque_cambiado[byte] ^= (1 << bit); //BIT_TOGGLE(x,b)

  return bloque_cambiado;
}

char** generar_subclaves(char* clave){
  char** subclaves;
  char clave_p1[8];
  int k;

  /* Reservamos memoria para la estructura de subclaves */
  subclaves = reserva_estructura_subclaves();
  if(!subclaves) return ERROR;

  /* Permutamos la clave inicial quitando los bits de paridad 64 bits --> 56 bits */
  pc1(clave, clave_p1);

  /* Generamos las 16 subclaves mediante la K inicial */
  for(k=0; k < ROUNDS; k++){
    lcs(clave_p1, k);
    copia_palabra_bytes(clave_p1, subclaves[k], 7);
  }

  return subclaves;
}

void cifrado_DES_B(char *bloque, char *bloque_cambiado, char **subclaves){
  char bloque_p[8], bloque_p_mod[8];
  char subclave_p2[8], la[4], ra[4], la_mod[4], ra_mod[4], ln[4], rn[4], ln_mod[4], rn_mod[4];
  char frk[4], frk_mod[4];
  int ronda, num_bits, k;

  /* Aplicamos la permutacon IP inicial a ambos bloques */
  ip(bloque, bloque_p, 0);
  copia_palabra_bytes(bloque_p, bloque, 8);

  ip(bloque_cambiado, bloque_p_mod, 0);
  copia_palabra_bytes(bloque_p_mod, bloque_cambiado, 8);


  /* Bucle de 16 rondas DES aplicadas sobre los bloques correspondientes */
  for(ronda=0; ronda<16; ronda++){

    /* Permutamos la clave de la ronda n y comprimimos 56 bits --> 48 bits */
    pc2(subclaves[ronda], subclave_p2);

    /* Obtenemos ambas mitades del bloque normal */
    for(k=0; k<4; k++){
      la[k] = bloque[k];
      ra[k] = bloque[k+4];
    }

    /* Obtenemos ambas mitades del bloque modificado */
    for(k=0; k<4; k++){
      la_mod[k] = bloque_cambiado[k];
      ra_mod[k] = bloque_cambiado[k+4];
    }

    /* Ln = Rn-1 */
    copia_palabra_bytes(ra, ln, 4);
    copia_palabra_bytes(ra_mod, ln_mod, 4);

    /* Ejecutamos f para Rn-1 */
    f(ra, frk, subclave_p2);
    f(ra_mod, frk_mod, subclave_p2);

    /* Hacemos Ln-1 + f(Rn-1,Kn) */
    for(k=0; k<4; k++){
      rn[k] = la[k] ^ frk[k];
    }
    for(k=0; k<4; k++){
      rn_mod[k] = la_mod[k] ^ frk_mod[k];
    }

    /* Obtenemos ambas mitades del bloque normal */
    for(k=0; k<4; k++){
      bloque[k] = ln[k];
      bloque[k+4] = rn[k];
    }
    /* Obtenemos ambas mitades del bloque modificado */
    for(k=0; k<4; k++){
      bloque_cambiado[k] = ln_mod[k];
      bloque_cambiado[k+4] = rn_mod[k];
    }

    /* Comparamos los resultados de la ronda de cada bloque */
    printf("\nRonda %d\n",ronda + 1);
    printf("Bloque normal: ");
    print_binary_bytes(bloque, 8);
    printf("Bloque modificado: ");
    print_binary_bytes(bloque_cambiado, 8);
    num_bits = cuenta_bits_distintos(bloque, bloque_cambiado);
    printf("Se diferencian en %d bits\n", num_bits);

  }

}

void cifrado_DES_K(char *bloque, char **subclaves, char **subclaves_mod){
  char bloque_p[8], bloque_cambiado[8];
  char subclave_p2[8], subclave_p2_mod[8], la[4], ra[4], la_mod[4], ra_mod[4], ln[4], rn[4], ln_mod[4], rn_mod[4];
  char frk[4], frk_mod[4];
  int ronda, num_bits, k;

  /* Aplicamos la permutacon IP inicial a ambos bloques */
  ip(bloque, bloque_p, 0);
  copia_palabra_bytes(bloque_p, bloque, 8);

  copia_palabra_bytes(bloque, bloque_cambiado, 8);

  /* Bucle de 16 rondas DES aplicadas sobre los bloques correspondientes */
  for(ronda=0; ronda<16; ronda++){

    /* Permutamos la clave normal de la ronda n y comprimimos 56 bits --> 48 bits */
    pc2(subclaves[ronda], subclave_p2);
    /* Permutamos la clave modificada de la ronda n y comprimimos 56 bits --> 48 bits */
    pc2(subclaves_mod[ronda], subclave_p2_mod);

    /* Obtenemos ambas mitades del bloque */
    for(k=0; k<4; k++){
      la[k] = bloque[k];
      ra[k] = bloque[k+4];
    }

    /* Obtenemos ambas mitades del bloque con la clave modificada */
    for(k=0; k<4; k++){
      la_mod[k] = bloque_cambiado[k];
      ra_mod[k] = bloque_cambiado[k+4];
    }

    /* Ln = Rn-1 */
    copia_palabra_bytes(ra, ln, 4);
    copia_palabra_bytes(ra_mod, ln_mod, 4);

    /* Ejecutamos f para Rn-1 */
    f(ra, frk, subclave_p2);
    f(ra_mod, frk_mod, subclave_p2_mod);

    /* Hacemos Ln-1 + f(Rn-1,Kn) */
    for(k=0; k<4; k++){
      rn[k] = la[k] ^ frk[k];
    }
    for(k=0; k<4; k++){
      rn_mod[k] = la_mod[k] ^ frk_mod[k];
    }

    /* Obtenemos ambas mitades del bloque normal */
    for(k=0; k<4; k++){
      bloque[k] = ln[k];
      bloque[k+4] = rn[k];
    }
    /* Obtenemos ambas mitades del bloque modificado */
    for(k=0; k<4; k++){
      bloque_cambiado[k] = ln_mod[k];
      bloque_cambiado[k+4] = rn_mod[k];
    }

    /* Comparamos los resultados de la ronda de cada bloque */
    printf("\nRonda %d\n",ronda + 1);
    printf("Bloque normal: ");
    print_binary_bytes(bloque, 8);
    printf("Bloque modificado: ");
    print_binary_bytes(bloque_cambiado, 8);
    num_bits = cuenta_bits_distintos(bloque, bloque_cambiado);
    printf("Se diferencian en %d bits\n", num_bits);

  }

}

int cuenta_bits_distintos(char* bloque1, char* bloque2){
  int bits=0, i, j;
  char bloque_aux[8];

  for(i=0; i<8; i++){
    bloque_aux[i] = bloque1[i] ^ bloque2[i];
    for(j=0; j<8; j++){
      if(bloque_aux[i] & (1 << j)) bits++;
    }
  }

  return bits;
}
