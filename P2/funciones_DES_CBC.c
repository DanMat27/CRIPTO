#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "des_tables.h"
#include "funciones_DES_CBC.h"

void ip(char *bloque, char *bloque_p, int inversa){
  int i,j, cont = 0, posicion, per;

  for(i=0; i<8; i++) {
      for(j=0; j<8; j++) {
        if(inversa) per = IP_INV[cont] - 1;
        else per = IP[cont] - 1;
        posicion = (per - (per%8))/8;
        /*printf("posicion = %d\n",posicion);*/
        if (((bloque[posicion]) & (1 << 7-per%8))){//BIT_GET(num,i)
            bloque_p[i] |= (1 << 7-j); //BIT_SET(num,j)
            /* print_binary_bytes(bloque_p,8); */
        } else {
          bloque_p[i] &= ~(1 << 7-j); //BIT_CLEAR(num,j)
          /* print_binary_bytes(bloque_p,8); */
        }
        cont ++;
      }
  }

}

void pc1(char *clave, char *clave_p){
  int i,j, cont = 0, posicion, per;

  for(i=0; i<7; i++) {
      for(j=0; j<8; j++) {
        per = PC1[cont] - 1;
        posicion = (per - (per%8))/8;
        /*printf("posicion = %d\n",posicion);*/
        if (((clave[posicion]) & (1 << 7-per%8))){//BIT_GET(num,i)
            clave_p[i] |= (1 << 7-j); //BIT_SET(num,j)
            /* print_binary_bytes(bloque_p,8); */
        } else {
          clave_p[i] &= ~(1 << 7-j); //BIT_CLEAR(num,j)
          /* print_binary_bytes(bloque_p,8); */
        }
        cont ++;
      }
  }
}

void pc2(char *clave, char *clave_p){
  int i,j, cont = 0, posicion, per;

  for(i=0; i<6; i++) {
      for(j=0; j<8; j++) {
        per = PC2[cont] - 1;
        posicion = (per - (per%8))/8;
        /*printf("posicion = %d\n",posicion);*/
        if (((clave[posicion]) & (1 << 7-per%8))){//BIT_GET(num,i)
            clave_p[i] |= (1 << 7-j); //BIT_SET(num,j)
            /* print_binary_bytes(bloque_p,8); */
        } else {
          clave_p[i] &= ~(1 << 7-j); //BIT_CLEAR(num,j)
          /* print_binary_bytes(bloque_p,8); */
        }
        cont ++;
      }
  }
}

void lcs(char *clave, int nronda){
  int nbits = ROUND_SHIFTS[nronda];
  int bitcl1, bitcl2, bitdl1, bitdl2, i;
  char aux;

  /* Guardamos los bits de la posicion 28 y 27 de Cn y Dn */
  if(clave[0] & (1 << 7)) bitcl1 = 1;
  else bitcl1 = 0;
  if(clave[0] & (1 << 6)) bitcl2 = 1;
  else bitcl2 = 0;
  if(clave[3] & (1 << 3)) bitdl1 = 1;
  else bitdl1 = 0;
  if(clave[3] & (1 << 2)) bitdl2 = 1;
  else bitdl2 = 0;

  aux = clave[4];

  /* Aplicamos desplazamiento a la izquierda a la mitad C (Izquierda) */
  for(i=0; i<6; i++){
    if(i != 3){
      clave[i] = clave[i] << nbits;
      if(nbits == 1){
        if(clave[i+1] & (1 << 7)) clave[i] |= (1 << 0); //BIT_SET(num,j)
        else clave[i] &= ~(1 << 0); //BIT_CLEAR(num,j)
      }
      else{
        if(clave[i+1] & (1 << 7)) clave[i] |= (1 << 1); //BIT_SET(num,j)
        else clave[i] &= ~(1 << 1); //BIT_CLEAR(num,j)
        if(clave[i+1] & (1 << 6)) clave[i] |= (1 << 0); //BIT_SET(num,j)
        else clave[i] &= ~(1 << 0); //BIT_CLEAR(num,j)
      }
    }
  }

  /* Caracter del medio i = 3 */
  clave[3] = clave[3] << nbits;
  if(nbits == 1){
    if(bitcl1) clave[3] |= (1 << 4); //BIT_SET(num,j)
    else clave[3] &= ~(1 << 4); //BIT_CLEAR(num,j)
    if(aux & (1 << 7)) clave[3] |= (1 << 0); //BIT_SET(num,j)
    else clave[3] &= ~(1 << 0); //BIT_CLEAR(num,j)
  }
  else{
    if(bitcl1) clave[3] |= (1 << 5); //BIT_SET(num,j)
    else clave[3] &= ~(1 << 5); //BIT_CLEAR(num,j)
    if(bitcl2) clave[3] |= (1 << 4); //BIT_SET(num,j)
    else clave[3] &= ~(1 << 4); //BIT_CLEAR(num,j)
    if(aux & (1 << 7)) clave[3] |= (1 << 1); //BIT_SET(num,j)
    else clave[3] &= ~(1 << 1); //BIT_CLEAR(num,j)
    if(aux & (1 << 6)) clave[3] |= (1 << 0); //BIT_SET(num,j)
    else clave[3] &= ~(1 << 0); //BIT_CLEAR(num,j)
  }

  /* Modificamos los bits finales con los almacenados i = 7 */
  clave[6] = clave[6] << nbits;
  if(nbits == 1){
    if(bitdl1) clave[6] |= (1 << 0); //BIT_SET(num,j)
    else clave[6] &= ~(1 << 0); //BIT_CLEAR(num,j)
  }
  else{
    if(bitdl1) clave[6] |= (1 << 1); //BIT_SET(num,j)
    else clave[6] &= ~(1 << 1); //BIT_CLEAR(num,j)
    if(bitdl2) clave[6] |= (1 << 0); //BIT_SET(num,j)
    else clave[6] &= ~(1 << 0); //BIT_CLEAR(num,j)
  }

}

