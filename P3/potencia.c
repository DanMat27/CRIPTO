/*
Desc: Realiza la potenciación de grandes numeros
      ./potencia [base] [exponente] [modulo]
      -base es el numero base en el calculo
      -exponente es la cifra exponente en el calculo
      -modulo es el espacio m modular del calculo
Authors: Daniel Mateo
         Laura Sanchez
File: potencia.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmp.h"
#include "potencia.h"

int main(int argc, char *argv[])
{
    mpz_t base, exponente, modulo, x;
    char *cadena_binaria;
    int i, l;

    /* Iniciamos estructuras de gmp */
    mpz_init (base);
    mpz_init (exponente);
    mpz_init (modulo);
    mpz_init (x);

    /* Comprobamos numero de argumentos, tienen que ser 4 */
    if(argc != 4){
      printf("Error en los argumentos. Introduce:\n");
      printf("./potencia [base] [exponente] [modulo]\n");
      return ERROR;
    }

    /* Asociamos los valores de los argumentos */
    mpz_set_str (base,argv[1],10);
    mpz_set_str (exponente,argv[2],10);
    mpz_set_str (modulo,argv[3],10);
    mpz_set_str (x,"1",10);

    /* Calcular orden del exponente L */
    for(i=9000-1; i>=0; i--){
        if(mpz_tstbit(exponente, i)){ /* Si bit i a 1 */
            l = i+1;
            break;
        }
    }
    printf("El orden del exponente es: %d\n", l);

    /* Calculo de la potencia modular */
    for(i=l-1; i>=0; i--){
        /* x = x^2 mod n */
        mpz_mul(x,x,x);
        mpz_fdiv_r(x,x,modulo);

        if(mpz_tstbit(exponente, i)){ /* Si bit i a 1 */
            /* x = x*b mod n */
            mpz_mul(x,x,base);
            mpz_fdiv_r(x,x,modulo);
        }
    }

    printf("--- Nuestro resultado: \n");
    gmp_printf("%Zd ^ %Zd mod %Zd = %Zd\n", base, exponente, modulo, x);
    printf("--- Resultado GMP: \n");
    mpz_powm(x, base, exponente, modulo);
    gmp_printf("%Zd ^ %Zd mod %Zd = %Zd\n", base, exponente, modulo, x);


    /* Liberamos estructuras gmp */
    mpz_clear (base);
    mpz_clear (exponente);
    mpz_clear (modulo);
    mpz_clear (x);

    printf("Se ha liberado correctamente la memoria...\n");

    return OK;
}
