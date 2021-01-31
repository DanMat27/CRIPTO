/*
Desc: Genera un numero probablemente primo de n bits que pasa ciertos tests (NTEST)
      ./primo -b bits -p sec [-o fileout]
      -b maximo de bits significativos del primo a generar
      -p probabilidad de equivocación que limita la eleccion del primo
      -o fichero opcional de salida
Authors: Daniel Mateo
         Laura Sanchez
File: primo.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include "gmp.h"
#include "primo.h"

int main(int argc, char *argv[])
{
    int bits; /* Tamanio bits primo */
    double sec, prob_test, cont_test; /* Probabilidad fallo primo */
    char ofile[50]; /* Fichero salida */
    FILE *o;
    int modo; /* Modo del programa (TECLADO/OFILE) */
    mpz_t primo, k, m; /* Primo resultante */
    gmp_randstate_t seed; /* Semilla aleatoria */
    int pmr, pgmp, i;
	clock_t tiempo_inicio, tiempo_final;
    double mins;

    /* Comprobamos argumentos */
    if(argc != 5 && argc != 7){
        printf("Error en el numero de argumentos.\n");
        printf("Debe introducir el formato: ./primo -b bits -p sec [-o fileout]\n");
        printf("-o es opcional\n");
        return ERROR;
    }

    /* Leemos argumentos de entrada */
    modo = read_args(argv, argc, &bits, &sec, ofile);
    printf("bits %d sec %lf\n", bits, sec);
    if(modo == ERROR){
        printf("Error en la lectura de argumentos.\n");
        return ERROR;
    }

    /* Inicializamos estructuras gmp */
    mpz_init (primo);
    mpz_init (k);
    mpz_init (m);
	
	tiempo_inicio = clock();

    /* Generamos primo random de tamanio bits bits */
    gmp_randinit_mt (seed);
    gmp_randseed_ui (seed, time(NULL) * getpid()); /* Semilla */
    mpz_rrandomb(primo, seed, bits); /* Rand */

    /* Se pone el ultimo bit a 1 para que sea impar */
    mpz_setbit(primo, 0);

    while(1){

        //gmp_printf("### Primo a probar: %Zd\n", primo);

        /* Si el primo no es divisible por ningun primo menor de 2k, puede que sea primo */
        pmr = prueba_tabla_primos(primo);
        if(pmr == ERROR){
            printf("> Error al abrir el fichero de primos.\n");
            return ERROR;
        }
        else if(pmr == POSIBLEPRIMO){
            //printf("> El primo pasa la prueba de la tabla de primos.\n");

            /* Ejecutamos NTEST tests de Miller-Rabin y contamos cuantas veces lo falla */
            cont_test = 0.0;

            /* Hallamos m y k tales que p-1 = (2^k)*m */
            calcula_k_m(k, m, primo, bits);
            // gmp_printf ("> %Zd - 1 = (2^%Zd) * %Zd\n", p, k, m);

            for(i = 0; i < NTEST; i++){
                pmr = test_miller_rabin(primo, k, m, bits, sec);
                if(pmr == POSIBLEPRIMO){
                    //printf("> El primo pasa el test de Miller-Rabin!\n");
                }
                else {
                    //printf("> El primo no pasa el test de Miller-Rabin.\n");
                    cont_test++;
                }
            }

            /* Probabilidad de equivocacion del primo en Miller-Rabin */
            prob_test = cont_test / NTEST;

            /* Comprobamos si satisface el error indicado por argumentos */
            /* Si no, se continua buscando otro primo mejor */
            if(prob_test <= sec){
                /* Test Miller-Rabin de GMP */
                pgmp = mpz_probab_prime_p(primo, NTEST);
                pmr = POSIBLEPRIMO;
            }
            else{
                pmr = COMPUESTO;
            }
        }
        else {
            //printf("> El primo no pasa la prueba de la tabla de primos. Es compuesto\n");
        }
        //printf("\n");

        /* Comprobamos si tras los tests es posible primo o hay que seguir probando */
        if(pmr == POSIBLEPRIMO) break;
        else{
            mpz_add_ui (primo, primo, 2); /* Se suma 2 al primo y se prueba con ese nuevo numero */
        }

        /* Si se pasa del tamanio de bits indicado se genera otro numero aleatorio del mismo tamanio */
        if(mpz_sizeinbase (primo, 2) > bits){
            //printf("> Num maximo de %d bits alcanzado. Regenerando un numero aleatorio...\n\n", bits);
            mpz_rrandomb(primo, seed, bits); /* Rand */
            mpz_setbit(primo, 0); /* Impar */
        }
    }
	tiempo_final = clock();

    /* Salida de los resultados */
    if(modo == TECLADO){
        printf("### Tamanio bits indicado: %d\n", bits);
        printf("### Probabilidad error indicada: %lf\n", sec);
        gmp_printf ("### Numero generado: %Zd\n", primo);
        printf("### Nuestro resultado: Es probablemente primo\n");
        if(pgmp == 0){
            printf("### Resultado GMP: No es primo, es compuesto\n");
        }
        else if(pgmp == 1){
            printf("### Resultado GMP: Es probablemente primo\n");
        }
        else printf("### Resultado GMP: Es primo seguro\n");
        printf("### Probabilidad equivocacion: %lf\n", prob_test);
        printf("### Pasa el test: %d veces de %d tests realizados\n", (NTEST - (int)cont_test), NTEST);
    }
    else if(modo == OFILE){
        o = fopen(ofile, "w");
        fprintf(o, "### Tamanio bits indicado: %d\n", bits);
        fprintf(o, "### Probabilidad error indicada: %lf\n", sec);
        fprintf(o, "### Numero generado: %s\n", mpz_get_str(NULL, 10, primo));
        fprintf(o, "### Nuestro resultado: Es probablemente primo\n");
        if(pgmp == 0){
            fprintf(o, "### Resultado GMP: No es primo, es compuesto\n");
        }
        else if(pgmp == 1){
            fprintf(o, "### Resultado GMP: Es probablemente primo\n");
        }
        else fprintf(o, "### Resultado GMP: Es primo seguro\n");
        fprintf(o, "### Probabilidad equivocacion: %lf\n", prob_test);
        fprintf(o, "### Pasa el test: %d veces de %d tests realizados\n", (NTEST - (int)cont_test), NTEST);
        fclose(o);
    }
	
	mins = (double)(tiempo_final - tiempo_inicio) / (CLOCKS_PER_SEC*60.0); 
    printf("T Ejecucion: %f min\n", mins);

    /* Liberamos estructuras gmp */
    mpz_clear (primo);
    mpz_clear (k);
    mpz_clear (m);

    printf("\nSe ha liberado correctamente la memoria...\n");

    return OK;
}


