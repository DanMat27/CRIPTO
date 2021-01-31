/*
Desc: Construye la S-box directa o inversa del cifrado AES.
      ./SBOX_AES {-C | -D} {-o ofile}
      -C calcular S-box directa
      -D calcular S-box inversa
      -o fichero de salida
Authors: Daniel Mateo
         Laura Sanchez
File: SBOX_AES.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SBOX_AES.h"
#include "funciones_DES_CBC.h"

int main(int argc, char const *argv[]) {
  char ofile[50];
  unsigned char **matriz=NULL; // Estructura con la matriz
  unsigned char polinomio; // Cada polinomio de GF(2^8)
  unsigned char inverso_mult; // Inverso multiplicativo
  unsigned char res_afin; // Resultado transformacion afin
  int i, f, c;

  if(argc != 4){
    printf("Error en los argumentos de entrada:\n");
    printf("./SBOX_AES {-C | -D} {-o ofile}\n");
    return ERROR;
  }

  if(strcmp("-o", argv[2]) == 0){
    strcpy(ofile, argv[3]);
  }
  else{
    printf("Error. Debe introducir: -o nombre_fichero\n");
    return ERROR;
  }

  /* Reservamos memoria para la matriz */
  matriz = reserva_matriz();
  if(!matriz){
    printf("Error reservando memoria.\n");
    return ERROR;
  }

  /* Opcion construir matriz directa */
  if(strcmp("-C", argv[1]) == 0){
    printf("Generando S-box Directa del AES...\n");
    /* Para cada polinomio perteneciente a GF(2^8) */
    polinomio = 0x01;
    for(i=1; i<256; i++){

      /* Calculamos el inverso multiplicativo */
      inverso_mult = calcular_inverso_multiplicativo(polinomio);
      if(inverso_mult == ERROR) {
        printf("Se ha encontrado un polinomio no coprimo con m(x).\n");
        return ERROR;
      }

      // res_afin = X * inverso_mult + C;
      res_afin = transformacion_afin(inverso_mult);

      /* Lo metemos en la posición correcta de la matriz */
      f = fila(polinomio);
      c = columna(polinomio);
      matriz[f][c] = res_afin;

      polinomio++;
    }

    matriz[0][0] = 0x63; // Se supone la inversa del 0x00

  }/* Opcion construir matriz inversa */
  else if(strcmp("-D", argv[1]) == 0){
    printf("Generando S-box Inversa del AES...\n");
    /* Para cada polinomio perteneciente a GF(2^8) */
    polinomio = 0x01;
    for(i=0; i<256; i++){
      /* Aplicamos la transformacion afin inversa */
      // res_afin = Y * polinomio + D;
      res_afin = transformacion_afin_inversa(polinomio);

      /* Calculamos el inverso multiplicativo */
      res_afin = calcular_inverso_multiplicativo(res_afin);

      /* Lo metemos en la posición correcta de la matriz */
      f = fila(polinomio);
      c = columna(polinomio);
      matriz[f][c] = res_afin;

      polinomio++;
    }

    matriz[6][3] = 0x00; // Se supone la inversa del 0x00

  }
  else{
    printf("Error debe elegir una de las dos opciones -C o -D\n");
    return ERROR;
  }

  /* Volcamos la matriz resultante en el fichero de salida */
  imprime_matriz_fichero(matriz, ofile);

  /* Liberamos memoria */
  liberar_matriz(matriz);

  printf("\nMemoria liberada correctamente...\n");

  return OK;
}

char transformacion_afin_inversa(char p){
  unsigned char c = 0x05;
  unsigned char res = 0x00;

  res = desplaza_izquierda_circular(p,1) ^ desplaza_izquierda_circular(p,3) ^desplaza_izquierda_circular(p,6) ^ c;

  return res;
}

char transformacion_afin(char p){
  unsigned char c = 0x63;
  unsigned char res = 0x00;
  int i;

  res = p ^ desplaza_izquierda_circular(p,1) ^ desplaza_izquierda_circular(p,2) ^ desplaza_izquierda_circular(p,3) ^ desplaza_izquierda_circular(p,4) ^ c;

  return res;
}

char desplaza_izquierda_circular(char p, int despl){
  int i;
  char aux = 0x00;

  for(i=0; i<despl; i++){
    if(p & (1 << 7-i)) aux |= (1 << (despl - (i+1)));
    else aux &= ~(1 << (despl - (i+1)));
  }

  aux = aux | (p << despl);

  return aux;

}

