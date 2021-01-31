/*
Desc: Criptoanalisis del Metodo Vigenere.
      ./criptoanalisis_vigenere {-n tamClave} [-i filein]
      -n tamano clave
      -i fichero de entrada
Authors: Daniel Mateo
         Laura Sanchez
File: criptoanalisis_vigenere.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TECLADO 2 //Modo sin ficheros in ni out
#define IFILE 3 //Modo con fichero in
#define ICINGLES 0.065 //Indice de coincidencia del lenguaje ingles
#define ICRANDOM 0.038 //Indice de coincidencia de un lenguaje aletorio
#define TAMTEXTO 496 //Tamanio maximo cadena de texto
#define FILENAME 64 //Tamanio maximo cadena de nombre de un fichero

#define OK 1
#define ERROR -1

double calcula_ic_criptoanalisis(char* bloque, int k, double n, double l);
char** divide_texto_bloques_n(int n, int ngrama, char* texto);
void asigna_ngramas_a_bloques(char** bloques, char* texto, int npts, int ngrama, int n);
char** reserva_estructura_bloques(int tam, int n);
void libera_estructura_bloques(char** bls, int tam);
char* limpia_texto(char* texto);


int main(int argc, char const *argv[]) {
  char ifile[FILENAME];
  char* texto = NULL; /* Texto introducido/leido */
  char* texto_limpio = NULL; /* Texto sin caracteres no pertenecientes al Alfabeto */
  char** bloques = NULL; /* Estructura con los bloques de tamanio n */
  char* bloque_aux = NULL;
  FILE* ifl; /* Fichero de entrada */
  int modo, n, i, k;
  double ic;

  /* Comprobamos numero de argumentos (como minimo 2) */
  if(argc < 3){
    printf("Error en los argumentos. Introduce:\n");
    printf("./criptoanalisis_vigenere {-n tamClave} [-i filein]\n");
    printf("[-i] es opcional.\n");
    return ERROR;
  }

  if(strcmp(argv[1],"-n") == 0){
    n = atoi(argv[2]);
  }
  else {
    printf("Error. Debe indicar el tamaño de la clave\n");
    return ERROR;
  }

  if (argc > 3){
    if(strcmp(argv[3],"-i") == 0){
      strcpy(ifile,argv[4]);
      modo = IFILE;
    }
    else {
      printf("Error. Debe indicar o fichero de entrada (-i) o de salida (-o)\n");
      return ERROR;
    }
  } else {
    modo = TECLADO;
  }


  if (modo == TECLADO){
    printf("Introduzca lo que quiere criptoanalizar:\n");
    texto = (char*)calloc(TAMTEXTO, sizeof(char));
    if(!texto){
        printf("Error al reservar la cadena del texto\n");
        return ERROR;
    }
    fgets(texto,TAMTEXTO,stdin);
    printf("\nCriptoanalizando %s",texto);
  }
  /*Se ha indicado unicamente fichero de entrada*/
  else {
    printf("Criptoanalizamos el fichero de entrada: %s\n", ifile);
    ifl = fopen(ifile, "rb");

    if (!ifl){
        printf("Error al abrir el fichero de entrada\n");
        return ERROR;
    }

    /* Leemos el fichero de entrada entero y lo metemos en texto */
    fseek(ifl, 0, SEEK_END);
    long fsize = ftell(ifl);
    fseek(ifl, 0, SEEK_SET);

    texto = malloc(fsize + 1);
    if(!texto){
        printf("Error al reservar la cadena del texto\n");
        return ERROR;
    }
    fread(texto, 1, fsize, ifl);
    texto[fsize] = 0;

    fclose(ifl);
  }


  /* Limpiamos el texto para que contenga solo caracteres del Alfabeto */
  texto_limpio = limpia_texto(texto);
  if(!texto_limpio){
      printf("Error limpiando el texto.\n");
      return ERROR;
  }

  /* Dividimos el texto cifrado en bloques del tamaño de la clave n */
  bloques = divide_texto_bloques_n(n, 1, texto_limpio);

  /* Bucle para hallar las n partes de la clave */
  for(i=0;i<n;i++){
    printf("Probando para el vector %d\n",i);
	bloque_aux = (char*)calloc(strlen(bloques[i])+1,sizeof(char));
	if(!bloque_aux) return ERROR;
	/* Probamos cada posible subclave con cada vector */
    for(k=0;k<26;k++){
	  strcpy(bloque_aux, bloques[i]);
	  bloque_aux[strlen(bloques[i])] = '\0';
      ic = 0.0;
      /*printf("Subclave = %c\n",k+65);*/
      /*printf("B%d - %s\n", i+1, bloques[i]);*/
      /* Se calcula su IC */
      ic = calcula_ic_criptoanalisis(bloque_aux, k, (double)n, (double)strlen(texto_limpio));
      if(ic == ERROR){
          free(texto_limpio);
          libera_estructura_bloques(bloques, n);
          return ERROR;
      }
      /*printf("Vector con IC = %lf\n", ic);*/

      if(ic <= ICINGLES+0.005 && ic >= ICINGLES-0.005){
          printf("Posible k_%d = %c con IC = %lf\n", i, k+65, ic);
      }

    }
	free(bloque_aux);
  }

  libera_estructura_bloques(bloques, n);


  return OK;
}

