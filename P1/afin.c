/*
Desc: Cifrado y Descifrado por Metodo Afin.
      ./afin {-C|-D} {-m |Zm|} {-a Nx} {-b N+} [-i filein] [-o fileout]
      -C el programa cifra
      -D el programa descifra
      -m tamano del espacio de texto cifrado
      -a coeficiente multiplicativo de la funcion afin
      -b termino constante de la funcion afin
      -i fichero de entrada
      -o fichero de salida
Authors: Daniel Mateo
         Laura Sanchez
File: afin.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "afin.h"
#include "euclides_ext.h"
#include "gmp.h"

int main(int argc, char* argv[]){

  char texto[200], ifile[50], ofile[50];
  mpz_t m, a, b, inv;
  FILE *i, *o;
  int modo, cont, valor_caracter;

  /* Comprobamos numero de argumentos (como minimo 8) */
  if(argc < 8){
    printf("Error en los argumentos. Introduce:\n");
    printf("./afin {-C|-D} {-m |Zm|} {-a Nx} {-b N+} [-i filein] [-o fileout]\n");
    printf("[-i] [-o] son opcionales.");
    return ERROR;
  }

  mpz_init (m);
  mpz_init (a);
  mpz_init (b);
  mpz_init (inv);

  /*Leemos los argumentos pasados por terminal*/
  modo = read_args_afin(argv,argc,m,a,b,ifile,ofile);

  if(modo == ERROR){
    return ERROR;
  }

  /*Comprobamos que m es mayor que a*/
  if (mpz_cmp (m, a) < 1){
    printf("Error. El espacio m debe ser mayor que el numero a.\n");
    return ERROR;
  }

  /*Comprobamos que a tenga inverso multiplicativo y lo calculamos*/
  if (euclides_ext(argv[5], argv[3], inv) == ERROR){
    printf("Imposible cifrar por el metodo afin porque a no tiene inverso multiplicativo en este espacio\n");
    return ERROR;
  }

  /*Opcion cifrar*/
  if (strcmp(argv[1],"-C") == 0){
    printf("# Ha elegido cifrar #\n\n");

    /*No se ha indicado fichero de entrada ni fichero de salida*/
    if (modo == TECLADO){
      printf("Introduzca lo que quiere cifrar:\n");
      fgets(texto,200,stdin);
      printf("\nCifrando %s",texto);

      cifrar(texto,a,b,m);

      gmp_printf("Texto cifrado: %s\n", texto);
    }
    /*Se ha indicado unicamente fichero de entrada*/
    else if (modo == IFILE){
      printf("Ciframos el fichero de entrada: %s\n", ifile);
      i = fopen(ifile, "r");

      if (i == NULL){
        printf("Error al abrir el fichero de entrada\n");
        return ERROR;
      }

      gmp_printf("\nTexto cifrado: \n");

      while (fgets(texto, 200, i) != NULL){
        /* Aqui ciframos cada linea leida */
        cifrar(texto,a,b,m);

        gmp_printf("%s", texto);
      }

      fclose(i);

    }
    /*Se ha indicado unicamente fichero de salida*/
    else if (modo == OFILE){
      printf("Introduzca lo que quiere cifrar:\n");
      fgets(texto,200,stdin);
      printf("\nCifrando %s",texto);

      cifrar(texto,a,b,m);

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
        return ERROR;
      }

      printf("Guardamos el texto cifrado en el fichero de salida: %s\n", ofile);
      fprintf (o, "%s", texto);

      fclose(o);

    }
    /*Se ha indicado fichero de entrada y fichero de salida*/
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

      while (fgets(texto, 200, i) != NULL){
        /* Aqui ciframos cada linea leida */
        cifrar(texto,a,b,m);

        fprintf (o, "%s", texto);
      }

      fclose(i);
      fclose(o);
    }

  }/*Opcion descifrar*/
  else if (strcmp(argv[1],"-D") == 0){
    printf("# Ha elegido descifrar #\n\n");

    /*No se ha indicado fichero de entrada ni fichero de salida*/
    if (modo == TECLADO){
      printf("Introduzca lo que quiere descifrar:\n");
      fgets(texto,200,stdin);
      printf("\nDescifrando %s",texto);

      descifrar(texto,inv,b,m);

      gmp_printf("Texto descifrado: %s\n", texto);
    }
    /*Se ha indicado unicamente fichero de entrada*/
    else if (modo == IFILE){
      printf("Desciframos el fichero de entrada: %s\n", ifile);
      i = fopen(ifile, "r");

      if (i == NULL){
        printf("Error al abrir el fichero de entrada\n");
        return ERROR;
      }

      gmp_printf("\nTexto descifrado: \n");

      while (fgets(texto, 200, i) != NULL){
        /* Aqui desciframos cada linea leida */
        descifrar(texto,inv,b,m);

        gmp_printf("%s", texto);
      }

      fclose(i);

    }
    /*Se ha indicado unicamente fichero de salida*/
    else if (modo == OFILE){
      printf("Introduzca lo que quiere descifrar:\n");
      fgets(texto,200,stdin);
      printf("\nDescifrando %s",texto);

      descifrar(texto,inv,b,m);

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
        return ERROR;
      }

      printf("Guardamos el texto descifrado en el fichero de salida: %s\n", ofile);
      fprintf (o, "%s", texto);

      fclose(o);
    }
    /*Se ha indicado fichero de entrada y fichero de salida*/
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

      while (fgets(texto, 200, i) != NULL){
        /* Aqui desciframos cada linea leida */
        descifrar(texto,inv,b,m);

        fprintf (o, "%s", texto);
      }

      fclose(i);
      fclose(o);

    }

  }
  else{
    printf("Error. Debe elegir Cifrar (-C) o Descifrar (-D)\n");
    return ERROR;
  }

  mpz_clear (inv);
  mpz_clear (m);
  mpz_clear (a);
  mpz_clear (b);

  return OK;
}