void f(char *ra, char *sust_p, char *clave){
  char ra_p[6], sust[4];
  int i;

  /* Permutamos y expandemos Rn-1 32 bits --> 48 bits*/
  e(ra, ra_p);
  /*printf("E(Rn-1) = ");
  print_binary_bytes(ra_p, 6);*/

  /* Rn-1 permutado XOR clave_n */
  for(i=0; i<6; i++){
    ra_p[i] ^= clave[i];
  }
  /*printf("kn + E(Rn-1) = ");
  print_binary_bytes(ra_p, 6);*/

  Sboxes(ra_p, sust);
  /*printf("SBoxes = ");
  print_binary_bytes(sust, 4);*/

  p(sust, sust_p);
  /*printf("P(SBoxes) = ");
  print_binary_bytes(sust_p, 4);*/

}

void e(char *ra, char *ra_p){
  int i,j, cont = 0, posicion, per;

  for(i=0; i<6; i++) {
      for(j=0; j<8; j++) {
        per = E[cont] - 1;
        posicion = (per - (per%8))/8;
        /*printf("posicion = %d\n",posicion);*/
        if (((ra[posicion]) & (1 << 7-per%8))){//BIT_GET(num,i)
            ra_p[i] |= (1 << 7-j); //BIT_SET(num,j)
            /* print_binary_bytes(bloque_p,8); */
        } else {
          ra_p[i] &= ~(1 << 7-j); //BIT_CLEAR(num,j)
          /* print_binary_bytes(bloque_p,8); */
        }
        cont ++;
      }
  }
}

void p(char *sust, char *sust_p){
  int i,j, cont = 0, posicion, per;

  for(i=0; i<4; i++) {
      for(j=0; j<8; j++) {
        per = P[cont] - 1;
        posicion = (per - (per%8))/8;
        /*printf("posicion = %d\n",posicion);*/
        if (((sust[posicion]) & (1 << 7-per%8))){//BIT_GET(num,i)
            sust_p[i] |= (1 << 7-j); //BIT_SET(num,j)
            /* print_binary_bytes(bloque_p,8); */
        } else {
          sust_p[i] &= ~(1 << 7-j); //BIT_CLEAR(num,j)
          /* print_binary_bytes(bloque_p,8); */
        }
        cont ++;
      }
  }
}

void Sboxes(char *ra_p, char *sust){
  int i,j=0,cont = 0, b0 = 0, b1 = 4, b2 = 3, b3 = 2, b4 = 1, b5 = 0, f, c;
  int fila[2];
  int columna[4];
  char aux, auxt[8];

  /* Cogemos los 6 bits correspondientes b0b1b2b3b4b5*/
  for(i=0; i<8; i++){  /*7 1 3 5 7  6 0 2 4 6  5 7 1 3 5  4 6 0 2 4  3 5 7 1 3  2 4 6 0 2 4 6 0 2 4*/
    /* Bits fila */ /*[101010 11] [1111 0101] [11 101010] [111100 00] [1111 0101] [11 101010]*/

    b0 = controlb0(b0);
    b1 = controlb5(b1);
    b2 = controlb0(b2);
    b3 = controlb5(b3);
    b4 = controlb0(b4);
    b5 = controlb5(b5);
    /*printf("b0 %d - b1 %d - b2 %d - b3 %d - b4 %d - b5 %d\n", b0, b1, b2, b3, b4, b5);*/

    /* Bit fila[1] */
    if(ra_p[j] & (1 << b0)) fila[1] = 1;
    else fila[1] = 0;
    j = cambioByte(j, &cont);

    /* Bits columna */
    if(ra_p[j] & (1 << b1)) columna[3] = 1;
    else columna[3] = 0;
    j = cambioByte(j, &cont);

    if(ra_p[j] & (1 << b2)) columna[2] = 1;
    else columna[2] = 0;
    j = cambioByte(j, &cont);

    if(ra_p[j] & (1 << b3)) columna[1] = 1;
    else columna[1] = 0;
    j = cambioByte(j, &cont);

    if(ra_p[j] & (1 << b4)) columna[0] = 1;
    else columna[0] = 0;
    j = cambioByte(j, &cont);

    /* Bit fila[0] */
    if(ra_p[j] & (1 << b5)) fila[0] = 1;
    else fila[0] = 0;
    j = cambioByte(j, &cont);

    f = convertirDecimal(fila, 2);
    c = convertirDecimal(columna, 4);
    /*printf("f %d - c %d\n", f, c);*/

    auxt[i] = (char)S_BOXES[i][f][c];
    /*printf("sbox %d\n", auxt[i]);*/
  }

  /* Introducimos los valores de las S-Boxes en */
  cont = 0;
  j = 0;
  for(i=0; i<8; i++){
    if(cont == 0){
      sust[j] = 0x00;
      sust[j] |= (auxt[i] << 4);
      cont++;
    }
    else{
      sust[j] |= (auxt[i] << 0);
      j++;
      cont = 0;
    }
  }

}

