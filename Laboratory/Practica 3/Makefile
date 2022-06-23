# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)

default: Practica3

# To create the executable file Practica3 we need the object files
# Practica3.o:

Practica3: Practica3.o 
	gcc Practica3.o -o Practica3 -lm

# Creating object files
Practica3.o: Practica3.c  
	gcc -c Practica3.c


# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) Practica3 *.o *~


