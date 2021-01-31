all: seg_perf desCBC TDEA_CBC linealidad_des avalancha_des linealidad_aes SBOX_AES

seg_perf: seg_perf.o
	gcc -o seg_perf seg_perf.o

desCBC: desCBC.o funciones_DES_CBC.o
	gcc -o desCBC desCBC.o funciones_DES_CBC.o -lm

TDEA_CBC: TDEA_CBC.o funciones_DES_CBC.o
	gcc -o TDEA_CBC TDEA_CBC.o funciones_DES_CBC.o -lm

linealidad_des: linealidad_des.o funciones_DES_CBC.o
	gcc -o linealidad_des linealidad_des.o funciones_DES_CBC.o -lm

avalancha_des: avalancha_des.o funciones_DES_CBC.o
	gcc -o avalancha_des avalancha_des.o funciones_DES_CBC.o -lm

linealidad_aes: linealidad_aes.o funciones_DES_CBC.o
	gcc -o linealidad_aes linealidad_aes.o funciones_DES_CBC.o -lm

SBOX_AES: SBOX_AES.o funciones_DES_CBC.o
	gcc -o SBOX_AES SBOX_AES.o funciones_DES_CBC.o -lm

seg_perf.o: seg_perf.c
	gcc -c seg_perf.c

desCBC.o: desCBC.c
	gcc -c desCBC.c

TDEA_CBC.o: TDEA_CBC.c
	gcc -c TDEA_CBC.c

linealidad_des.o: linealidad_des.c
	gcc -c linealidad_des.c

avalancha_des.o: avalancha_des.c
	gcc -c avalancha_des.c

linealidad_aes.o: linealidad_aes.c
	gcc -c linealidad_aes.c

SBOX_AES.o: SBOX_AES.c
	gcc -c SBOX_AES.c

funciones_DES_CBC.o: funciones_DES_CBC.c
	gcc -c funciones_DES_CBC.c

clean:
	rm -f *.o seg_perf desCBC TDEA_CBC linealidad_des avalancha_des linealidad_aes SBOX_AES