char calcular_inverso_multiplicativo(char polinomio){
  unsigned char inverso, x, c;
  unsigned char r, rplus1, rplus2, coc; // Para Euclides
  unsigned char rs[1000];
  int cont;

  /* Algoritmo de Euclides mcd(m(x), polinomio)*/
  rplus1 = polinomio;
  rplus2 = dividir_m_polinomio(polinomio, &coc);
  cont = -1;
  r = 0x1B;
  //printf("11b = %x x %x + %x\n", coc, rplus1, rplus2);
  if(rplus2 == 0x00){
    inverso = 0x01;
    return inverso;
  }
  while(rplus2 != 0x00){
      cont++;
      /* Guardamos los restos y el cociente en el array para mas tarde */
      rs[cont*4] = r;
      rs[cont*4 + 1] = coc;
      rs[cont*4 + 2] = rplus1;
      rs[cont*4 + 3] = rplus2;

      /* Calculo de los restos */
      r = rplus1;
      rplus1 = rplus2;
      rplus2 = dividir_polinomios(r, rplus1, &coc);
      //printf("%x = %x x %x + %x\n", r, coc, rplus1, rplus2);
  }

  /* Comprobamos si el MCD es 1 ==> Son coprimos a y m */
   /*if(rplus1 ==  0x01){
       printf("\nSi es coprimo con m(x) = 0x11B\n");
   }
   else{
       printf("\nNo es coprimo con m(x) = 0x11B\n");
       return ERROR;
   }*/

   /* Bucle de recursión del algoritmo de Euclides extendido */
   x = 0x01;
   c = rs[cont*4 + 1];
   cont--;
   while(cont > -1){
     /* Aqui usamos los datos del array guardados en Euclides normal */
     //printf ("%x = %x - %x x %x\n", rs[cont*4 + 3], rs[cont*4], rs[cont*4 + 1], rs[cont*4 + 2]);
     r = c;
     c = producto_polinomios(c, rs[cont*4 + 1]);
     c = x^c;
     x = r;

     cont--;
   }

   /* Calculamos el inverso multiplicativo (inv = c*a mod m) */
   //printf ("\n1 = %x x %x mod 11b\n", c, polinomio);
   inverso = c;
   //printf ("Inverso de %x en GF(2^8) es = %x\n\n", polinomio, inverso);

  return inverso;
}

char dividir_polinomios(char p1, char p2, char *coc){
  int i, grado_p, grado_r;
  int c[] = {0,0,0,0,0,0,0,0};
  unsigned char r, p;
  int flag_final = 0, num_coc = 0, pos_m;

  *coc = 0x00;

  grado_p = calcula_grado(p2);
  grado_r = calcula_grado(p1);
  pos_m = grado_r;
  p = p1;
  r = p >> (grado_r - grado_p);
  pos_m -= (grado_p); // Nos movemos para bajar un bit
  while(flag_final != 1){
    grado_r = calcula_grado(r);

    if(grado_r >= grado_p){
       c[num_coc] = 1; // Aniadimos 1 en cociente
       r = r ^ p2;
    }
    else{
      c[num_coc] = 0; // Aniadimos 0 en cociente
    }

    num_coc++; // Nuevo bit cociente
    pos_m --; // Nos movemos 1 para bajar un bit
    if(pos_m < 0) flag_final = 1; // Final de division
    else{
      r = r << 1;
      if(p1 & (1 << (pos_m))) r = r | 0x01; // Se baja un 1
      else r = r & 0xFE; // Se baja un 0
    }

  }

  for(i=num_coc-1; i>=0; i--){
    if(c[i] == 1) *coc |= (1 << num_coc-(i+1));
    else *coc &= ~(1 << num_coc-(i+1));
  }

  return r;

}

char dividir_m_polinomio(char polinomio, char* coc){
  int i, grado_p, grado_r;
  int c[] = {0,0,0,0,0,0,0,0};
  unsigned char m = 0x1B;
  unsigned char r, p;
  int flag_final = 0, num_coc = 0, pos_m = 7;

  *coc = 0x00;

  /* Desplazamos para quitar los ceros de la izquierda*/
  grado_p = calcula_grado(polinomio);
  p = (polinomio << 8-grado_p);

  /* m/polinomio */
  c[0] = 1; /* Grado de m siempre es mayor que el grado del polinomio */
  num_coc++;
  /* m XOR polinomio */
  r = m ^ p;
  r = (r >> 8-(grado_p+1));

  pos_m -= (grado_p); // Nos movemos para bajar un bit
  if(pos_m < 0) flag_final = 1; // Final de division
  if(m & (1 << pos_m)) r = r | 0x01; // Se baja un 1
  else r = r & 0xFE; // Se baja un 0

  while(flag_final != 1){
    grado_r = calcula_grado(r);

    if(grado_r >= grado_p){
       c[num_coc] = 1; // Aniadimos 1 en cociente
       r = r ^ polinomio;
    }
    else{
      c[num_coc] = 0; // Aniadimos 0 en cociente
    }

    num_coc++; // Nuevo bit cociente
    pos_m --; // Nos movemos 1 para bajar un bit
    if(pos_m < 0) flag_final = 1; // Final de division
    else{
      r = r << 1;
      if(m & (1 << (pos_m))) r = r | 0x01; // Se baja un 1
      else r = r & 0xFE; // Se baja un 0
    }

  }

  for(i=num_coc-1; i>=0; i--){
    if(c[i] == 1) *coc |= (1 << num_coc-(i+1));
    else *coc &= ~(1 << num_coc-(i+1));
  }

  return r;
}