double calcula_ic_criptoanalisis(char* bloque, int k, double n, double l){
    double array_pi[26] = {0.0804, 0.0154, 0.0306, 0.0399, 0.1251, 0.0230, 0.0196, 0.0549, 0.0726, 0.0016, 0.0067, 0.0414, 0.0253, 0.0709,
                           0.0760, 0.0200, 0.0011, 0.0612, 0.0654, 0.0925, 0.0271, 0.0099, 0.0192, 0.0019, 0.0173, 0.0019};
    int i, j, f, ncars;
    double ic;
    char* aux = NULL;
    double* frecuencias = NULL;
    int flag, car;
    int ngrama = 1;

    /* Cadena auxiliar para guardar el ngrama que se compara con los demas */
    aux = (char*)calloc(2,sizeof(char));
    if(!aux){
        printf("Error de reserva de memoria para cadena auxiliar de un ngrama.\n");
        return ERROR;
    }
    aux[1] = '\0';

    /* Numero de ngramas del bloque (l) */
    ncars = strlen(bloque);
    /*printf("Ngramas %d\n", l); */

	/*printf("Vector:\n");
    for(i=0; i<ncars; i++){
      printf("%c ",bloque[i]);
    }*/

    /* Array donde guardamos las frecuencias (como maximo tamanio l) */
    frecuencias = (double*)calloc(ncars,sizeof(double));
    if(!frecuencias) {
        printf("Error de reserva de memoria para el array de frecuencias.\n");
        free(aux);
        return ERROR;
    }

    /* Bucles de conteo de frecuencias de los ngramas en el bloque */
    for(j = 0; j < ncars*ngrama; j += ngrama){
        if(bloque[j] != '0') frecuencias[j/ngrama] = 1.0;
        else frecuencias[j/ngrama] = 0.0;

        /* Guardamos ngrama en aux */
        for(f = 0; f < ngrama; f++){
            aux[f] = bloque[j+f];
        }

        /* Recorremos resto de ngramas y comparamos con aux para sumar frecuencias */
        for(f = j+ngrama; f < ncars*ngrama; f += ngrama){
            flag = -1;
            /* Si coinciden los dos ngramas comparados, se suma 1 a su frecuencia */
            for(i = 0; i < ngrama; i++){
                if(aux[i] == bloque[f+i] && aux[i] != '0'){
                    if(flag == -1) flag = 1;
                }
                else flag = 0;
            }
            if(flag == 1){
                /* Sumamos 1 a su frecuencia correspondiente */
                frecuencias[j/ngrama] += 1.0;
                /* Los ngramas ya tenidos en cuenta se borran con un 0 en el bloque */
                for(i = f; i < f+ngrama; i++){
                    bloque[i] = '0';
                }
            }
        }

    }

    /*
    printf("Vector y frecuencias:\n");
    for(i=0; i<ncars; i++){
      printf("%c %d |",bloque[i], frecuencias[i]);
    }*/

    /*for(i=0;i<ncars;i++){
      printf("%lf ", frecuencias[i]);
    }*/

    /* Calculo de IC(X_j) = M(k_j) = n/l * sumi=0_25 p_i*(f_ik_j ) */
    /* p_i: probabilidad de que aparezca el caracter i (descifrado), f_tabla_i/100 */
    /* f_i: frecuencia del caracter i en X_j */
    /* k_j: clave con la que se cifra el vector j (X_j) */
    /* l: longitud del texto cifrado */
    /* n: numero de vectores = tamaño de la clave */
    ic = 0.0;
    for(i=0;i<ncars;i++){
        if(bloque[i] == '0') continue;
        car = (bloque[i] - 65) - k;
        if(car < 0) car = car + 26;
        ic += (array_pi[car]*(frecuencias[i]));
    }
    /*printf("IC%lf %lf %lf\n", ic, n, l);*/
    ic *= n/l;

    free(frecuencias);
    free(aux);

    return ic;
}

char** divide_texto_bloques_n(int n, int ngrama, char* texto){
    char** bloques = NULL;
    int i, num_particiones = 0;

    num_particiones = strlen(texto)/(n*ngrama);
    if (strlen(texto)%(n*ngrama) != 0) num_particiones++;

    bloques = reserva_estructura_bloques(n, num_particiones*ngrama);

    asigna_ngramas_a_bloques(bloques, texto, num_particiones, ngrama, n);

    return bloques;
}

void asigna_ngramas_a_bloques(char** bloques, char* texto, int npts, int ngrama, int n){
    int i, j, tam;
    int cont, b;

    tam = npts*ngrama;
    b = 0;
    for(i = 0; i < n; i++){
        cont = 0;
        for(j = b; j < strlen(texto) && cont < tam;){
            bloques[i][cont] = texto[j];
            cont++;
            if(cont%ngrama != 0) j++;
            else j += (n-1)*ngrama+1;
        }
        b += ngrama;
    }

}

char** reserva_estructura_bloques(int tam, int n){
    int i, j, k;
    char** bls = NULL;

    if(tam <= 0) return NULL;
    bls = (char**)calloc(tam,sizeof(char*));
    if(!bls) return NULL;
    for(i = 0; i < tam; i++){
       bls[i] = (char*)calloc(n+1,sizeof(char));
       if(!bls[i]) return NULL;
       bls[i][n] = '\0';
    }

    return bls;
}

void libera_estructura_bloques(char** bls, int tam){
    int i;
    if(tam <= 0 || !bls) return;
    for(i = 0; i < tam; i++){
        free(bls[i]);
    }
    free(bls);
    bls = NULL;
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
