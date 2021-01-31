/*
Desc: Comprueba que tenga seguridad perfecta el cifrado por desplazamiento
      cuando las claves son equiprobables y que no tiene cuando no lo son.
      ./seg_perf {-P | -I} [-i f ilein] [-o f ileout]
      -P se utiliza el metodo equiprobable
      -I se utiliza el metodo no equiprobable
      -i fichero de entrada
      -o fichero de salida
Authors: Daniel Mateo
         Laura Sanchez
File: seg_perf.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seg_perf.h"

int main(int argc, char *argv[]) {
  char ifile[50], ofile[50], texto[200];
  char *texto_fichero = NULL;
  char *texto_aux = NULL;
  FILE *i, *o;
  int modo, posicion = 0, j, k;
  double p_k[26];
  double p_px[26];
  double p_cy[26];
  double p_cyx[26*26];
  double p_pxy;
  int flag=1;
  /*double p_px[26] = {0.0804, 0.0154, 0.0306, 0.0399, 0.1251, 0.0230, 0.0196, 0.0549, 0.0726, 0.0016, 0.0067, 0.0414, 0.0253, 0.0709,
                         0.0760, 0.0200, 0.0011, 0.0612, 0.0654, 0.0925, 0.0271, 0.0099, 0.0192, 0.0019, 0.0173, 0.0019};*/

  /* Comprobamos numero de argumentos (como minimo 2) */
  if(argc < 2){
    printf("Error en los argumentos. Introduce:\n");
    printf("./seg_perf {-P|-I} [-i filein] [-o fileout]\n");
    printf("[-i] [-o] son opcionales.\n");
    return ERROR;
  }

  /* Leemos los argumentos pasados por terminal */
  /* modo es la opcion elegida: TECLADO, IFILE, OFILE, IOFILES */
  modo = read_args_seg_perf(argv, argc, ifile, ofile);
  if(modo == ERROR){
    return ERROR;
  }

  /* Inicializamos arrays a 0*/
  for(j=0; j<26; j++){
    p_k[j] = 0.0;
    p_cy[j] = 0.0;
    p_px[j] = 0.0;
  }
  for(j=0; j<26*26; j++){
    p_cyx[j] = 0.0;
  }

  if (strcmp(argv[1],"-P") == 0 || strcmp(argv[1],"-I") == 0 ) {

    /* Opcion equiprobabilidad en las claves */
    if(strcmp(argv[1],"-P") == 0 ){
      printf("# Ha elegido equiprobabilidad en las claves #\n\n");
    }/* Opcion no equiprobabilidad en las claves */
    else{
      printf("# Ha elegido no equiprobabilidad en las claves #\n\n");
    }


    /* No se ha indicado fichero de entrada ni fichero de salida */
    if (modo == TECLADO){
      printf("Introduzca lo que quiere cifrar:\n");
      fgets(texto,200,stdin);
      texto_aux = (char*)calloc(200,sizeof(char));
      if(!texto_aux){
          printf("Error al reservar la cadena del texto\n");
          return ERROR;
      }
      strcpy(texto_aux, texto);
      printf("\nCifrando con el metodo de desplazamiento %s",texto);

      if(strcmp(argv[1],"-P") == 0 ){
        cifrar_equiprobable(texto,p_k);
      }
      else{
        cifrar_no_equiprobable(texto,p_k);
      }

      //printf("Texto cifrado: %s\n", texto);
    }
    /* Se ha indicado unicamente fichero de entrada */
    else if (modo == IFILE){
      printf("Cifrando con el metodo de desplazamiento el fichero de entrada: %s\n", ifile);
      i = fopen(ifile, "r");

      if (i == NULL){
        printf("Error al abrir el fichero de entrada\n");
        return ERROR;
      }

      /* Leemos el fichero de entrada entero y lo metemos en texto_fichero */
      fseek(i, 0, SEEK_END);
      long fsize = ftell(i);
      fseek(i, 0, SEEK_SET);

      texto_fichero = malloc(fsize + 1);
      if(!texto_fichero){
          printf("Error al reservar la cadena del texto\n");
          return ERROR;
      }
      texto_aux = (char*)calloc(fsize+1,sizeof(char));
      if(!texto_aux){
          printf("Error al reservar la cadena del texto\n");
          return ERROR;
      }
      fread(texto_fichero, 1, fsize, i);
      texto_fichero[fsize] = 0;
      strcpy(texto_aux, texto_fichero);

      //printf("\nTexto cifrado: \n");

      if(strcmp(argv[1],"-P") == 0 ){
        cifrar_equiprobable(texto_fichero,p_k);
      }
      else{
        cifrar_no_equiprobable(texto_fichero,p_k);
      }

      //printf("%s", texto_fichero);

      fclose(i);

    }
    /* Se ha indicado unicamente fichero de salida */
    else if (modo == OFILE){
      printf("Introduzca lo que quiere cifrar:\n");
      fgets(texto,200,stdin);
      texto_aux = (char*)calloc(200,sizeof(char));
      if(!texto_aux){
          printf("Error al reservar la cadena del texto\n");
          return ERROR;
      }
      strcpy(texto_aux, texto);
      printf("\nCifrando con el metodo de desplazamiento %s",texto);

      if(strcmp(argv[1],"-P") == 0 ){
        cifrar_equiprobable(texto,p_k);
      }
      else{
        cifrar_no_equiprobable(texto,p_k);
      }

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
        return ERROR;
      }

      printf("Guardamos el texto cifrado en el fichero de salida: %s\n", ofile);
      fprintf (o, "%s", texto);

      fclose(o);

    }
    /* Se ha indicado fichero de entrada y fichero de salida */
    else {
      printf("Cifrando con el metodo de desplazamiento el fichero de entrada: %s\n", ifile);
      i = fopen(ifile, "r");

      if (i == NULL){
        printf("Error al abrir el fichero de entrada\n");
        return ERROR;
      }

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
        return ERROR;
      }

      printf("Guardamos el texto cifrado en el fichero de salida: %s\n", ofile);

      /* Leemos el fichero de entrada entero y lo metemos en texto_fichero */
      fseek(i, 0, SEEK_END);
      long fsize = ftell(i);
      fseek(i, 0, SEEK_SET);

      texto_fichero = malloc(fsize + 1);
      if(!texto_fichero){
          printf("Error al reservar la cadena del texto\n");
          return ERROR;
      }
      texto_aux = (char*)calloc(fsize+1,sizeof(char));
      if(!texto_aux){
          printf("Error al reservar la cadena del texto\n");
          return ERROR;
      }
      fread(texto_fichero, 1, fsize, i);
      texto_fichero[fsize] = 0;
      strcpy(texto_aux, texto_fichero);

      if(strcmp(argv[1],"-P") == 0 ){
        cifrar_equiprobable(texto_fichero,p_k);
      }
      else{
        cifrar_no_equiprobable(texto_fichero,p_k);
      }

      fprintf (o, "%s", texto_fichero);

      fclose(i);
      fclose(o);


    }
  }
  else {
    printf("Error. Debes elegir la opcion de claves equiprobables -P o no -I\n");
    return ERROR;
  }

  /* Calculamos las P_p(x|y) = (P_c(y|x) * P_p(x)) / P_c(y) */
  /* Primero calculamos P_c(y) y P_c(y|x)*/
  if (modo == IFILE || modo == IOFILES){
    calcula_p_caracter_plano_y_cifrado(texto_fichero, texto_aux, p_cy, p_px);
    calcula_p_caracter_cifrado_condicionado(texto_fichero, texto_aux, p_k, p_cyx);
  }
  else{
    calcula_p_caracter_plano_y_cifrado(texto, texto_aux, p_cy, p_px);
    calcula_p_caracter_cifrado_condicionado(texto, texto_aux, p_k, p_cyx);
  }

  /* Imprimimos primero las probabilididades de los caracteres en el texto plano */
  printf("########################### P_p(x) ################################\n");
  for(j=0; j<26; j++){
    printf("P_p(%c) = %lf\n", j+65, p_px[j]);
  }
  printf("\n");
  /* Imprimimos las p_k(k) */
  printf("########################### P_k(k) ################################\n");
  for(j=0; j<26; j++){
    printf("P_k(%c) = %lf\n", j+65, p_k[j]);
  }
  printf("\n");
  /* Imprimimos las p_c(y) */
  printf("########################### P_c(y) ################################\n");
  for(j=0; j<26; j++){
    printf("P_c(%c) = %lf\n", j+65, p_cy[j]);
  }
  printf("\n");
  printf("########################### P_c(y|x) ################################\n");
  /* Imprimimos las p_c(y|x) */
  for(j=0; j<26; j++){
    for(k=0; k<26; k++){
        printf("P_c(%c|%c) = %lf\n", j+65, k+65, p_cyx[j*26 + k]);
    }
  }
  printf("\n");

  /* Calculamos e imprimimos P_p(x|y) */
  printf("########################### P_p(x|y) ################################\n");
  for(j=0; j<26; j++){
    for(k=0; k<26; k++){

      p_pxy = ( p_cyx[j*26 + k] * p_px[k] ) / p_cy[j];
      printf("P_p(%c|%c) = P_c(%c|%c) * P_p(%c) / P_c(%c) = %lf\n", k+65, j+65, j+65, k+65, k+65, j+65, p_pxy);
      //printf("%lf - %lf\n", p_pxy, p_px[k]);

      /* Comprobamos si cumple la seguridad perfecta */
      if(p_pxy >= p_px[k] - 0.05 && p_pxy <= p_px[k] + 0.05) printf("Cumple seguridad perfecta P_p(%c|%c) ~= P_p(%c)\n", k+65, j+65, k+65);
      else{
        flag = 0;
        printf("No cumple la seguridad perfecta P_p(%c|%c) != P_p(%c)\n", k+65, j+65, k+65);
      }

    }
  }

  if(!flag) printf("\nEl criptosistema elegido no tiene seguridad perfecta!\n");
  else printf("\nEl criptosistema elegido si tiene seguridad perfecta!\n");

  if (modo == IFILE || modo == IOFILES){
    free(texto_fichero);
  }
  free(texto_aux);
  return OK;
}

