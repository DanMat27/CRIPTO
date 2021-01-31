/*
Desc: Cifrado y Descifrado por Metodo Afin mejorado.
      ./afin_mejorado {-C|-D} {-m |Zm|} {-a Nx} {-b N+} [-i filein] [-o fileout]
      -C el programa cifra
      -D el programa descifra
      -m tamano del espacio de texto cifrado
      -a coeficiente multiplicativo de la funcion afin
      -b termino constante de la funcion afin
      -i fichero de entrada
      -o fichero de salida
Authors: Daniel Mateo
         Laura Sanchez
File: afin_mejorado.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "euclides_ext.h"
#include "gmp.h"

#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in
#define OFILE 4 //Modo con fichero out
#define IOFILES 5 //Modo con ficheros in y out

int read_args_afin(char* argv[], int argc, mpz_t m, mpz_t a, mpz_t b, char* ifile, char* ofile);
void cifrar(char*** alfabeto, char* texto, mpz_t a, mpz_t b, mpz_t m);
void descifrar(char*** alfabeto, char* texto, mpz_t inv, mpz_t b, mpz_t m);

/* 
* Function: char*** genera_matriz_bigramas()
* Desc: Reserva memoria y devuelve una matriz
*       de bigramas. Esta será el alfabeto.
* Ret: matriz o NULL
*/
char*** genera_matriz_bigramas(){
  char*** matriz;
  int k, j;

  matriz = (char***)calloc(26,sizeof(char**));
  if(!matriz) return NULL;
  for(k=0;k<26;k++){
    matriz[k] = (char**)calloc(26,sizeof(char*));
    if(!matriz[k]) return NULL;
    for(j=0;j<26;j++){
      matriz[k][j] = malloc(3 * sizeof(char));
      if(!matriz[k][j]) return NULL;
      matriz[k][j][0] = 65 + k;
      matriz[k][j][1] = 65 + j;
      matriz[k][j][2] = '\0';
    }
  }

  return matriz;
}

/* 
* Function: void libera_matriz_bigramas(char*** matriz)
* Desc: Libera la memoria de la matriz de bigramas.
* Ret: -
*/
void libera_matriz_bigramas(char*** matriz){
  int i, j;

  for (i = 0; i < 26; i++){
    for (j = 0; j < 26; j++)
      free(matriz[i][j]);
    free(matriz[i]);
  }
  free(matriz);
}

/* 
* Function: char* bigrama_en_alfabeto(int pos, char*** matriz)
* Desc: Copia el bigrama de la posicion pos de la matriz.
* Ret: bigrama o NULL
*/
void bigrama_en_alfabeto(int pos, char*** matriz, char* bigrama){
  int i,j;

  for (i = 0; i < 26; i++){
    for (j = 0; j < 26; j++){
      if(pos == 0){
        strcpy(bigrama, matriz[i][j]);
        return;
      } 
      pos--;
    }
  }

}

/* 
* Function: void imprime_matriz(char*** matriz)
* Desc: Imprime la matriz alfabeto de bigramas por pantalla.
* Ret: -
*/
void imprime_matriz(char*** matriz){
  int i,j;

  for(i = 0; i < 26; i++){
    printf("  %d", i);
  }
  printf("\n");

  for (i = 0; i < 26; i++){
    printf("%d ", i);
    for (j = 0; j < 26; j++){
      if(j<10) printf("%s ", matriz[i][j]);
      else printf("%s  ", matriz[i][j]);
    }
    printf("\n");
  }
    
}



