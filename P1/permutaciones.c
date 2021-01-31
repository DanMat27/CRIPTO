/*
Desc: Cifrado y Descifrado con un Doble Cifrado de Permutacion.
      ./permutaciones {-C|-D} {-k1 K1 -k2 K2} [-i filein] [-o fileout]
      -C el programa cifra
      -D el programa descifra
      -k1 vector para permutar las filas de tamaño N
      -k2 vector para permutar las columnas de tamaño M
      -i fichero de entrada
      -o fichero de salida
Authors: Daniel Mateo
         Laura Sanchez
File: permutaciones.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "permutaciones.h"

int main(int argc, char *argv[]) {
  char k1[9], k2[9], ifile[64], ofile[64], texto[200], k1_inversa[9], k2_inversa[9];
  char aux;
  int n, m, i, j, x, k, cont = 0;
  int *bloque_texto_plano = NULL;
  int *bloque_texto_cifrado = NULL;
  int *array_posiciones = NULL;
  int modo;
  int flag, cif;
  char *texto_limpio = NULL, *texto_fichero = NULL;
  FILE *ifl, *ofl;

  /* Comprobamos numero de argumentos (como minimo 3) */
  if(argc < 6){
    printf("Error en los argumentos. Introduce:\n");
    printf("./permutaciones {-C|-D} {-k1 K1 -k2 K2} [-i filein] [-o fileout]\n");
    printf("[-i] [-o] son opcionales.\n");
    return ERROR;
  }

  /* Leemos los argumentos pasados por terminal */
  /* modo es la opcion elegida: TECLADO, IFILE, OFILE, IOFILES */
  modo = read_args_permutaciones(argv, argc, k1, k2, ifile, ofile);
  if(modo == ERROR){
    return ERROR;
  }

  /* Sacamos los valores N y M (tamanios matrices) */
  n = strlen(k1);
  m = strlen(k2);

  /*Comprobamos que los vectores k1 y k2 son correctos*/
  if(comprueba_claves_correctas(k1, k2) == ERROR){
    printf("No se pueden repetir numeros en las claves.\n");
    printf("Tampoco pueden poseer numeros mayores que N*M o menores que 1\n");
    return ERROR;
  }

  /*Elección de cifrado*/
  if(strcmp(argv[1], "-C") == 0){
    printf("# Ha elegido cifrar #\n");
    cif = 1;
  }
  /*Elección de descifrado*/
  else if(strcmp(argv[1], "-D") == 0){
    printf("# Ha elegido descifrar #\n");
    cif = 0;
  }
  /* Eleccion erronea */
  else{
    printf("# Ha elegido una opcion erronea #\n");
    return ERROR;
  }

  /* No se introduce ni fichero de entrada ni de salida o se ha introducido fichero de salida solo */
  if (modo == TECLADO || modo == OFILE){
    printf("Introduzca el texto:\n");
    fgets(texto,200,stdin);

    /* Copiamos el texto introducido por teclado en texto_limpio */
    texto_limpio = malloc(strlen(texto));
    strcpy(texto_limpio,texto);

  } /* Se ha introducido fichero de entrada o tanto el fichero de entrada como el de salida*/
  else if ( modo == IFILE || modo == IOFILES){
    printf("Leemos el fichero de entrada: %s\n", ifile);

    ifl = fopen(ifile, "r");

    if (ifl == NULL){
      printf("Error al abrir el fichero de entrada\n");
      return ERROR;
    }

    /* Leemos el fichero de entrada entero y lo metemos en texto_fichero */
    fseek(ifl, 0, SEEK_END);
    long fsize = ftell(ifl);
    fseek(ifl, 0, SEEK_SET);

    texto_fichero = malloc(fsize + 1);
    if(!texto_fichero){
        printf("Error al reservar la cadena del texto\n");
        return ERROR;
    }
    fread(texto_fichero, 1, fsize, ifl);
    texto_fichero[fsize] = 0;

    /* Copiamos el texto leido en texto_limpio */
    texto_limpio = malloc(strlen(texto_fichero));
    strcpy(texto_limpio,texto_fichero);

    free(texto_fichero);
    fclose(ifl);

  }
  else{
    return ERROR;
  }

  /* Proceso de permutaciones sobre cada bloque del texto plano */
  flag = 0;
  for(i=0; i < strlen(texto_limpio);){

    /* Array donde vamos guardando las posiciones de los caracteres cifrados en el bloque */
    cont = 0;
    array_posiciones = malloc(sizeof(int)*cont);

    /* Reservamos memoria para el array que contiene un bloque del texto plano (matriz) */
    bloque_texto_plano = (int*)calloc(m*n,sizeof(int));
    if(!bloque_texto_plano) return ERROR;

    /* Reservamos memoria para el array que contiene un bloque cifrado (matriz) */
    bloque_texto_cifrado = (int*)calloc(m*n,sizeof(int));
    if(!bloque_texto_cifrado) return ERROR;

    /* Asignamos a cada posicion de la matriz el caracter correspondiente del texto plano */
    for (j = 0; j<m*n; j++){
      /* No ciframos aquellos caracteres que no formen parte del alfabeto */
      if (texto_limpio[i] <= 90 || texto_limpio[i] >= 65 ){
        bloque_texto_plano[j] = texto_limpio[i];
        cont ++;
        array_posiciones = realloc(array_posiciones, sizeof(int)*(cont));
        array_posiciones[cont-1] = i;
      }
      i++;
      if(i > strlen(texto_limpio)){
        flag = i;
        break;
      }
    }

    /* Bloque incompleto, no continuamos cifrando */
    if(flag != 0) break;

    if(cif) {
      /* Realizamos la primera permutación por filas con k1 */
      for(x=0; x<m; x++){
        for(j=0; j<n; j++){
          k = k1[j] - 49;
          bloque_texto_cifrado[x*n + j] = bloque_texto_plano[x*n + k];
        }
      }

      /* Confirmamos cambios de la primera permutacion */
      for(j=0; j<m*n; j++){
          bloque_texto_plano[j] = bloque_texto_cifrado[j];
      }

      /* Realizamos la segunda permutación por columnas con k2 */
      for(x=0; x<n; x++){
        for(j=0; j<m; j++){
          k = k2[j] - 49;
          bloque_texto_cifrado[j*n + x] = bloque_texto_plano[n*k + x];
        }
      }

    }
    else {

      /* Calculamos las permutaciones inversas de k1 y k2 */
      for(x = 0; x < n; x++){
        k1_inversa[k1[x]-49] = x+1 + 48;
      }

      for(x = 0; x < m; x++){
        k2_inversa[k2[x]-49] = x+1 + 48;
      }

      /* Realizamos la primera permutación por columnas con k2 */
      for(x=0; x<n; x++){
        for(j=0; j<m; j++){
          k = k2_inversa[j] - 49;
          bloque_texto_cifrado[j*n + x] = bloque_texto_plano[n*k + x];
        }
      }

      /* Confirmamos cambios de la primera permutacion */
      for(j=0; j<m*n; j++){
          bloque_texto_plano[j] = bloque_texto_cifrado[j];
      }

      /* Realizamos la segunda permutación por filas con k1 */
      for(x=0; x<m; x++){
        for(j=0; j<n; j++){
          k = k1_inversa[j] - 49;
          bloque_texto_cifrado[x*n + j] = bloque_texto_plano[x*n + k];
        }
      }

    }

    /* Copiamos el resultado en texto limpio en las posiciones correctas */
    for(j=0; j<m*n; j++){
        texto_limpio[array_posiciones[j]] = bloque_texto_cifrado[j];
    }

    /* Liberamos el bloque */
    free(bloque_texto_plano);
    free(bloque_texto_cifrado);
    free(array_posiciones);
  }

  /*Imprimimos por pantalla el resultado*/
  if(modo == TECLADO || modo == IFILE){
    printf("Resultado:\n");
    fprintf(stdout, "%s\n", texto_limpio);
  /* Escribimos el resultado en el fichero de salida en caso de haberlo indicado */
  } else {
    ofl = fopen(ofile, "w");

    if (ofl == NULL){
      printf("Error al abrir el fichero de salida\n");
      return ERROR;
    }

    printf("\nEscribimos el resultado en el fichero de salida: %s\n",ofile);
    fprintf (ofl, "%s", texto_limpio);

    fclose(ofl);

  }

  free(texto_limpio);

  return OK;
}