int test_miller_rabin(mpz_t p, mpz_t k, mpz_t m, int bits, double sec){
    mpz_t a, p1; /* a aleatorio */
    mpz_t x, aux; /* Para pruebas */
    int i, ki, flag = COMPUESTO;

    mpz_init (a);
    mpz_init (p1);
    mpz_init (x);
    mpz_init (aux);

    /* Generamos numero a aleatorio */
    /* Tiene que cumplir que 1 < a < p-1 */
    mpz_set (p1, p);
    mpz_sub_ui (p1, p1, 1); // p1 = p - 1
    mpz_random (a, 7); // a = rand()
    mpz_fdiv_r (a, a, p1); // a = a % (p-1)
    mpz_add_ui (a, a, 2); // a = a + 2 != 1
    // gmp_printf ("> Se prueba con a = %Zd\n", a);

    /* Se prueba la primera propiedad */
    /* Comprobar x = a^m mod p */
    mpz_powm(x, a, m, p); // x = a^m mod p
    // gmp_printf ("> x = %Zd\n", x);
    mpz_set_ui (aux, 1);
    if(mpz_cmp (x, aux) == 0){ // x == 1
        flag = POSIBLEPRIMO;
    }
    else if(mpz_cmp (x, p1) == 0){ // x = p-1
        flag = POSIBLEPRIMO;
    }
    /* Se prueba la otra propiedad */
    else{
        /* Comprobar x = a^((2^i)*m) mod p */
        ki = mpz_get_ui (k);
        for(i = 1; i <= ki; i++){
            mpz_set_ui (aux, 2);
            mpz_powm(x, x, aux, p); // x = a^((2^i)*m) mod p
            mpz_set_ui (aux, 1);
            if(mpz_cmp (x, aux) == 0){ // x == 1
                flag = COMPUESTO;
            }
            else if(mpz_cmp (x, p1) == 0){ // x == p-1
                flag = POSIBLEPRIMO;
            }
        }
    }

    mpz_init (a);
    mpz_init (p1);
    mpz_init (x);
    mpz_init (aux);

    return flag;
}

