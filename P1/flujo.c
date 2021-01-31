/*
Desc: Cifrado y Descifrado con un Cifrado de Flujo.
      ./flujo {-C|-D} [-i filein] [-o fileout]
      -C el programa cifra
      -D el programa descifra
      -i fichero de entrada
      -o fichero de salida
Authors: Daniel Mateo
         Laura Sanchez
File: flujo.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flujo.h"

int main(int argc, char *argv[]) {

  char ifile[50], ofile[50], texto[200];
  char *texto_fichero = NULL;
  FILE *i, *o;
  int modo, posicion = 0;

  /* Comprobamos numero de argumentos (como minimo 3) */
  if(argc < 2){
    printf("Error en los argumentos. Introduce:\n");
    printf("./flujo {-C|-D} [-i filein] [-o fileout]\n");
    printf("[-i] [-o] son opcionales.\n");
    return ERROR;
  }

  /* Leemos los argumentos pasados por terminal */
  /* modo es la opcion elegida: TECLADO, IFILE, OFILE, IOFILES */
  modo = read_args_flujo(argv, argc, ifile, ofile);
  if(modo == ERROR){
    return ERROR;
  }

  /* Opcion cifrar */
  if (strcmp(argv[1],"-C") == 0) {
    printf("# Ha elegido cifrar #\n\n");

    /* No se ha indicado fichero de entrada ni fichero de salida */
    if (modo == TECLADO){
      printf("Introduzca lo que quiere cifrar:\n");
      fgets(texto,200,stdin);
      printf("\nCifrando %s",texto);

      cifrar(texto);

      printf("Texto cifrado: %s\n", texto);
    }
    /* Se ha indicado unicamente fichero de entrada */
    else if (modo == IFILE){
      printf("Ciframos el fichero de entrada: %s\n", ifile);
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
      fread(texto_fichero, 1, fsize, i);
      texto_fichero[fsize] = 0;

      printf("\nTexto cifrado: \n");

      cifrar(texto_fichero);

      printf("%s", texto_fichero);

      fclose(i);
      free(texto_fichero);

    }
    /* Se ha indicado unicamente fichero de salida */
    else if (modo == OFILE){
      printf("Introduzca lo que quiere cifrar:\n");
      fgets(texto,200,stdin);
      printf("\nCifrando %s",texto);

      cifrar(texto);

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
      printf("Ciframos el fichero de entrada: %s\n", ifile);
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
      fread(texto_fichero, 1, fsize, i);
      texto_fichero[fsize] = 0;

      cifrar(texto_fichero);

      fprintf (o, "%s", texto_fichero);

      fclose(i);
      fclose(o);
      free(texto_fichero);

    }
  }
  /* Opcion descifrar */
  else if (strcmp(argv[1],"-D") == 0){
    printf("# Ha elegido descifrar #\n\n");

    /* No se ha indicado fichero de entrada ni fichero de salida */
    if (modo == TECLADO){
      printf("Introduzca lo que quiere descifrar:\n");
      fgets(texto,200,stdin);
      printf("\nDescifrando %s",texto);

      descifrar(texto);

      printf("Texto descifrado: %s\n", texto);
    }
    /* Se ha indicado unicamente fichero de entrada */
    else if (modo == IFILE){
      printf("Desciframos el fichero de entrada: %s\n", ifile);
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
      fread(texto_fichero, 1, fsize, i);
      texto_fichero[fsize] = 0;

      printf("\nTexto descifrado: \n");

      descifrar(texto_fichero);

      printf("%s", texto_fichero);

      fclose(i);
      free(texto_fichero);

    }
    /* Se ha indicado unicamente fichero de salida */
    else if (modo == OFILE){
      printf("Introduzca lo que quiere descifrar:\n");
      fgets(texto,200,stdin);
      printf("\nDescifrando %s",texto);

      descifrar(texto);

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
        return ERROR;
      }

      printf("Guardamos el texto descifrado en el fichero de salida: %s\n", ofile);
      fprintf (o, "%s", texto);

      fclose(o);
    }
    /* Se ha indicado fichero de entrada y fichero de salida */
    else {
      printf("Desciframos el fichero de entrada: %s\n", ifile);
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

      printf("Guardamos el texto descifrado en el fichero de salida: %s\n", ofile);

      /* Leemos el fichero de entrada entero y lo metemos en texto_fichero */
      fseek(i, 0, SEEK_END);
      long fsize = ftell(i);
      fseek(i, 0, SEEK_SET);

      texto_fichero = malloc(fsize + 1);
      if(!texto_fichero){
          printf("Error al reservar la cadena del texto\n");
          return ERROR;
      }
      fread(texto_fichero, 1, fsize, i);
      texto_fichero[fsize] = 0;

      descifrar(texto_fichero);

      fprintf (o, "%s", texto_fichero);

      fclose(i);
      fclose(o);
      free(texto_fichero);

    }
  }
  else {
    printf("Error. Debe elegir Cifrar (-C) o Descifrar (-D)\n");
    return ERROR;
  }

  return OK;
}

int read_args_flujo(char* argv[], int argc, char* ifile, char* ofile){
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

void descifrar(char* texto){
  int long_bloque = 0, cont = 0, valor_caracter, valor_clave, aux;
  int valor_nuevo_caracter, long_texto, coc;
  int k = 3; /* Clave del flujo */

  long_texto = strlen(texto);

  while (long_texto != 0){

    if (texto[cont] >= 65 && texto[cont] <= 90){
      /* Generamos la clave del flujo */
      k = flujo_claves_pseudoaleatorias(k);

      /* 65 - 90 */
      valor_caracter = texto[cont] - 65;
      valor_clave = k;
      valor_nuevo_caracter = valor_caracter - valor_clave;

      if (valor_nuevo_caracter < 0){
        coc = (-1)*valor_nuevo_caracter / 26;
        aux = valor_nuevo_caracter + (coc+1)*26;
        aux = aux % 26;
      } else{
        aux = valor_nuevo_caracter % 26;
      }

      texto[cont] = aux + 65;
    }

    cont++;
    long_texto--;

  }

  return;
}

void cifrar(char *texto){
  int long_bloque = 0, cont = 0, valor_caracter, valor_clave, aux;
  int valor_nuevo_caracter, long_texto, coc;
  int k = 3; /* Clave del flujo */

  long_texto = strlen(texto);

  while (long_texto != 0){

    if (texto[cont] >= 65 && texto[cont] <= 90){
      /* Generamos la clave del flujo */
      k = flujo_claves_pseudoaleatorias(k);

      /* 65 - 90 */
      valor_caracter = texto[cont] - 65;
      valor_clave = k;
      valor_nuevo_caracter = valor_caracter + valor_clave;

      if (valor_nuevo_caracter < 0){
        coc = (-1)*valor_nuevo_caracter / 26;
        aux = valor_nuevo_caracter + (coc+1)*26;
        aux = aux % 26;
      } else{
        aux = valor_nuevo_caracter % 26;
      }

      texto[cont] = aux + 65;
    }

    cont ++;
    long_texto--;

  }

  return;
}


int flujo_claves_pseudoaleatorias(int x){
  int a = 23, b = 7, m = 101; /* Semilla */
  int k; /* Clave generada */

  k = (a*x + b) % m;

  return k;
}
