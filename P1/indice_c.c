/*
Desc: Indice de Coincidencia de un Texto.
      Halla posible longitud n de la clave utilizada en el
      texto cifrado, y usando tamaños de ngramas indicados.
      ./indice_c {-l NGrama} [-i filein] [-o fileout]
      -l Longitud del NGrama buscado
      -i fichero de entrada
      -o fichero de salida
      IC(Ingles) = 0.065
      IC(Random) = 0.038
Authors: Daniel Mateo
         Laura Sanchez
File: indice_c.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "indice_c.h"


int main(int argc, char* argv[]){
    char ifile[FILENAME], ofile[FILENAME];
    FILE *ifl, *ofl; /* Ficheros de entrada y salida */
    int modo, l, flag;
    int i, n, num_bloques; /* Bloques = Vectores */
    double ic, ict; /* Indice de Coincidencia */
    char** bloques = NULL; /* Estructura con los bloques de tamanio n */
    char* texto_limpio = NULL; /* Texto sin caracteres no pertenecientes al Alfabeto */
    char* texto = NULL; /* Texto introducido/leido */

    /* Comprobamos numero de argumentos (como minimo 8) */
    if(argc < 3){
        printf("Error en los argumentos. Introduce:\n");
        printf("./indice_c {-l NGrama} [-i filein] [-o fileout]\n");
        printf("[-i] [-o] son opcionales.");
        return ERROR;
    }

    /* Se leen los argumentos */
    modo = read_args_afin(argv, argc, &l, ifile, ofile);
    if(modo == ERROR){
        return ERROR;
    }

    printf("Tamanio del NGrama: %d\n\n", l);

    /* Leemos el texto segun el modo */
    if(modo == TECLADO){
        texto = (char*)calloc(TAMTEXTO, sizeof(char));
        if(!texto){
            printf("Error al reservar la cadena del texto\n");
            return ERROR;
        }
        printf("Introduzca el texto a evaluar: \n");
        fgets(texto,TAMTEXTO,stdin);
    }
    else if(modo == IFILE){
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
    else if(modo == OFILE){
        ofl = fopen(ofile, "w");
        if (!ofl){
            printf("Error al abrir el fichero de salida\n");
        }

        texto = (char*)calloc(TAMTEXTO, sizeof(char));
        if(!texto){
            printf("Error al reservar la cadena del texto\n");
            return ERROR;
        }
        printf("Introduzca el texto a evaluar: \n");
        fgets(texto, TAMTEXTO, stdin);
    }
    else{
        ifl = fopen(ifile, "rb");
        if (!ifl){
            printf("Error al abrir el fichero de entrada\n");
        }
        ofl = fopen(ofile, "w");
        if (!ofl){
            printf("Error al abrir el fichero de salida\n");
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

    /*printf("\nTexto introducido: %s\n", texto);*/

    /* Limpiamos el texto para que contenga solo caracteres del Alfabeto */
    texto_limpio = limpia_texto(texto);
    if(!texto_limpio){
        printf("Error limpiando el texto.\n");
        return ERROR;
    }
    /*printf("Texto limpio: %s\n", texto_limpio);*/

    /* Bucle que halla los ICs de los bloques para cada tamaño n probado */
    flag = 0;
    for(n = 1; n <= NTRY; n++){
        ict = 0.0;
        /* Se divide el texto en n bloques */
        bloques = divide_texto_bloques_n(n, l, texto_limpio);
        printf("\n###Probando con n = %d:\n", n);
        for(i=0;i<n;i++){
            ic = 0.0;
            /*printf("B%d - %s\n", i+1, bloques[i]);*/
            /* Se calcula su IC */
            ic = calcula_ic(bloques[i], l);
            if(ic == ERROR){
                free(texto_limpio);
                libera_estructura_bloques(bloques, n);
                return ERROR;
            }
            printf("Vector%d con IC = %lf\n", i+1, ic);
            ict += ic;
        }
        libera_estructura_bloques(bloques, n);

        /* Media del IC de todos los bloques */
        ict = ict / n;
        printf("Media de IC = %lf para tamanio n = %d\n", ict, n);
        /* Se comprueba si es un IC valido (cercano al del Ingles) */
        if(ict <= ICINGLES+0.005 && ict >= ICINGLES-0.005){
            printf("Tamanio n = %d con IC = %lf y %d-gramas puede ser el correcto!\n", n, ict, l);
            flag = 1;
            /* Se escribe en fichero de salida si lo hay */
            if(modo == OFILE || modo == IOFILES){
                /* Por ahora se guarda el indice de incidencia y su correspondiente tamanio n de clave */
                fprintf(ofl, "Tamanio n = %d con IC = %lf y %d-gramas", n, ict, l);
                fclose(ofl);
            }
        }
    }

    if(!flag) printf("\nNo se ha encontrado ningun n valido para el texto introducido\n");
    else printf("\nSe ha encontrado algun valor n valido para el texto introducido!\n");

    free(texto_limpio);
    free(texto);

    return OK;
}


double calcula_ic(char* bloque, int ngrama){
    int i, j, k;
    double ic, l;
    char* aux = NULL;
    double* frecuencias = NULL;
    int flag;

    /* Cadena auxiliar para guardar el ngrama que se compara con los demas */
    aux = (char*)calloc(ngrama+1,sizeof(char));
    if(!aux){
        printf("Error de reserva de memoria para cadena auxiliar de un ngrama.\n");
        return ERROR;
    }
    aux[ngrama] = '\0';

    /* Numero de ngramas del bloque (l) */
    l = strlen(bloque) / ngrama;
    /*printf("Ngramas %d\n", l); */

    /* Array donde guardamos las frecuencias (como maximo tamanio l) */
    frecuencias = (double*)calloc(l,sizeof(double));
    if(!frecuencias) {
        printf("Error de reserva de memoria para el array de frecuencias.\n");
        free(aux);
        return ERROR;
    }

    /* Bucles de conteo de frecuencias de los ngramas en el bloque */
    for(j = 0; j < l*ngrama; j += ngrama){
        if(bloque[j] != '0') frecuencias[j/ngrama] = 1.0;
        else frecuencias[j/ngrama] = 0.0;

        /* Guardamos ngrama en aux */
        for(k = 0; k < ngrama; k++){
            aux[k] = bloque[j+k];
        }

        /* Recorremos resto de ngramas y comparamos con aux para sumar frecuencias */
        for(k = j+ngrama; k < l*ngrama; k += ngrama){
            flag = -1;
            /* Si coinciden los dos ngramas comparados, se suma 1 a su frecuencia */
            for(i = 0; i < ngrama; i++){
                if(aux[i] == bloque[k+i] && aux[i] != '0'){
                    if(flag == -1) flag = 1;
                }
                else flag = 0;
            }
            if(flag == 1){
                /* Sumamos 1 a su frecuencia correspondiente */
                frecuencias[j/ngrama] += 1.0;
                /* Los ngramas ya tenidos en cuenta se borran con un 0 en el bloque */
                for(i = k; i < k+ngrama; i++){
                    bloque[i] = '0';
                }
            }
        }

    }

    /* Calculo de frecuencias para IC => sumi=0_nchars f_i*(f_i - 1)/l(l-1)*/
    ic = 0.0;
    for(i=0;i<l;i++){
        ic += (frecuencias[i]*(frecuencias[i]-1));
    }
    ic /= (l*(l-1));

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

int read_args_afin(char* argv[], int argc, int* l, char* ifile, char* ofile) {
    int flag = TECLADO;

    if(strcmp(argv[1],"-l") == 0){
      *l = atoi(argv[2]);
    }
    else {
      printf("Error. Debe indicar el tamano del NGrama (-l)\n");
      return ERROR;
    }

    if (argc > 3){

      if(strcmp(argv[3],"-i") == 0){
        strcpy(ifile,argv[4]);
        flag = IFILE;
      }
      else if(strcmp(argv[3],"-o") == 0){
        strcpy(ofile,argv[4]);
        flag = OFILE;
      }
      else {
        printf("Error. Debe indicar o fichero de entrada (-i) o de salida (-o)\n");
        return ERROR;
      }

      if(argc == 7){
        if(strcmp(argv[5],"-o") == 0){
          strcpy(ofile,argv[6]);
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