void calcula_k_m(mpz_t k, mpz_t m, mpz_t p, int bits){
    int i;
    mpz_t p1, x, aux, zero;

    mpz_init (p1);
    mpz_init (x);
    mpz_init (aux);
    mpz_init (zero);

    mpz_set (p1, p);
    mpz_sub_ui (p1, p1, 1); // p1 = p - 1
    mpz_set_ui (x, 1); // x = 1
    mpz_set_ui (zero, 0);
    for(i = 1; i < bits-1; i++){
        mpz_mul_ui (x, x, 2); // x = 2^i
        mpz_div (m, p1, x); // m = p1/x
        mpz_set_ui (aux, 2);
        mpz_fdiv_r (aux, m, aux); // m % 2
        if(mpz_cmp (aux, zero) != 0){ // m % 2 != 0
            mpz_set_ui (k, i); // k = i
            break;
        }
    }

    mpz_clear (p1);
    mpz_clear (x);
    mpz_clear (aux);
    mpz_clear (zero);
}

int prueba_tabla_primos(mpz_t p){
    int flag=POSIBLEPRIMO;
    unsigned int pcmp;
    FILE *f=NULL;
    mpz_t aux, zero;

    mpz_init (aux);
    mpz_init (zero);

    /*Leemos el fichero donde estan escritos los numeros primos del 2 al 2053*/
    f = fopen("primos_2k.txt", "r");
    if(!f) return ERROR;

    mpz_set_ui (zero, 0);
    while(fscanf(f, "%d", &pcmp) != EOF){
        // printf("%d\n", pcmp);
        mpz_set_ui (aux, pcmp);
        if(mpz_cmp (p, aux) < 1){ // Si p es mas pequenio no se divide
            mpz_set_ui (aux, 1);
        }
        else{
            mpz_fdiv_r (aux, p, aux); // p % pcmp
        }

        if(mpz_cmp (aux, zero) == 0){ // p % pcmp == 0
            flag = COMPUESTO;
            break;
        }
    }

    fclose(f);
    mpz_clear (aux);
    mpz_clear (zero);

    return flag;
}

int read_args(char* argv[], int argc, int *bits, double *sec, char* ofile){
    int flag = TECLADO;

    if(strcmp(argv[1],"-b") == 0){
        *bits = atof(argv[2]);
    }
    else{
        printf("Error en argumento -b\n");
        return ERROR;
    }

    if(strcmp(argv[3],"-p") == 0){
        *sec = atof(argv[4]);
    }
    else{
        printf("Error en argumento -p\n");
        return ERROR;
    }

    if(argc == 7){
        if(strcmp(argv[5],"-o") == 0){
            strcpy(ofile, argv[6]);
            flag = OFILE;
        }
        else{
            printf("Error en argumento opcional -o\n");
            return ERROR;
        }
    }

    return flag;
}