int main(int argc, char* argv[]){

  char texto[200], ifile[50], ofile[50];
  mpz_t m, a, b, inv;
  FILE *i, *o;
  int modo, cont, valor_caracter;
  char*** alfabeto;

  /* Comprobamos numero de argumentos (como minimo 8) */
  if(argc < 8){
    printf("Error en los argumentos. Introduce:\n");
    printf("./afin_mejorado {-C|-D} {-m |Zm|} {-a Nx} {-b N+} [-i filein] [-o fileout]\n");
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

  if (_mpz_cmp_si (m, 676) != 0){
    mpz_set_str (m,"676",10); /* 26*26 */
  }

  /*Comprobamos que m es mayor que a*/
  if (mpz_cmp (m, a) < 1){
    printf("Error. El espacio m debe ser mayor que el numero a.\n");
    return ERROR;
  }

  /*Comprobamos que a tenga inverso multiplicativo y lo calculamos*/
  if (euclides_ext(argv[5], "676", inv) == ERROR){
    printf("Imposible cifrar por el metodo afin porque a no tiene inverso multiplicativo en este espacio\n");
    return ERROR;
  }

  alfabeto = genera_matriz_bigramas();
  if(!alfabeto){
    printf("Error al reservar memoria para la matriz de bigramas del alfabeto.\n");
    return ERROR;
  }

  /*imprime_matriz(alfabeto);*/

  /*Opcion cifrar*/
  if (strcmp(argv[1],"-C") == 0){
    printf("# Ha elegido cifrar #\n\n");

    /*No se ha indicado fichero de entrada ni fichero de salida*/
    if (modo == TECLADO){
      printf("Introduzca lo que quiere cifrar:\n");
      fgets(texto,200,stdin);
      printf("\nCifrando %s",texto);

      cifrar(alfabeto,texto,a,b,m);

      gmp_printf("Texto cifrado: %s\n", texto);
    }
    /*Se ha indicado unicamente fichero de entrada*/
    else if (modo == IFILE){
      printf("Ciframos el fichero de entrada: %s\n", ifile);
      i = fopen(ifile, "r");

      if (i == NULL){
        printf("Error al abrir el fichero de entrada\n");
      }

      gmp_printf("\nTexto cifrado: \n");

      while (fgets(texto, 200, i) != NULL){
        /* Aqui ciframos cada linea leida */
        cifrar(alfabeto,texto,a,b,m);

        gmp_printf("%s", texto);
      }

      fclose(i);

    }
    /*Se ha indicado unicamente fichero de salida*/
    else if (modo == OFILE){
      printf("Introduzca lo que quiere cifrar:\n");
      fgets(texto,200,stdin);
      printf("\nCifrando %s",texto);

      cifrar(alfabeto,texto,a,b,m);

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
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
      }

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
      }

      printf("Guardamos el texto cifrado en el fichero de salida: %s\n", ofile);

      while (fgets(texto, 200, i) != NULL){
        /* Aqui ciframos cada linea leida */
        cifrar(alfabeto,texto,a,b,m);

        fprintf (o, "%s", texto);
      }

      fclose(i);
      fclose(o);
    }

  }/*Opcion descifrar*/
  else if (strcmp(argv[1],"-D") == 0){
    printf("# Ha elegido descifrar #\n\n");

    if (modo == TECLADO){
      printf("Introduzca lo que quiere descifrar:\n");
      fgets(texto,200,stdin);
      printf("\nDescifrando %s",texto);

      descifrar(alfabeto,texto,inv,b,m);

      gmp_printf("Texto descifrado: %s\n", texto);
    }
    /*Se ha indicado unicamente fichero de entrada*/
    else if (modo == IFILE){
      printf("Desciframos el fichero de entrada: %s\n", ifile);
      i = fopen(ifile, "r");

      if (i == NULL){
        printf("Error al abrir el fichero de entrada\n");
      }

      gmp_printf("\nTexto descifrado: \n");

      while (fgets(texto, 200, i) != NULL){
        /* Aqui ciframos cada linea leida */
        descifrar(alfabeto,texto,inv,b,m);

        gmp_printf("%s", texto);
      }

      fclose(i);

    }
    /*Se ha indicado unicamente fichero de salida*/
    else if (modo == OFILE){
      printf("Introduzca lo que quiere descifrar:\n");
      fgets(texto,200,stdin);
      printf("\nDescifrando %s",texto);

      descifrar(alfabeto,texto,inv,b,m);

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
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
      }

      o = fopen(ofile, "w");

      if (o == NULL){
        printf("Error al abrir el fichero de salida\n");
      }

      printf("Guardamos el texto descifrado en el fichero de salida: %s\n", ofile);

      while (fgets(texto, 200, i) != NULL){
        /* Aqui ciframos cada linea leida */
        descifrar(alfabeto,texto,inv,b,m);

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

void cifrar(char*** alfabeto, char* texto, mpz_t a, mpz_t b, mpz_t m){

  int valor_caracter, cont = 0;
  mpz_t x;
  int flag; /* Flag que indica si ya es bigrama */
  int char_anterior, cont_anterior; /* Primer char del bigrama */
  int pos; /* Valores en alfabeto */
  char big_cif[] = {'A','A','\0'};

  mpz_init (x);
  
  flag = 0;
  while (texto[cont] != EOF && texto[cont] != '\n'){
    
    if (texto[cont] >= 65 && texto[cont] <= 90){
      /* 65 - 90 */
      valor_caracter = texto[cont] - 65;

      /* Se cifran cada par de caracteres (bigramas) */
      if(!flag){
        char_anterior = valor_caracter;
        cont_anterior = cont;
        flag = 1;
      }
      else{
        /* Valor del bigrama plano en alfabeto */
        pos = char_anterior*26 + valor_caracter;
        mpz_set_ui (x,pos);
        /*  a*x + b mod m */
        mpz_mul (x,x,a);
        mpz_add (x,x,b);
        mpz_fdiv_r (x,x,m);
        /* Valor del bigrama cifrado en alfabeto */
        pos = mpz_get_ui(x);
        bigrama_en_alfabeto(pos, alfabeto, big_cif);
        /* Sustituimos cada caracter en su sitio correspondiente */
        texto[cont] = big_cif[1];
        texto[cont_anterior] = big_cif[0];
        flag = 0;
      }
      
    }

    cont++;

  }

  mpz_clear (x);

}

void descifrar(char*** alfabeto, char* texto, mpz_t inv, mpz_t b, mpz_t m){

  int valor_caracter, cont = 0;
  mpz_t x;
  int flag; /* Flag que indica si ya es bigrama */
  int char_cif_anterior, cont_cif_anterior; /* Primer char del bigrama cifrado */
  int pos; /* Valores en alfabeto */
  char big_des[] = {'A','A','\0'};
  
  mpz_init (x);

  flag = 0;
  while (texto[cont] != EOF && texto[cont] != '\n'){

    if (texto[cont] >= 65 && texto[cont] <= 90){
      /* 65 - 90 */
      valor_caracter = texto[cont] - 65;

      /* Se descifran cada par de caracteres (bigramas) */
      if(!flag){
        char_cif_anterior = valor_caracter;
        cont_cif_anterior = cont;
        flag = 1;
      }
      else{
        /* Valor del bigrama cifrado en alfabeto */
        pos = char_cif_anterior*26 + valor_caracter;
        mpz_set_ui (x,pos);
        /* (x-b)*a^-1 mod m */
        mpz_sub(x,x,b);
        mpz_mul(x,x,inv);
        mpz_fdiv_r(x,x,m);
        /* Valor del bigrama descifrado en alfabeto */
        pos = mpz_get_ui(x);
        bigrama_en_alfabeto(pos, alfabeto, big_des);
        /* Sustituimos cada caracter en su sitio correspondiente */
        texto[cont] = big_des[1];
        texto[cont_cif_anterior] = big_des[0];
        flag = 0;
      }

    }

    cont++;

  }

  mpz_clear (x);

}

