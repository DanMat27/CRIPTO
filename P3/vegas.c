/*
Desc: Dado el exponente de descifrado obtiene los dos numeros primos aleatorios que componen
      el modulo del RSA
      ./vegas -b numbits
      -b numero de bits significativos del modulo del RSA
Authors: Daniel Mateo
         Laura Sanchez
File: vegas.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "gmp.h"
#include "vegas.h"
#include "euclides_ext.h"

int main(int argc, char *argv[]){
    mpz_t p, q, n, fin, e, d, z;
    gmp_randstate_t seed;
    int bits;

    if(argc != 3){
        printf("Error en los argumentos.\n");
        printf("./vegas -b numbits\n");
        printf(" -b numero de bits significativos del modulo del RSA");
        return ERROR;
    }

    bits = atoi(argv[2]);

    /* Inicializamos estructuras gmp */
    mpz_init (p);
    mpz_init (q);
    mpz_init (n);
    mpz_init (fin);
    mpz_init (e);
    mpz_init (d);
    mpz_init (z);

    /* Genera primos aleatorios de bits/2 bits */
    gmp_randinit_mt (seed);
    gmp_randseed_ui (seed, time(NULL) * getpid()); /* Semilla */
    genera_primo_n_bits(p, bits/2, seed);
    genera_primo_n_bits(q, bits/2, seed);

    /* Obtenemos n */
    mpz_mul (n, p, q);

    /* Obtenemos fi(n) = (p-1)*(q-1) */
    mpz_sub_ui (p, p, 1);
    mpz_sub_ui (q, q, 1);
    mpz_mul (fin, p, q);

    mpz_add_ui (p, p, 1);
    mpz_add_ui (q, q, 1);

    gmp_printf ("### p generado: %Zd \n### q generado: %Zd\n", p, q);
    gmp_printf ("### n: %Zd\n", n);


    /* Buscamos un 1 < e < fi(n) que sea coprimo con fi(n) usando euclides*/
    /* En caso de ser coprimos calculamos si inverso usando euclides extendido*/
    mpz_random (e, 7); // a = rand()
    mpz_fdiv_r (e, e, fin); // e = e % fi(n)
    mpz_add_ui (e, e, 2); // e += 2
    while(1){
        if(euclides_ext(e, fin, d) == OK) break;
        mpz_add_ui (e, e, 1); // e ++
    }

    gmp_printf ("### e generado: %Zd \n### d generado: %Zd\n", e, d);

    /* Algoritmo de las Vegas */
    if(algoritmo_vegas(d, n, e, z) == NORESPONDE){
        printf("> No se ha podido hallar p y q\n");
    }
    else{
        printf("> p o q hallados!\n");
        gmp_printf ("### p o q: %Zd\n", z);
        mpz_fdiv_q (z, n, z);
        gmp_printf ("### p o q: %Zd\n", z);
    }

    /* Liberamos estructuras gmp */
    mpz_clear (p);
    mpz_clear (q);
    mpz_clear (n);
    mpz_clear (fin);
    mpz_clear (e);
    mpz_clear (d);
    mpz_clear (z);

    printf("\nSe ha liberado correctamente la memoria...\n");

    return OK;
}