int convertirDecimal(int *array, int tam){
  int i;
  int dec = 0;

  for(i=0; i<tam; i++){
    if(array[i] == 1) dec += pow(2,i);
  }

  return dec;
}

int controlb0(int ind){
  if(ind == 0) return 7;
  else if(ind == 7) return 1;
  else return ind + 2;
}

int controlb5(int ind){
  if(ind == 6) return 0;
  else return ind + 2;
}

int cambioByte(int ind, int *cont){
  (*cont)++;

  if(*cont == 8){
    ind++;
    *cont = 0;
  }

  return ind;
}

void copia_palabra_bytes(char* b1, char* b2, int nbytes){
  int i;

  for(i=0;i<nbytes;i++){
    b2[i] = b1[i];
  }

}

void print_binary_byte(char num){
    int i = 7;

    while (i >= 0) {
        if (((num) & (1 << i))) //BIT_GET(num,i) A = 65 = 0x41 = 01000001
            printf("1");
        else
            printf("0");
        i--;
    }
    printf("\n");
}

void print_binary_bytes(char* num, int nbytes){
    int i,j;

    i = 0;
    while (i < nbytes) {
        j = 7;
        while (j >= 0) {
            if (((num[i]) & (1 << j))) //BIT_GET(num,i)
                printf("1");
            else
                printf("0");
            j--;
        }
        printf(" ");
        i++;
    }
    printf("\n");
}

char** reserva_estructura_subclaves(){
  char** subclaves = NULL;
  int j;

  subclaves = (char**)calloc(16, sizeof(char*));
  if(!subclaves) return NULL;

  for(j=0; j < 16; j++){
    subclaves[j] = (char*)calloc(7+1, sizeof(char));
    if(!subclaves[j]){
      free(subclaves);
      return NULL;
    }
    subclaves[j][7] = 0x00;
  }

  return subclaves;
}

void libera_estructura_subclaves(char** subclaves){
  int j;

  for(j=0; j<16; j++){
    free(subclaves[j]);
  }
  free(subclaves);
}

int read_args_des(char* argv[], int argc, char *clave, char *iv, char* ifile, char* ofile, int triple){
  int flag = TECLADO;
  int i = 0, t = 8;

  if(triple) t = 24; // En caso de Triple DES clave de 192 bits

  if(strcmp(argv[2],"-k") == 0){
    if(strlen(argv[3]) < 8){
      printf("La clave debe ser de al menos 8 bytes (8 caracteres)\n");
      return ERROR;
    }
    for(i=0; i<t; i++){
      clave[i] = argv[3][i];
    }
  }else{
    printf("Necesario indicar la clave {-k}\n");
    return ERROR;
  }

  if(strcmp(argv[4],"-iv") == 0){
    if(strlen(argv[5]) < 8){
      printf("El vector de inicializacion debe ser de al menos 8 bytes (8 caracteres)\n");
      return ERROR;
    }
    for(i=0; i<8; i++){
      iv[i] = argv[5][i];
    }
  }else{
    printf("Necesario indicar el vector de inicializacion {-iv}\n");
    return ERROR;
  }

  if (argc > 6){

    if(strcmp(argv[6],"-i") == 0){
      strcpy(ifile,argv[7]);
      flag = IFILE;
    }
    else if(strcmp(argv[6],"-o") == 0){
      strcpy(ofile,argv[7]);
      flag = OFILE;
    }
    else {
      printf("Error. Debe indicar o fichero de entrada (-i) o de salida (-o)\n");
      return ERROR;
    }

    if(argc == 10){

      if(strcmp(argv[8],"-o") == 0){
        strcpy(ofile,argv[9]);
        flag = IOFILES;
      }
      else{
          printf("Error. Debe indicar fichero de salida (-o)\n");
          return ERROR;
      }
    }

  }

  if(flag == IFILE || flag == OFILE){
    printf("Error. Debe indicar ambos ficheros de entrada y salida\n");
    return ERROR;
  }

  return flag;
}
