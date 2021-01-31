/*
Desc: Cifra con el algoritmo DES simple y el modo de operacion CBC
      ./desCBC {-C | -D} {-k clave -iv vectorinicializacion} [-i filein] [-o fileout]
      -C proceso de cifrado
      -D proceso de descifrado
      -k claves (cadena de 8 caracteres = 64 bits)
      -iv vector de inicializacion (cadena de 8 caracteres = 64 bits)
      -i fichero de entrada
      -o fichero de salida
Authors: Daniel Mateo
         Laura Sanchez
File: desCBC.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des_tables.h"
#include "funciones_DES_CBC.h"

int main(int argc, char *argv[]) {
  char ifile[50], ofile[50];
  char clave[9], iv[8], bloque[8], bloque_aux[9], bloque_auxdesc[9], bloque_cn1[9];
  char bloque_p[9]; // Contiene la permutacion inicial
  char clave_p1[7]; // Contiene la clave permutada en PC1
  char subclave_p2[6]; // Contiene la subclave permutada en PC2
  char la[4], ra[4], ln[4], rn[4]; // Contienen mitades del bloque (anterior y n)
  char frk[4]; // Salida de funcion F de la ronda
  char** subclaves = NULL; // Estructura con las 16 subclaves
  char *texto_fichero = NULL;
  char texto_aux[256];
  FILE *i, *o;
  int modo, j, k, ronda, cont, flagc, flagfinal = 0;
  double tamf;
  char* texto = NULL;
  char c; // Caracter leido

  /* Comprobamos numero de argumentos (como minimo 6) */
  if(argc < 6){
    printf("Error en los argumentos. Introduce:\n");
    printf("./desCBC {-C | -D} {-k clave -iv vectorinicializacion} [-i filein -o fileout]\n");
    printf("[-i] [-o] son opcionales, pero si indicas -i tienes que indicar -o y viceversa.\n");
    return ERROR;
  }

  /* Leemos los argumentos pasados por terminal */
  /* modo es la opcion elegida: TECLADO, IFILE, OFILE, IOFILES */
  modo = read_args_des(argv, argc, clave, iv, ifile, ofile, 0);
  if(modo == ERROR){
    return ERROR;
  }

  printf("Clave:\n");
  print_binary_bytes(clave, 8);
  printf("Vector de inicializacion:\n");
  print_binary_bytes(iv, 8);
  printf("\n");

  if(strcmp(argv[1],"-C") == 0){
    printf("Ha elegido la opcion de cifrado...\n");
    flagc = 1;
  }else if (strcmp(argv[1],"-D") == 0){
    printf("Ha elegido la opcion de descifrado...\n");
    flagc = 0;
  }else{
    printf("Debe elegir la opcion de cifrado {-C} o de descifrado {-D}\n");
    return ERROR;
  }

  /* Modo TECLADO */
  if(modo == TECLADO){
      /* Ej de la pagina:
      http://page.math.tu-berlin.de/~kant/teaching/hess/krypto-ws2006/des.htm */
      printf("\n######CIFRADO/DESCIFRADO DE EJEMPLO######\n");

      texto = (char*)calloc(8+1,sizeof(char));
      if(!texto){
        printf("Error al reservar memoria.\n");
        return ERROR;
      }

      if(flagc){
        /* Texto Plano = 0x0123456789ABCDEF */
        texto[0] = 0x01;
        texto[1] = 0x23;
        texto[2] = 0x45;
        texto[3] = 0x67;
        texto[4] = 0x89;
        texto[5] = 0xAB;
        texto[6] = 0xCD;
        texto[7] = 0xEF;
        texto[8] = 0x00;
        printf("Texto plano = ");
        print_binary_bytes(texto, 8);
      }
      else{
        /* Texto Cifrado = 0x85E813540F0AB405 */
        texto[0] = 0x85;
        texto[1] = 0xE8;
        texto[2] = 0x13;
        texto[3] = 0x54;
        texto[4] = 0x0F;
        texto[5] = 0x0A;
        texto[6] = 0xB4;
        texto[7] = 0x05;
        texto[8] = 0x00;
        printf("Texto cifrado = ");
        print_binary_bytes(texto, 8);
      }

      /* Clave = 0x133457799BBCDFF1 */
      clave[0] = 0x13;
      clave[1] = 0x34;
      clave[2] = 0x57;
      clave[3] = 0x79;
      clave[4] = 0x9B;
      clave[5] = 0xBC;
      clave[6] = 0xDF;
      clave[7] = 0xF1;
      clave[8] = 0x00;
      printf("Clave = ");
      print_binary_bytes(clave, 8);
      printf("\n");
  }
  /* Modo ficheros de entrada y salida */
  else if(modo == IOFILES){
    i =fopen(ifile,"r");
    if(i == NULL){
      printf("Error al abrir el fichero de entrada.\n");
      return ERROR;
    }
    fseek(i, 0L, SEEK_END);
    tamf = ftell(i)/8;
    fclose(i);

    i =fopen(ifile,"r");
    if(i == NULL){
      printf("Error al abrir el fichero de entrada.\n");
      return ERROR;
    }

    o = fopen(ofile, "w");
    if(!o){
      printf("Error al abrir el fichero de salida.\n");
      return ERROR;
    }

  }

  /* Reservamos memoria para la estructura de subclaves */
  subclaves = reserva_estructura_subclaves();
  if(!subclaves) return ERROR;

  /* Permutamos la clave inicial quitando los bits de paridad 64 bits --> 56 bits */
  pc1(clave, clave_p1);
  if(modo == TECLADO){
    printf("K+ = ");
    print_binary_bytes(clave_p1, 7);
  }

  /* Generamos las 16 subclaves mediante la K inicial */
  for(k=0; k < ROUNDS; k++){
    lcs(clave_p1, k);
    copia_palabra_bytes(clave_p1, subclaves[k], 7);
    if(modo == TECLADO){
      printf("k%d = ", k+1);
      print_binary_bytes(subclaves[k], 7);
    }
  }
  printf("\n");

  /* Empezamos a cifrar el texto bloque a bloque */
  cont = 1;
  while(cont <= tamf+1) {

    /* Bucle de lectura de 8 bytes del ejemplo de TECLADO */
    if(modo == TECLADO){
      j = 0;
      for(k=0; k < 8; k++){
        bloque[k] = texto[j];
        bloque_p[k] = '\0';
        j++;
      }
    }
    /* Bucle de lectura de 8 bytes del fichero de entrada */
    else{
      j = 0;
      for(k=0; k < 8; k++){
        c = fgetc(i);
        /*printf("caracter %c\n", c);*/
        if(feof(i) && c == EOF){
          /*printf("Final de fichero 1\n");*/
          flagfinal = 1;
          break;
        }

        bloque[k] = c;
        bloque_cn1[k] = c;
        j++;
        bloque_p[k] = '\0';
      }

    }

    /*if(j==8){
      printf("BLOQUEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
    } else {
      printf("Faltan %d caracteres\n",8-j);
    }*/

    /* Si bloque final, rellenamos si no es multiplo de 8 bytes */
    if(flagfinal == 1){
      /*printf("Entro bloque final %d\n", cont);*/
      if(j != 8){
        for(k=j; k < 8; k++){
          bloque[k] = 0x00;
          bloque_cn1[k] = 0x00;
          bloque_p[k] = 0x00;
        } 
      }
    }
    /*printf("Bloque que vamos a cifrar/descifrar\n");
    print_binary_bytes(bloque, 8);
    printf("\n");*/

    if (flagc == 1 && modo == IOFILES){
      if(cont > 1){
        /* CBC --> Hacemos Pn XOR Cn-1 */
        for(k=0; k<8; k++){
          bloque[k] = bloque[k] ^ bloque_aux[k];
        }
      }
      /* Si se trata del primer bloque hacemos Pn XOR IV */
      else {
        for(k=0; k<8; k++){
          bloque[k] = bloque[k] ^ iv[k];
        }
      }
    }

    /* Aplicamos la permutacon IP inicial al bloque */
    ip(bloque, bloque_p, 0);
    copia_palabra_bytes(bloque_p, bloque, 8);
    if(modo == TECLADO){
      printf("IP Bloque %d = ", cont);
      print_binary_bytes(bloque, 8);
    }

    /* Bucle de 16 rondas DES aplicadas sobre el bloque correspondiente */
    for(ronda=0; ronda<16; ronda++){
      if(modo == TECLADO){
        printf("Bloque ronda %d = ", ronda+1);
        print_binary_bytes(bloque, 8);
      }

      /* Permutamos la clave de la ronda n y comprimimos 56 bits --> 48 bits */
      /* Opcion Cifrado. Claves en orden. */
      if(flagc) pc2(subclaves[ronda], subclave_p2);
      /* Opcion Descifrado. Claves en orden inverso. */
      else pc2(subclaves[15-ronda], subclave_p2);
      if(modo == TECLADO){
        printf("Final k%d = ", ronda+1);
        print_binary_bytes(subclave_p2, 6);
      }

      /* Obtenemos ambas mitades del bloque */
      for(k=0; k<4; k++){
        la[k] = bloque[k];
        ra[k] = bloque[k+4];
      }

      /* Ln = Rn-1 */
      copia_palabra_bytes(ra, ln, 4);
      if(modo == TECLADO){
        printf("L%d = ", ronda+1);
        print_binary_bytes(ln, 4);
      }

      /* Ejecutamos f para Rn-1 */
      f(ra, frk, subclave_p2);

      /* Hacemos Ln-1 + f(Rn-1,Kn) */
      for(k=0; k<4; k++){
        rn[k] = la[k] ^ frk[k];
      }
      if(modo == TECLADO){
        printf("R%d = ", ronda+1);
        print_binary_bytes(rn, 4);
        printf("\n");
      }

      /* Obtenemos ambas mitades del bloque */
      for(k=0; k<4; k++){
        bloque[k] = ln[k];
        bloque[k+4] = rn[k];
      }

    }

    /* Realizamos el SWAP FINAL del DES*/
    for(k=0; k<4; k++){
      bloque_aux[k] = bloque[k];
    }
    for(k=0; k<4; k++){
      bloque[k] = bloque[k+4];
    }
    for(k=0; k<4; k++){
      bloque[k+4] = bloque_aux[k];
    }

    /* Aplicamos la permutacon IP inversa final al bloque cifrado */
    ip(bloque, bloque_p, 1);
    copia_palabra_bytes(bloque_p, bloque, 8);
    if(modo == TECLADO){
      printf("IP-1 Bloque %d = ", cont);
      print_binary_bytes(bloque, 8);
    }

    /* En el caso de estar descifrando DES-1(Cn) XOR Cn-1 */
    if (flagc == 0 && modo == IOFILES){
      if(cont > 1){
        /* CBC --> Hacemos Pn XOR Cn-1 */
        /*print_binary_bytes(bloque, 8);*/
        for(k=0; k<8; k++){
          bloque[k] = bloque[k] ^ bloque_auxdesc[k];
        }
        /*print_binary_bytes(bloque_auxdesc, 8);
        print_binary_bytes(bloque, 8);*/
      }
      /* Si se trata del primer bloque hacemos DES-1(Cn) XOR IV */
      else {
        for(k=0; k<8; k++){
          bloque[k] = bloque[k] ^ iv[k];
        }
      }
    }
    /*printf("\nBloque resultante\n");
    print_binary_bytes(bloque, 8);*/

    /* Guardamos el bloque cifrado Cn para aplicar el CBC en el caso del cifrado*/
    if(flagc) copia_palabra_bytes(bloque, bloque_aux, 8);
    if(!flagc) copia_palabra_bytes(bloque_cn1, bloque_auxdesc, 8);

    /* Escribimos bloque cifrado en fichero de salida */
    if(modo == IOFILES){
      for(k=0; k<8; k++){
        if(flagc && cont == tamf+1){
          if(bloque[k] != 0x00){
            fprintf(o, "%c", bloque[k]);
          }
        }
        else if(!flagc && cont == tamf){
          if(bloque[k] != 0x00){
            fprintf(o, "%c", bloque[k]);
          }
        }
        else{
          fprintf(o, "%c", bloque[k]);
        }
      }
    }
    /* Modo TECLADO */
    else{
      break;
    }

    if(!flagc && cont == tamf) break;

    cont ++;
  }

  /* Cerramos ficheros */
  if(modo == IOFILES){
    fclose(i);
    fclose(o);
  }

  /* Liberamos memoria */
  free(texto);
  libera_estructura_subclaves(subclaves);

  printf("\nMemoria liberada correctamente. Cerrando programa...\n");

  return OK;
}