int read_args_afin(char* argv[], int argc, mpz_t m, mpz_t a, mpz_t b, char* ifile, char* ofile) {
    int flag = TECLADO;

    if(strcmp(argv[2],"-m") == 0){
      mpz_set_str (m,argv[3],10);
    }
    else {
      printf("Error. Debe indicar el tamano del espacio (-m)\n");
      return ERROR;
    }

    if(strcmp(argv[4],"-a") == 0){
      mpz_set_str (a,argv[5],10);
    }
    else {
      printf("Error. Debe indicar el coeficiente multiplicativo (-a)\n");
      return ERROR;
    }

    if(strcmp(argv[6],"-b") == 0){
      mpz_set_str (b,argv[7],10);
    }
    else {
      printf("Error. Debe indicar el termino constante (-b)\n");
      return ERROR;
    }

    if (argc > 8){

      if(strcmp(argv[8],"-i") == 0){
        strcpy(ifile,argv[9]);
        flag = IFILE;
      }
      else if(strcmp(argv[8],"-o") == 0){
        strcpy(ofile,argv[9]);
        flag = OFILE;
      }
      else {
        printf("Error. Debe indicar o fichero de entrada (-i) o de salida (-o)\n");
        return ERROR;
      }

      if(argc == 12){

        if(strcmp(argv[10],"-o") == 0){
          strcpy(ofile,argv[11]);
          flag = IOFILES;
        }
        else{
            printf("Error. Debe indicar o fichero de salida (-o)\n");
            return ERROR;
        }
      }
    }

    return flag;
}

void cifrar(char* texto, mpz_t a, mpz_t b, mpz_t m){

  int valor_caracter, cont = 0;
  mpz_t x;

  mpz_init (x);

  while (texto[cont] != EOF && texto[cont] != '\n'){

    if (texto[cont] >= 65 && texto[cont] <= 90){
      /* 65 - 90 */
      valor_caracter = texto[cont] - 65;

      mpz_set_ui (x,valor_caracter);

      /*  a*x + b mod m */
      mpz_mul (x,x,a);
      mpz_add (x,x,b);
      mpz_fdiv_r (x,x,m);

      texto[cont] = mpz_get_ui(x) + 65;
    }

    cont++;

  }

  mpz_clear (x);

}

void descifrar(char* texto, mpz_t inv, mpz_t b, mpz_t m){

  int valor_caracter, cont = 0;
  mpz_t x;

  mpz_init (x);

  while (texto[cont] != EOF && texto[cont] != '\n'){

    if (texto[cont] >= 65 && texto[cont] <= 90){
      /* 65 - 90 */
      valor_caracter = texto[cont] - 65;

      mpz_set_ui (x,valor_caracter);

      /* (x-b)*a^-1 mod m */
      mpz_sub(x,x,b);
      mpz_mul(x,x,inv);
      mpz_fdiv_r(x,x,m);

      texto[cont] = mpz_get_ui(x) + 65;
    }

    cont++;

  }

  mpz_clear (x);

}
