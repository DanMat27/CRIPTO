/*
Desc: Cifra con el algoritmo triple DES con 3 claves y el modo de operacion CBC
      ./TDEA_CBC {-C | -D} {-k clave -iv vectorinicializacion} {-i filein -o fileout}
      -C proceso de cifrado
      -D proceso de descifrado
      -k claves (cadena de 8 caracteres = 64 bits)
      -iv vector de inicializacion (cadena de 8 caracteres = 64 bits)
      -i fichero de entrada
      -o fichero de salida
Authors: Daniel Mateo
         Laura Sanchez
File: TDEA_CBC.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des_tables.h"
#include "funciones_DES_CBC.h"

int main(int argc, char *argv[]) {
  char ifile[50], ofile[50]; // Paths de los ficheros de entrada y salida
  FILE *i, *o; // Ficheros de entrada y salida
  char clave_total[25], iv[8]; // Contienen la clave y el iv de entrada
  char clave1[9], clave2[9], clave3[9]; // Contienen las 3 claves de la total de 192 bits
  char bloque_p[9], bloque[8], bloque_aux[9], bloque_auxdesc[9], bloque_cn1[9]; // Bloques
  char** subclaves1 = NULL; // Estructura con las 16 subclaves de la clave 1
  char** subclaves2 = NULL; // Estructura con las 16 subclaves de la clave 2
  char** subclaves3 = NULL; // Estructura con las 16 subclaves de la clave 3
  char clave_p1[7]; // Contiene la clave 1 permutada en PC1
  char clave_p2[7]; // Contiene la clave 2 permutada en PC1
  char clave_p3[7]; // Contiene la clave 3 permutada en PC1
  char subclave_p2[6]; // Contiene la subclave permutada en PC2
  char la[4], ra[4], ln[4], rn[4]; // Contienen mitades del bloque (anterior y n)
  char frk[4]; // Salida de funcion F de la ronda
  int modo, k, r, flagc, j, ronda, flagfinal, cont = 0;
  char c; // Caracter leido
  double tamf;

  /* Comprobamos numero de argumentos (como minimo 6) */
  if(argc < 6){
    printf("Error en los argumentos. Introduce:\n");
    printf("./TDEA_CBC {-C | -D} {-k clave -iv vectorinicializacion} {-i filein -o fileout}\n");
    return ERROR;
  }

  /* Leemos los argumentos pasados por terminal */
  /* modo es la opcion elegida: TECLADO, IFILE, OFILE, IOFILES */
  modo = read_args_des(argv, argc, clave_total, iv, ifile, ofile, 1);
  if(modo != IOFILES){
    printf("Este programa solo soporta introduccion de ficheros de entrada y de salida obligatoriamente.\n");
    return ERROR;
  }

  /* Opcion del algoritmo */
  if(strcmp(argv[1],"-C") == 0){
    printf("Ha elegido la opcion de cifrado...\n");
    flagc = 1;
  }else if (strcmp(argv[1],"-D") == 0){
    printf("Ha elegido la opcion de descifrado...\n");
    flagc = 0;
  }else{
    printf("Debe elegir la opcion de cifrado {-C} o de descifrado {-D}\n");
    return ERROR;
  }

  i =fopen(ifile,"r");
  if(i == NULL){
    printf("Error al abrir el fichero de entrada.\n");
    return ERROR;
  }
  fseek(i, 0L, SEEK_END);
  tamf = ftell(i)/8;
  fclose(i);

  /* Fichero de entrada */
  i =fopen(ifile,"r");
  if(i == NULL){
    printf("Error al abrir el fichero de entrada.\n");
    return ERROR;
  }

  /* Fichero de salida */
  o = fopen(ofile, "w");
  if(!o){
    printf("Error al abrir el fichero de salida.\n");
    return ERROR;
  }

  /* Dividimos la clave en 3, cada una tiene 64 bits */
  for(k=0; k<8; k++){
    clave1[k] = clave_total[k];
    clave2[k] = clave_total[k+8];
    clave3[k] = clave_total[k+8+8];
  }

  printf("Clave 1:\n");
  print_binary_bytes(clave1, 8);
  printf("Clave 2:\n");
  print_binary_bytes(clave2, 8);
  printf("Clave 3:\n");
  print_binary_bytes(clave3, 8);
  printf("Vector de inicializacion:\n");
  print_binary_bytes(iv, 8);
  printf("\n");

  /* Reservamos memoria para la estructura de subclaves */
  subclaves1 = reserva_estructura_subclaves();
  if(!subclaves1) return ERROR;

  subclaves2 = reserva_estructura_subclaves();
  if(!subclaves2) return ERROR;

  subclaves3 = reserva_estructura_subclaves();
  if(!subclaves3) return ERROR;

  /* Generamos las subclaves para las 3 claves */

  /* Permutamos la clave inicial quitando los bits de paridad 64 bits --> 56 bits */
  pc1(clave1, clave_p1);
  pc1(clave2, clave_p2);
  pc1(clave3, clave_p3);

  /* Generamos las 16 subclaves mediante las Ks iniciales */
  for(k=0; k < ROUNDS; k++){
    lcs(clave_p1, k);
    copia_palabra_bytes(clave_p1, subclaves1[k], 7);
  }

  for(k=0; k < ROUNDS; k++){
    lcs(clave_p2, k);
    copia_palabra_bytes(clave_p2, subclaves2[k], 7);
  }

  for(k=0; k < ROUNDS; k++){
    lcs(clave_p3, k);
    copia_palabra_bytes(clave_p3, subclaves3[k], 7);
  }

  /*[BLOQUE]+IV    [BLOQUE]+[BLOQUEC]
  DES_k1
  DES-1_k2
  DES_k3
  [BLOQUEC]*/
  /* Empezamos a cifrar el texto bloque a bloque */
  cont = 1;
  while(flagfinal != 1) {

    j = 0;
    for(k=0; k < 8; k++){
      c = fgetc(i);
      /*printf("caracter %c\n", c);*/
      if(c == EOF) {
        if(feof(i)){
          /*printf("Final de fichero 1\n");*/
          flagfinal = 1;
          break;
        }
      }

      bloque[k] = c;
      bloque_cn1[k] = c;
      j++;
      bloque_p[k] = '\0';
    }

    /* Si bloque final, rellenamos si no es multiplo de 8 bytes */
    if(flagfinal == 1){
      /*printf("Entro bloque final %d\n", cont);*/
      if(j != 8){
        for(k=j; k < 8; k++){
          bloque[k] = 0x00;
          bloque_cn1[k] = 0x00;
          bloque_p[k] = '\0';
        }
      }

    }

    if (flagc == 1){
      if(cont > 1){
        /* CBC --> Hacemos Pn XOR Cn-1 */
        for(k=0; k<8; k++){
          bloque[k] = bloque[k] ^ bloque_aux[k];
        }
      }
      /* Si se trata del primer bloque hacemos Pn XOR IV */
      else {
        for(k=0; k<8; k++){
          bloque[k] = bloque[k] ^ iv[k];
        }
      }
    }

    /* Cn = DES_k3(DES-1_k2(DES_k1(Pn))) */
    for(r=0; r < 3; r++){

      /* Aplicamos la permutacon IP inicial al bloque */
      ip(bloque, bloque_p, 0);
      copia_palabra_bytes(bloque_p, bloque, 8);

      /* Bucle de 16 rondas DES aplicadas sobre el bloque correspondiente */
      for(ronda=0; ronda<16; ronda++){

        /* Permutamos la clave de la ronda n y comprimimos 56 bits --> 48 bits */
        /* Opcion Cifrado. Claves en orden. */
        if(flagc){
          if(r == 0) pc2(subclaves1[ronda], subclave_p2);
          else if(r == 1) pc2(subclaves2[15 - ronda], subclave_p2);
          else if(r == 2) pc2(subclaves3[ronda], subclave_p2);
        }
        /* Opcion Descifrado. Claves en orden inverso. */
        else {
          if(r == 0) pc2(subclaves3[15 - ronda], subclave_p2);
          else if(r == 1) pc2(subclaves2[ronda], subclave_p2);
          else if(r == 2) pc2(subclaves1[15 - ronda], subclave_p2);
        }

        /* Obtenemos ambas mitades del bloque */
        for(k=0; k<4; k++){
          la[k] = bloque[k];
          ra[k] = bloque[k+4];
        }

        /* Ln = Rn-1 */
        copia_palabra_bytes(ra, ln, 4);

        /* Ejecutamos f para Rn-1 */
        f(ra, frk, subclave_p2);

        /* Hacemos Ln-1 + f(Rn-1,Kn) */
        for(k=0; k<4; k++){
          rn[k] = la[k] ^ frk[k];
        }

        /* Obtenemos ambas mitades del bloque */
        for(k=0; k<4; k++){
          bloque[k] = ln[k];
          bloque[k+4] = rn[k];
        }

      }

      /* Realizamos el SWAP FINAL del DES*/
      for(k=0; k<4; k++){
        bloque_aux[k] = bloque[k];
      }
      for(k=0; k<4; k++){
        bloque[k] = bloque[k+4];
      }
      for(k=0; k<4; k++){
        bloque[k+4] = bloque_aux[k];
      }

      /* Aplicamos la permutacon IP inversa final al bloque cifrado */
      ip(bloque, bloque_p, 1);
      copia_palabra_bytes(bloque_p, bloque, 8);

    }

    /* En el caso de estar descifrando DESTRIPLE-1(Cn) XOR Cn-1 */
    if (flagc == 0){
      if(cont > 1){
        /* CBC --> Hacemos Pn XOR Cn-1 */
        /*print_binary_bytes(bloque, 8);*/
        for(k=0; k<8; k++){
          bloque[k] = bloque[k] ^ bloque_auxdesc[k];
        }
        /*print_binary_bytes(bloque_auxdesc, 8);
        print_binary_bytes(bloque, 8);*/
      }
      /* Si se trata del primer bloque hacemos DES-1(Cn) XOR IV */
      else {
        for(k=0; k<8; k++){
          bloque[k] = bloque[k] ^ iv[k];
        }
      }
    }

    /* Guardamos el bloque cifrado Cn para aplicar el CBC en el caso del cifrado*/
    if(flagc) copia_palabra_bytes(bloque, bloque_aux, 8);
    if(!flagc) copia_palabra_bytes(bloque_cn1, bloque_auxdesc, 8);

    /* Escribimos bloque cifrado o descifrado en fichero de salida */
    for(k=0; k<8; k++){
      if(flagc && cont == tamf+1){
        if(bloque[k] != 0x00){
          fprintf(o, "%c", bloque[k]);
        }
      }
      else if(!flagc && cont == tamf){
        if(bloque[k] != 0x00){
          fprintf(o, "%c", bloque[k]);
        }
      }
      else{
        fprintf(o, "%c", bloque[k]);
      }
    }

    if(!flagc && cont == tamf) break;

    cont++;

  }

  /* Liberamos memoria */
  libera_estructura_subclaves(subclaves1);
  libera_estructura_subclaves(subclaves2);
  libera_estructura_subclaves(subclaves3);

  fclose(i);
  fclose(o);

  printf("\nMemoria liberada correctamente. Cerrando programa...\n");

  return OK;
}