int calcula_grado(char p){
  int i, grado;

  /* Calculamos el grado del polinomio */
  grado = -1;
  for(i=0;i<8;i++){
    if(p & (1 << i)) grado = i;
  }

  return grado;
}

char desplaza_izquierda(char num){
    return (num << 1);
}

char get_bit_n(char p, int n){
    return ((p) & (1 << n));
}

char xtime(char p){
    char v_1b = 27; //Polinomio '1B'
    char bit;

    bit = get_bit_n(p, 7); //Miramos el bit 7
    if(bit == 0){
        return (desplaza_izquierda(p)); //Desplazamiento de p 1 bit a la izquierda
    }
    else{
        return (desplaza_izquierda(p) ^ v_1b); //Desplazamiento de p y XOR con '1B'
    }
}

char producto_polinomios(char p1, char p2){
    char xt; //Polinomio auxiliar para los xtimes
    char sol; //Polinomio con la solucion de p1 * p2
    int i;

    /* Calculamos todos los xtimes de p1 y vamos haciendo los calculos (XORs) */
    /* Se aplica el XOR solo si el bit i de p2 esta a 1 */
    sol = 0;
    xt = p1;
    i = 0;
    while(i < 8){
        if(get_bit_n(p2, i) != 0){
            sol = sol ^ xt; //XOR del xtime y lo que llevamos calculado
        }
        xt = xtime(xt); //Calculamos xtime de la siguiente iteracion
        i++;
    }

    return sol;
}

int fila(char p){
  int f;
  int fila[4];

  /* Bit 3 */
  if(p & (1 << 4)) fila[0] = 1;
  else fila[0] = 0;

  /* Bit 2 */
  if(p & (1 << 5)) fila[1] = 1;
  else fila[1] = 0;

  /* Bit 1 */
  if(p & (1 << 6)) fila[2] = 1;
  else fila[2] = 0;

  /* Bit 0 */
  if(p & (1 << 7)) fila[3] = 1;
  else fila[3] = 0;

  f = convertirDecimal(fila, 4);

  return f;
}

int columna(char p){
  int c;
  int columna[4];

  /* Bit 7 */
  if(p & (1 << 0)) columna[0] = 1;
  else columna[0] = 0;

  /* Bit 6 */
  if(p & (1 << 1)) columna[1] = 1;
  else columna[1] = 0;

  /* Bit 5 */
  if(p & (1 << 2)) columna[2] = 1;
  else columna[2] = 0;

  /* Bit 4 */
  if(p & (1 << 3)) columna[3] = 1;
  else columna[3] = 0;

  c = convertirDecimal(columna, 4);

  return c;
}

void imprime_matriz_fichero(unsigned char **matriz, char *ofile){
  FILE *o;
  int i,j;

  o = fopen(ofile, "w");
  if(o==NULL){
    printf("Error al abrir el fichero de salida\n");
    return;
  }

  for(i=0; i<16; i++){
    for(j=0; j<16; j++){
      fprintf(o, "%x ",matriz[i][j]);
    }
    fprintf(o,"\n");
  }

  fclose(o);
}

unsigned char **reserva_matriz(){
    unsigned char** matriz=NULL;
    int i;

    matriz = (unsigned char**)calloc(16, sizeof(unsigned char*));
    if(!matriz) return NULL;

    for(i=0; i<16; i++){
      matriz[i] = (unsigned char*)calloc(16, sizeof(unsigned char));
      if(!matriz[i]) return NULL;
    }

    return matriz;
}

void liberar_matriz(unsigned char **matriz){
  int i;

  for(i=0; i<16; i++){
    free(matriz[i]);
  }

  free(matriz);
}
