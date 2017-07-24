# Makefile with variables and rules
P=sendGPS
OBJECTS= sendGPS.o comm.o readNSI.o
CFLAGS = -g -Wall -O3
LDLIBS=
CC=gcc
all: $(P)
# Generate application
$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) $(OBJECTS)
# Generate object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o$@
clean:
	rm *.o $(P)