int read_args_permutaciones(char* argv[], int argc, char* k1, char* k2, char* ifile, char* ofile){
  int flag = TECLADO;

  if(strcmp(argv[2],"-k1") == 0){
    strcpy(k1,argv[3]);
  }
  else {
    printf("Error. Debe indicar la cadena de caracteres usada como primera clave (-k1)\n");
    return ERROR;
  }

  if(strcmp(argv[4],"-k2") == 0){
    strcpy(k2,argv[5]);
  }
  else {
    printf("Error. Debe indicar la cadena de caracteres usada como segunda clave (-k2)\n");
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

  return flag;
}

int comprueba_claves_correctas(char *k1, char *k2){
  char aux;
  int i,j,m,n;
  int flag = 0;

  n = strlen(k1);
  m = strlen(k2);

  /* Comprobamos k1 */
  for(j=0; j<n; j++){
    aux = k1[j] - 48;
    if(aux > n || aux <= 0) return ERROR;
    for(i=j+1; i<n; i++){
      if (aux == k1[i] - 48){
        return ERROR;
      }
    }
  }

  /* Comprobamos k2 */
  for(j=0; j<m; j++){
    aux = k2[j] - 48;
    if(aux > m || aux <= 0) return ERROR;
    for(i=j+1; i<m; i++){
      if (aux == k2[i] - 48){
        return ERROR;
      }
    }
  }

  return OK;
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
