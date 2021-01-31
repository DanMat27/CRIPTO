/*
Desc: Criptoanalisis - Metodo Vigenere. Obtiene la longitud de la clave mediante
      el test de Kasiski
      ./kasiski {-l TamRepeticion} [-i filein]
      -l longitud de las repeticiones buscadas
      -i fichero de entrada
Authors: Daniel Mateo
         Laura Sanchez
File: kasiski.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kasiski.h"
#include "euclides.h"

int main(int argc, char *argv[]) {

  int modo, longitud_repeticion, longitud_clave;
  long fsize;
  char ifile[50], texto[200];
  char *texto_fichero;
  FILE *i, *o;

  /* Comprobamos numero de argumentos (como minimo 3) */
  if(argc < 3){
    printf("Error en los argumentos. Introduce:\n");
    printf("./kasiski {-l TamRepeticion} [-i filein]\n");
    printf("[-i] es opcional.\n");
    return ERROR;
  }

  /* Leemos los argumentos pasados por terminal */
  /* modo es la opcion elegida: TECLADO, IFILE*/
  modo = read_args_kasiski(argv, argc, &longitud_repeticion, ifile);

  if(modo == ERROR){
    return ERROR;
  }

  if (modo == TECLADO){
    printf("Introduce el texto cifrado por el metodo vigenere del cual obtendremos");
    printf(" la longitud de la clave\n");

    fgets(texto,200,stdin);

    obtener_longitud_clave(texto,longitud_repeticion);

  }
  /* Se ha indicado unicamente fichero de entrada */
  else if (modo == IFILE){
    printf("\nCogeremos el texto del fichero de entrada %s del cual obtendremos", ifile);
    printf(" la longitud de la clave.\n\n");

    i = fopen(ifile, "rb");
    if (!i){
      printf("Error al abrir el fichero de entrada\n");
      return ERROR;
    }

    /* Leemos el fichero de entrada entero y lo metemos en texto_fichero */
    fseek(i, 0, SEEK_END);
    fsize = ftell(i);
    fseek(i, 0, SEEK_SET);

    texto_fichero = malloc(fsize + 1);
    if(!texto_fichero){
        printf("Error al reservar la cadena del texto\n");
        return ERROR;
    }
    fread(texto_fichero, 1, fsize, i);
    texto_fichero[fsize] = 0;

    fclose(i);

    obtener_longitud_clave(texto_fichero,longitud_repeticion);

    free(texto_fichero);

  }

  return OK;
}

int read_args_kasiski(char* argv[], int argc, int *l, char* ifile){
  int flag = TECLADO;

  if(strcmp(argv[1],"-l") == 0){
    *l = atoi(argv[2]);
  }
  else {
    printf("Error. Debe indicar la longitud de las repeticiones que vamos a buscar (-l)\n");
    return ERROR;
  }

  if (argc > 3){

    if(strcmp(argv[3],"-i") == 0){
      strcpy(ifile,argv[4]);
      flag = IFILE;
    }
    else {
      printf("Error. Debe indicar o fichero de entrada (-i)\n");
      return ERROR;
    }

  }

  return flag;
}

void obtener_longitud_clave(char* texto, int longitud_repeticion){
  char *texto_limpio = NULL, repeticion_buscada[longitud_repeticion];
  int longitud_texto, i = 0, j, flag_norepeticion = 0, ngrama, p = 0;
  int posicion_primera_aparicion, mcd_aux, aux, n_encontrada = 0, r = 0;
  int *array_posiciones = NULL;

  if (limpia_texto(texto) == NULL){
    return;
  }

  /* Primero quitamos todos los caracteres ASCII que no formen parte de nuestro
     alfabetto (65 - 90) */
  texto_limpio = limpia_texto(texto);

  longitud_texto = strlen(texto_limpio);

  /* Bucle principal encargado de calcular el tamaño de la clave n, si es posible */
  while(n_encontrada == 0){

    array_posiciones = malloc(sizeof(int)*p);

    posicion_primera_aparicion = r;

    if (r + longitud_repeticion > longitud_texto){
      printf("\nNo se ha encontrado el valor de n\n");
      break;
    }

    /* Copiamos en repeticion_buscada el conjunto de caracteres que vamos a buscar */
    for (i = 0; i < longitud_repeticion; i++){
      repeticion_buscada[i] = texto_limpio[r+i];
    }
    repeticion_buscada[longitud_repeticion] = '\0';

    printf("\nLa repeticion buscada es: %s\n", repeticion_buscada);

    /* Comprobamos las veces que se repite y guardamos la posicion */
    for(i = r + longitud_repeticion; i < longitud_texto; i++){

      ngrama = i;

      for(j = 0; j < longitud_repeticion; j++){

        if (flag_norepeticion == 0){
          if ( texto_limpio[ngrama] != repeticion_buscada[j] ){
            flag_norepeticion = 1;
          }
        }

        ngrama ++;
      }

      if (flag_norepeticion == 0){
        array_posiciones = realloc(array_posiciones, sizeof(int)*(p+1));
        array_posiciones[p] = i;
        printf("Posicion %d\n",array_posiciones[p]);
        p++;
      }

      flag_norepeticion = 0;

    }

    /* Si se han encontrado repeticiones */
    if(p != 0){

      /*Una vez obtenidas las posiciones restamos por la posicion de la primera aparicion*/
      printf("mcd(");
      for(i = 0; i < p; i++){
        array_posiciones[i] = array_posiciones[i] - posicion_primera_aparicion;
        printf(" %d ",array_posiciones[i]);
      }
      printf(") = ");

      /* La logitud de la clave cumple: n | mcd(distancias) */
      /* Primero calculamos el mcd(distancias) */
      aux = array_posiciones[0];

      if(p != 1){
        for (i = 1; i < p; i++){
          mcd_aux = mcd(array_posiciones[i], aux);
          aux = mcd_aux;
        }
      }

      printf("%d\n", aux);

      /*Vemos para que valores de n se cumple que n | mcd(distancias) */
      for (i = aux; i > 0; i--){
        if ( aux % i == 0 ){
          printf("n puede ser %d\n",i);
          n_encontrada = 1;
        }
      }

      if (n_encontrada == 0){
        r++;
        if (r == longitud_texto){
          n_encontrada = 1;
          printf("No se ha encontrado el valor de n\n");
        }
        free(array_posiciones);
        p = 0;
      }

    }/* No se ha encontrado ninguna repeticion */
     else {
      printf("No hay repeticiones de %s\n", repeticion_buscada);
      r++;
      if (r == longitud_texto){
        n_encontrada = 1;
        printf("No se ha encontrado el valor de n\n");
      }

    }

  }

  free(array_posiciones);

  return;
}

char* limpia_texto(char* texto){
    int i, cont;
    char* texto_limpio = NULL;

    /* Cuento letras pertenecientes al alfabeto */
    cont = 0;
    for(i = 0; i < strlen(texto); i++){
      if(texto[i] >= 65 && texto[i] <= 90) cont++;
    }

    texto_limpio = (char*)calloc(cont+1,sizeof(char));
    if(!texto_limpio) return NULL;

    /* Saco esas letras al texto limpio */
    cont = 0;
    for(i = 0; i < strlen(texto); i++){
      if(texto[i] >= 65 && texto[i] <= 90){
          texto_limpio[cont] = texto[i];
          cont++;
      }
    }
    texto_limpio[cont] = '\0';

    return texto_limpio;
}