int algoritmo_vegas(mpz_t d, mpz_t n, mpz_t e, mpz_t z){
    mpz_t mul, k, m, p1, mcd, aux, x, y, a;
    int ki, bits, i;

    mpz_init (k);
    mpz_init (m);
    mpz_init (mul);
    mpz_init (p1);
    mpz_init (mcd);
    mpz_init (aux);
    mpz_init (x);
    mpz_init (y);
    mpz_init (a);

    /* Hallamos m y k tales que e*d-1 = (2^k)*m */
    mpz_mul (mul, d, e);
    calcula_k_m(k, m, mul);
    // gmp_printf ("> %Zd - 1 = (2^%Zd) * %Zd\n", mul, k, m);

    /* Generamos numero a aleatorio 1 < a < n-1*/
    mpz_set (p1, n);
    mpz_sub_ui (p1, p1, 1); // p1 = n - 1
    mpz_random (a, 7); // a = rand()
    mpz_fdiv_r (a, a, p1); // a = a % (n-1)
    mpz_add_ui (a, a, 2); // a = a + 2 != 1

    /* Comprobamos si mcd(a,n) > 1 */
    euclides(a, n, mcd);
    // gmp_printf ("> euclides %Zd \n", mcd);

    /* Si lo cumple, mcd(a, n) es p o q */
    mpz_set_ui (aux, 1);
    if(mpz_cmp (mcd, aux) > 1){
        mpz_add_ui (z, mcd, 0);
        return 1;
    }

    /* Comprobar x = a^m mod n */
    mpz_powm(x, a, m, n); // x = a^m mod n
    // gmp_printf ("> x = %Zd\n", x);
    if(mpz_cmp (x, aux) == 0){ // x == 1
        return NORESPONDE;
    }
    else if(mpz_cmp (x, p1) == 0){ // x = n-1
        return NORESPONDE;
    }

    /* Comprobar x = a^((2^i)*m) mod n */
    ki = mpz_get_ui (k);
    for(i = 1; i < ki; i++){
        mpz_add_ui (y, x, 0); // y = x
        mpz_set_ui (aux, 2);
        mpz_powm(x, x, aux, n); // x = a^((2^i)*m) mod n
        mpz_set_ui (aux, 1);
        if(mpz_cmp (x, aux) == 0){ // x == 1
            mpz_add_ui (aux, y, 1);
            euclides(n, aux, z);
            return 1;
        }
        mpz_add_ui (aux, p1, 0);
        if(mpz_cmp (x, aux) == 0){ // x == -1 == n-1
            return NORESPONDE;
        }
    }

    /* Si sale del bucle devolvemos mcd(x+1, n) que es p o q */
    mpz_add_ui (aux, x, 1);
    euclides(n, aux, z);

    mpz_clear (k);
    mpz_clear (m);
    mpz_clear (mul);
    mpz_clear (p1);
    mpz_clear (mcd);
    mpz_clear (aux);
    mpz_clear (x);
    mpz_clear (y);
    mpz_clear (a);

    return 1;
}

void calcula_k_m(mpz_t k, mpz_t m, mpz_t p){
    int i, bits;
    mpz_t p1, x, aux, zero;

    mpz_init (p1);
    mpz_init (x);
    mpz_init (aux);
    mpz_init (zero);

    bits = mpz_sizeinbase (p, 2);

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


void genera_primo_n_bits(mpz_t primo, int bits, gmp_randstate_t seed){
    int pmr, pgmp;

    /* Generamos primo random de tamanio bits bits */
    mpz_rrandomb(primo, seed, bits); /* Rand */

    /* Se pone el ultimo bit a 1 para que sea impar */
    mpz_setbit(primo, 0);

    while(1){

        /* Si el primo no es divisible por ningun primo menor de 2k, puede que sea primo */
        pmr = prueba_tabla_primos(primo);
        if(pmr == ERROR){
            printf("> Error al abrir el fichero de primos.\n");
            return;
        }
        else if(pmr == POSIBLEPRIMO){
            /* Comprobamos si es probablemente primo con GMP */
            /* Si no, se continua buscando otro primo mejor */
            pgmp = mpz_probab_prime_p(primo, 10);
            if(pgmp == 0){
                pmr = COMPUESTO;
            }
        }

        /* Comprobamos si tras los tests es posible primo o hay que seguir probando */
        if(pmr == POSIBLEPRIMO) break;
        else{
            mpz_add_ui (primo, primo, 2); /* Se suma 2 al primo y se prueba con ese nuevo numero */
        }

        /* Si se pasa del tamanio de bits indicado se genera otro numero aleatorio del mismo tamanio */
        if(mpz_sizeinbase (primo, 2) > bits){
            mpz_rrandomb(primo, seed, bits); /* Rand */
            mpz_setbit(primo, 0); /* Impar */
        }
    }

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
