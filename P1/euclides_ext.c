/*
Desc: Algoritmo de Euclides Extendido
      Halla el inverso a^-1 de un numero a
      siendo coprimo con el espacio m (b).
      Si no tiene inverso, devuelve NULL.
Authors: Daniel Mateo
         Laura Sanchez
File: euclides_ext.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gmp.h"
#include "euclides_ext.h"

int euclides_ext(char* cada, char* cadb, mpz_t inv){
    mpz_t rs[1000];
    int i, cont, signo;
	  mpz_t a,b,r,coc,rplus1,rplus2,x,c;

    /* Iniciamos estructuras de gmp */
    mpz_init (a);
    mpz_init (b); //m
    mpz_init (rplus1);
    mpz_init (rplus2);
    mpz_init (r);
    mpz_init (coc);
    mpz_init (x);
    mpz_init (c);

  	mpz_set_str (a,cada,10);
  	mpz_set_str (b,cadb,10);

    /* Si m es menor que a, les damos la vuelta */
    if (mpz_cmp (b, a) < 1) {
      mpz_set_str (a,cadb,10);
  	  mpz_set_str (b,cada,10);
    }

    fprintf(stdout, "> Numeros introducidos: \n");
    gmp_printf  ("m | a : %Zd | %Zd\n\n", b, a);
    fprintf(stdout, "> Desarrollo del algoritmo de euclides: \n");

    /* Bucle del calculo del MCD(m,a) del algoritmo de Euclides normal */
    mpz_add_ui (r,b,0);
    mpz_add_ui (rplus1,a,0);
    mpz_fdiv_r (rplus2, r, rplus1);
    mpz_fdiv_q (coc, r, rplus1);
    cont = -1;
    gmp_printf  ("%Zd = %Zd x %Zd + %Zd\n", r, coc, rplus1, rplus2);
    while(_mpz_cmp_si (rplus2, 0) != 0){
        cont++;
        /* Guardamos los restos y el cociente en el array para mas tarde */
        mpz_add_ui(rs[cont*4],r,0);
        mpz_add_ui(rs[cont*4 + 1],coc,0);
        mpz_add_ui(rs[cont*4 + 2],rplus1,0);
        mpz_add_ui(rs[cont*4 + 3],rplus2,0);
        
        /* Calculo de los restos */
        mpz_add_ui(r,rplus1,0);
        mpz_add_ui(rplus1,rplus2,0);
        mpz_fdiv_r (rplus2, r, rplus1);
        mpz_fdiv_q (coc, r, rplus1);
        gmp_printf  ("%Zd = %Zd x %Zd + %Zd\n", r, coc, rplus1, rplus2);
    }

    /* Comprobamos si el MCD es 1 ==> Son coprimos a y m */
    if(_mpz_cmp_si (rplus1, 1) == 0){
        gmp_printf  ("\nmcd(%Zd,%Zd) = 1 => SI SON COPRIMOS\n", b, a);
        fprintf(stdout, "\n> Se va a proceder a hallar el inverso multiplicativo modular por el algoritmo de euclides extendido:\n");
    }
    else{
        gmp_printf  ("\nmcd(%Zd,%Zd) = %Zd => NO SON COPRIMOS\n", b, a, rplus1);
        gmp_printf  ("%Zd en Z_%Zd no tiene inverso multiplicativo modular\n", a, b);
        /* Limpiamos estructuras de gmp */
        mpz_clear (a);
        mpz_clear (b);
        mpz_clear (r);
        mpz_clear (rplus1);
        mpz_clear (rplus2);
        mpz_clear (coc);
        mpz_clear (x);
        mpz_clear (c);
        /* Devolvemos ERROR */
        return ERROR;
    }

    /* Bucle de recursión del algoritmo de Euclides extendido */
    mpz_set_str (x,"1",10);
    mpz_add_ui(c,rs[cont*4 + 1],0);
    cont--;
    signo = 0;
    while(cont > -1){
      /* Aqui usamos los datos del array guardados en Euclides normal */
      gmp_printf ("%Zd = %Zd - %Zd x %Zd\n", rs[cont*4 + 3], rs[cont*4], rs[cont*4 + 1], rs[cont*4 + 2]);
      mpz_add_ui (r,c,0);
      mpz_mul (c,c,rs[cont*4 + 1]);
      mpz_add (c,x,c);
      mpz_add_ui (x,r,0);
      cont--;
      if (signo) signo = 0;
      else signo = 1;
    }

    /* Calculamos el inverso multiplicativo (inv = c*a mod m) */
    if (!signo) mpz_mul_si (c,c,-1);
    gmp_printf ("\n1 = %Zd*%Zd mod %Zd\n", c, a, b);
    mpz_fdiv_r (c, c, b);
    gmp_printf ("Inverso de %Zd en Z_%Zd es = %Zd\n\n", a, b, c);

    /* Asignamos el valor del inverso a inv */
    mpz_add_ui(inv,c,0);

    /* Limpiamos estructuras de gmp */
	  mpz_clear (a);
    mpz_clear (b);
    mpz_clear (r);
    mpz_clear (rplus1);
    mpz_clear (rplus2);
    mpz_clear (coc);
    mpz_clear (x);
    mpz_clear (c);

    /* Devolvemos OK */
    return OK;
}
