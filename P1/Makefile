all: afin afin_mejorado vigenere kasiski indice_c criptoanalisis_vigenere flujo permutaciones

afin: afin.o euclides_ext.o
	gcc -o afin afin.o euclides_ext.o -L. libgmp.a

afin_mejorado: afin_mejorado.o euclides_ext.o
	gcc -o afin_mejorado afin_mejorado.o euclides_ext.o -L. libgmp.a

vigenere: vigenere.o
	gcc -o vigenere vigenere.o

kasiski: kasiski.o euclides.o
	gcc -o kasiski kasiski.o euclides.o

indice_c: indice_c.o
	gcc -o indice_c indice_c.o

criptoanalisis_vigenere: criptoanalisis_vigenere.o
	gcc -o criptoanalisis_vigenere criptoanalisis_vigenere.o

flujo: flujo.o
	gcc -o flujo flujo.o

permutaciones: permutaciones.o
	gcc -o permutaciones permutaciones.o

afin_mejorado.o: afin_mejorado.c
	gcc -c afin_mejorado.c

euclides_ext.o: euclides_ext.c
	gcc -c euclides_ext.c

euclides.o: euclides.c
	gcc -c euclides.c

afin.o: afin.c
	gcc -c afin.c

vigenere.o: vigenere.c
	gcc -c vigenere.c

kasiski.o: kasiski.c
	gcc -c kasiski.c

indice_c.o: indice_c.c
	gcc -c indice_c.c

criptoanalisis_vigenere.o: criptoanalisis_vigenere.c
	gcc -c criptoanalisis_vigenere.c

flujo.o: flujo.c
	gcc -c flujo.c

permutaciones.o: permutaciones.c
	gcc -c permutaciones.c

clean:
	rm -f *.o afin afin_mejorado vigenere kasiski indice_c criptoanalisis_vigenere flujo permutaciones
