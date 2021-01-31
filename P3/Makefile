all: potencia primo vegas 

potencia: potencia.o
	gcc -o potencia potencia.o -L. libgmp.a

primo: primo.o
	gcc -o primo primo.o -L. libgmp.a

vegas: vegas.o euclides_ext.o
	gcc -o vegas vegas.o euclides_ext.o -L. libgmp.a

potencia.o: potencia.c
	gcc -c potencia.c

primo.o: primo.c
	gcc -c primo.c

vegas.o: vegas.c
	gcc -c vegas.c

euclides_ext.o: euclides_ext.c
	gcc -c euclides_ext.c

clean:
	rm -f *.o potencia primo vegas euclides_ext