int read_args_seg_perf(char* argv[], int argc, char* ifile, char* ofile){
  int flag = TECLADO;

  if (argc > 2){

    if(strcmp(argv[2],"-i") == 0){
      strcpy(ifile,argv[3]);
      flag = IFILE;
    }
    else if(strcmp(argv[2],"-o") == 0){
      strcpy(ofile,argv[3]);
      flag = OFILE;
    }
    else {
      printf("Error. Debe indicar o fichero de entrada (-i) o de salida (-o)\n");
      return ERROR;
    }

    if(argc == 6){

      if(strcmp(argv[4],"-o") == 0){
        strcpy(ofile,argv[5]);
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


void cifrar_equiprobable(char *texto, double *p_k){
  int cont = 0, valor_caracter, valor_clave, aux;
  int valor_nuevo_caracter, long_texto, i, cont_caracteres = 0;

  long_texto = strlen(texto);

  while (long_texto != 0){

    if (texto[cont] >= 65 && texto[cont] <= 90){
      /* 65 - 90 */
      cont_caracteres++;
      valor_clave = rand() % 26;
      p_k[valor_clave] += 1.0;
      valor_caracter = texto[cont] - 65;

      valor_nuevo_caracter = valor_caracter + valor_clave;

      if (valor_nuevo_caracter < 0){
        aux = valor_nuevo_caracter + 26;
      } else{
        aux = valor_nuevo_caracter % 26;
      }

      texto[cont] = aux + 65;

    }

    cont ++;
    long_texto--;

  }

  for(i=0; i<26; i++){
    p_k[i] /= cont_caracteres;
  }

  return;
}

void cifrar_no_equiprobable(char *texto, double *p_k){
  int cont = 0, valor_caracter, valor_clave, aux;
  int valor_nuevo_caracter, long_texto, i, cont_caracteres = 0;

  long_texto = strlen(texto);

  while (long_texto != 0){

    if (texto[cont] >= 65 && texto[cont] <= 90){
      /* 65 - 90 */
      cont_caracteres++;
      valor_clave = rand() % 100;

      if ( valor_clave >= 0 && valor_clave < 50 ) {
        valor_clave = 0;
      }
      else if (valor_clave >= 50 && valor_clave < 80) {
        valor_clave = 1;
      }
      else if (valor_clave >= 80 && valor_clave < 90) {
        valor_clave = 2;
      }
      else{
        valor_clave = rand() % (25-3+1) + 3; /*Generamos un valor entre 3 y 25 */
      }

      valor_caracter = texto[cont] - 65;

      valor_nuevo_caracter = valor_caracter + valor_clave;

      if (valor_nuevo_caracter < 0){
        aux = valor_nuevo_caracter + 26;
      } else{
        aux = valor_nuevo_caracter % 26;
      }

      texto[cont] = aux + 65;

      p_k[valor_clave]+=1.0;

    }

    cont ++;
    long_texto--;

  }

  for(i=0; i<26; i++){
    p_k[i] /= cont_caracteres;
  }

  return;
}

void calcula_p_caracter_plano_y_cifrado(char *texto_c, char *texto_p, double *p_cy, double *p_px){
  int cont = 0, cont_caracteres = 0, i;

  while(cont != strlen(texto_c)){

    if (texto_c[cont] >= 65 && texto_c[cont] <= 90){
      cont_caracteres++;

      p_cy[texto_c[cont]-65] += 1.0;
      p_px[texto_p[cont]-65] += 1.0;

    }

    cont ++;

  }

  for(i=0; i<26; i++){
    p_cy[i] /= cont_caracteres;
    p_px[i] /= cont_caracteres;
  }

  return;
}

void calcula_p_caracter_cifrado_condicionado(char* texto_c, char* texto_p, double* p_k, double* p_cyx){
  int len_texto, i;
  int char_c, char_p, valor_clave;

  len_texto = strlen(texto_c);

  for(i=0; i<len_texto; i++){

    if(texto_p[i] >= 65 && texto_p[i] <= 90){
      char_c = texto_c[i] - 65;
      char_p = texto_p[i] - 65;

      valor_clave = char_c - char_p;
      if (valor_clave < 0){
        valor_clave = valor_clave + 26;
      } else{
        valor_clave = valor_clave % 26;
      }

      /* y*26 + x */
      /* Guardamos en cada posicion del array p(y|x) para un x e y,
         vamos sumando las probabilidades de las claves que cumplen y = x + k */
      if(p_cyx[char_c*26 + char_p] == 0.0){
        p_cyx[char_c*26 + char_p] += p_k[valor_clave];
      }
    }

  }

}
